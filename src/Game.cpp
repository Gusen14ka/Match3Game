#include "Game.h"
#include <iostream>
#include "Animations/MoveAnimation.h"
#include "Animations/ScaleAnimation.h"
#include "Animations/BonusAnimation.h"
#include <filesystem>

Game::Game()
    : window_({ Board::COLS * 50, Board::ROWS * 50 }, "Match3Game")
    , board_()
    , state_(GameState::WAIT_FOR_INPUT)
{
    auto exePath = std::filesystem::current_path();
    std::filesystem::path fontFile = exePath / "Includes" / "Arial.ttf";
    // загрузим шрифт (поместите рядом с exe файл Arial.ttf или любой другой)
    if (!bonusFont_.loadFromFile(fontFile.string())) {
        std::cerr << "Failed to load font\n";
    }
    board_.setBonusCallback([this](const BonusEvent& event) {
        bonusQueue_.push(event);
        });
}

void Game::run() {
    const float ANIM_STEP = 1.f / 60.f;
    float lag = 0.f;
    sf::Clock clock;
    while (window_.isOpen()) {
        float frameTime = clock.restart().asSeconds();
        lag += frameTime;
        processInput();
        
        updateState();
        render(frameTime);

    }
}

void Game::updateState() {
    switch (state_)
    {
    case GameState::MENU:
        break;

        // Ждём клика
    case GameState::WAIT_FOR_INPUT:
        break;

        // После клика мы сразу сделали логический swap и запустили анимацию
    case GameState::WAIT_SWAP:
        break;

        // Проверяем: есть ли матчи после свапа?

    case GameState::CHECK_SWAP_MATCH:
        if (checkMetches()) {
            std::cout << "CHECK_SWAP_MATCH match" << std::endl;
            state_ = GameState::WAIT_REMOVE;
        }
        else
        {
            std::cout << "CHECK_SWAP_MATCH NO match" << std::endl;
            // откат неудачного свапа
            board_.swapTiles(pendingSwapB_.value(), pendingSwapA_.value());
            enqueueSwapAnims(pendingSwapB_.value(), pendingSwapA_.value(), [this] {
                // после откатной анимации вернёмся в ожидание ввода
                state_ = GameState::WAIT_FOR_INPUT;
                pendingSwapA_.reset();
                pendingSwapB_.reset();
                });
            state_ = GameState::WAIT_SWAP;
        }
        break;

        // Обратный свап, в случае неудачного свапа
    case GameState::WAIT_SWAP_BACK:
        /*
        if (animMgr_.empty()) {
            state_ = GameState::WAIT_FOR_INPUT;
        }
        */
        break;

        // Ждём, пока исчезнут matched плитки
    case GameState::WAIT_REMOVE:
        if (animMgr_.empty()) {
            // После завершения анимаций запускаем логику удаления совпадений 
            // добавляем бонусы и запускае анимацию падения
            board_.removeMatched(lastMatches_);
            board_.addInstantBonus(lastMatches_);

            
            // сброс scale у всех (чтобы не осталось 0)
            for (int r = 0; r < Board::ROWS; ++r)
                for (int c = 0; c < Board::COLS; ++c)
                    board_.getTileByPos({ r,c }).setScale(1.f, 1.f);

            std::cout << "WAIT_REMOVE NONE BONUS\n";
            auto moves = board_.computeCollapseMoves();
            board_.applyCollapseMoves(moves);

            enqueueCollapseAnim(moves, [this] {
                state_ = GameState::CHECK_POST_COLLAPSE;
                });
            state_ = GameState::WAIT_COLLAPSE;
                

                /*
                for (auto& m : moves) {
                    std::cout << m.from.row << "," << m.from.col << " -> "
                        << m.to.row << "," << m.to.col << std::endl;
                }
                board_.syncTilesToLogical();
                state_ = GameState::CHECK_POST_COLLAPSE;
                */
            
        }
        break;

        // Ждём падения бонуса
    case GameState::WAIT_BONUS_FALL:
        //std::cout << "WAIT_BONUS_FALL\n";
        
        if (animMgr_.empty()) {
            
            state_ = GameState::CHECK_POST_COLLAPSE;
            
        }
        
        break;
        // Применяем бонус
    case GameState::APPLY_BONUS:
        if (currentBonus_) {
            board_.applyBonus(currentBonus_.value());
            currentBonus_.reset();
            std::cout << "APPLYING BONUS\n";
        }
        else {
            auto moves = board_.computeCollapseMoves();
            board_.applyCollapseMoves(moves);
            enqueueCollapseAnim(moves, [this] {
                state_ = GameState::CHECK_POST_COLLAPSE;
                std::cout << "FROM APPLYING BONUS TO CHECK_POST_COLLAPSE\n";
                });
            state_ = GameState::WAIT_BONUS_FALL;
        }
        break;
        
        // Ждём падения новых клеток
    case GameState::WAIT_COLLAPSE:
        //std::cout << "WAIT_COLLAPSE \n";
        break;
        // Проверяем новые метчи после коллапса
    case GameState::CHECK_POST_COLLAPSE:
        std::cout << "CHECK_POST_COLLAPSE" << std::endl;
        /*
        if (!bonusQueue_.empty()) {
            state_ = GameState::WAIT_COLLAPSE;
        }
        */
        if (checkMetches()) {
            std::cout << "CHECK_POST_COLLAPSE in check true" << std::endl;
            state_ = GameState::WAIT_REMOVE;
        }
        else
        {
            if (!bonusQueue_.empty()) {
                std::cout << "BONUS IN CHECK_POST_COLLAPSE\n";
                auto counter = std::make_shared<size_t>(0);
                size_t total = bonusQueue_.size();
                while (!bonusQueue_.empty()) {
                    currentBonus_ = bonusQueue_.front();
                    bonusQueue_.pop();
                    auto anim = std::make_unique<BonusAnimation>(
                        window_,
                        [this](const Pos& p) { return board_.cellCenterPx(p); },
                        bonusFont_,
                        currentBonus_->posBonus,
                        currentBonus_->type == Bonus::Bomb ? 'B' : 'P',
                        0.7f
                    );
                    anim->setOnComplete([this, counter, total] {
                        if (++(*counter) == total) {
                            state_ = GameState::APPLY_BONUS;
                        }
                        });
                    animMgr_.add(std::move(anim));
                }


                state_ = GameState::WAIT_BONUS_FALL;
            }
            else {
                std::cout << "CHECK_POST_COLLAPSE in check FALSE" << std::endl;
                // Иначе возвращаем управление игроку
                state_ = GameState::WAIT_FOR_INPUT;
            }
        }
        break;
    default:
        break;
    }
}

