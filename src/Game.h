﻿#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Bonuses/IBonusStrategy.h"
#include <queue>
#include "Animations/AnimationManager.h"
#include "Pos.h"

enum class GameState { MENU, WAIT_FOR_INPUT, WAIT_SWAP, CHECK_SWAP_MATCH,
    WAIT_SWAP_BACK, WAIT_REMOVE, WAIT_REMOVE_ANIM, WAIT_BONUS_FALL,
    APPLY_BONUS, WAIT_COLLAPSE, CHECK_POST_COLLAPSE
};

class Game {
public:
    Game();
    void run();

private:
    sf::Font bonusFont_;
    sf::RenderWindow window_;
    Board board_;
    GameState state_;

    AnimationManager animMgr_;
    std::optional<Pos> pendingSwapA_, pendingSwapB_;
    std::set<Pos> lastMatches_;

    std::vector<TileMove> pendingCollapseMoves_;

    // очередь бонусов, которые должны обработаться
    std::queue<BonusEvent> bonusQueue_;

    // текущее событие
    std::optional<BonusEvent> currentBonus_;

    // для анимации падения
    float fallProgress_ = 0.f;
    float fallSpeed_ = 5.0f;
    float collapseDur_ = 0.4f;
    float bonusFallDur_ = 0.3f;
    float swapDur_ = 0.25f;

    // Реализует основной свитч-кейс с сценариями работы
    void updateState();

    void processInput();
    void render(float frameTime);

    // Делает свап в данных + ставит анимацию + меняет состояние
    //bool trySwap(const Pos& a, const Pos& b);

    // Добавляет MoveAnimation для двух меняющихся плиток
    void enqueueSwapAnims(const Pos& aPos, const Pos& bPos, std::function<void()> onDone);

    // Добавляет MoveAnimation для "падающих" сверху клеток
    void enqueueCollapseAnim(std::vector<TileMove>& moves, std::function<void()> onDone);

    // Добавляет анимацию падения бонуса
    void enqueueBonusFallAnim(const Pos& p/*, std::function<void()> onDone*/);

    // Проверка на метчи
    /// запускаем «исчезновение» (scale→0) для matched
    bool checkMetches();

    void enqueueScaleUpAnims(std::vector<TileMove>& move);
};