void Game::processInput() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
            return;
        }

        if (state_ == GameState::WAIT_FOR_INPUT 
            && event.type == sf::Event::MouseButtonPressed) 
        {
            auto pos = sf::Mouse::getPosition(window_);
            int col = pos.x / board_.getTileSize();
            int row = pos.y / board_.getTileSize();
            if (row >= 0 && row < Board::ROWS && col >= 0 && col < Board::COLS) {
                pendingSwapA_ = Pos{ row, col };
                auto sel = board_.select(pendingSwapA_.value());
                if (sel) {
                    auto [a, b] = sel.value();
                    pendingSwapA_ = a;
                    pendingSwapB_ = b;
                    board_.swapTiles(a, b);
                    // 2) анимируем этот свап, и в колбэке разбираем результат
                    enqueueSwapAnims(a, b, [this] {
                        state_ = GameState::CHECK_SWAP_MATCH;
                        });
                    state_ = GameState::WAIT_SWAP;
                }
            }
        }
    }
}

void Game::render(float frameTime) {
    window_.clear(sf::Color::Black);
    // 1) рисуем фон доски
    float ts = board_.getTileSize();
    for (int r = 0; r < Board::ROWS; ++r) {
        for (int c = 0; c < Board::COLS; ++c) {
            sf::RectangleShape cell({ ts - 2, ts - 2 });
            cell.setPosition(c * ts + 1, r * ts + 1);
            cell.setFillColor(sf::Color(50, 50, 50)); // тёмно‑серый фон
            window_.draw(cell);
        }
    }
    board_.draw(window_);
    animMgr_.updateAll(frameTime);
    window_.display();

}

void Game::enqueueSwapAnims(const Pos& aPos, const Pos& bPos, std::function<void()> onDone) {
    
    auto& tileA = board_.getTileByPos(aPos);
    auto& tileB = board_.getTileByPos(bPos);
    sf::Vector2f pxPosA = tileA.getPxPosition();
    sf::Vector2f pxPosB = tileB.getPxPosition();

    // первая анимация(A → B)
    auto animA = std::make_unique<MoveAnimation<Tile>>(tileA, pxPosA, pxPosB, swapDur_);
    // вторая анимация (B → A), на её onComplete повесим переход состояния
    auto animB = std::make_unique<MoveAnimation<Tile>>(tileB, pxPosB, pxPosA, swapDur_);

    animB->setOnComplete(std::move(onDone));

    animMgr_.add(std::move(animA));
    animMgr_.add(std::move(animB));
    //std::cout << "Anim compl" << std::endl;
}

void Game::enqueueCollapseAnim(std::vector<TileMove>& moves, std::function<void()> onDone) {
    std::cout << "equeue collapse\n";

    if (moves.empty()) { onDone(); return; }

    auto counter = std::make_shared<size_t>(0);
    size_t total = moves.size();
    for (auto const& m : moves) {
        // Берём tile именно из grid_[to] — это гарантированно единственный объект
        Tile& tile = board_.getTileByPos(m.to);

        // Ставим его пиксельно туда, откуда он должен упасть
        sf::Vector2f from_px = board_.cellCenterPx(m.from);
        tile.setPxPosition(from_px);

        // Целевая позиция — логическая клетка to
        sf::Vector2f to_px = board_.cellCenterPx(m.to);

        auto anim = std::make_unique<MoveAnimation<Tile>>(tile, from_px, to_px, collapseDur_);

        anim->setOnComplete([counter, total, onDone]() {
            if (++(*counter) == total && onDone) {
                onDone();
            }
            });

        animMgr_.add(std::move(anim));
    }

}


void Game::enqueueBonusFallAnim(const Pos& p) {
    /*
    auto& tile = board_.getTileByPos(p);
    auto dest = tile.getPxPosition();
    sf::Vector2f top = { dest.x, -static_cast<float>(board_.getTileSize()) };
    tile.setPxPosition(top);
    animMgr_.add(std::make_unique<MoveAnimation<Tile>>(tile, top, dest, bonusFallDur_));
    */
    //board_.syncTilesToLogical();
}

bool Game::checkMetches() {
    auto matches = board_.findMatches();
    if (!matches.empty()) {
        lastMatches_ = std::move(matches);
        // Есть новые матчи — запускаем удаление
        
        for (auto const& p : lastMatches_) {
            auto& tile = board_.getTileByPos(p);
            animMgr_.add(std::make_unique<ScaleAnimation<Tile>>(tile, 1.f, 0.f, 0.2f));
        }
        return true;
        
    }
    return false;
}

void Game::enqueueScaleUpAnims(std::vector<TileMove>& moves) {
    /*
    for (auto const& m : moves) {
        Tile& tile = *m.tile;
        // сразу поставим scale=0, чтобы анимация заметна:
        tile.setScale(0.f, 0.f);
        animMgr_.add(std::make_unique<ScaleAnimation<Tile>>(tile, 0.f, 1.f, 0.2f));
    }
    */
    //board_.syncTilesToLogical();
}
