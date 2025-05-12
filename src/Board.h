#pragma once
#include "Tile.h"
#include "Pos.h"
#include "Bonuses/IBonusStrategy.h"
#include "Bonuses/BonusGenerator.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <set>
#include <random>
#include <tuple>
#include <functional>
#include <array>

struct TileMove {
    Tile* tile;        // какой объект Tile двигаем
    Pos   from;        // из какой логической клетки
    Pos   to;          // в какую логическую клетку
};

struct PosHash {
    size_t operator()(const Pos& p) const {
        return p.row * 1000 + p.col;
    }
};

// Класс игрового поля
class Board {
public:
    using BonusCallback = std::function<void(const BonusEvent&)>;

    static const int ROWS = 8, COLS = 8;
    static constexpr float BONUS_PROBABILITY = 0.50f;

    using TileMap = std::unordered_map<Pos, Tile, PosHash>;

    Board();

    // Заполняет поле плитками
    void reset();

    // Проверяет поле на мэтчи и обрабатываем их
    //void processLogical(); // чистая модель: матчи → удаление → падение → бонусы
 

    // Добавляет в окно отрисовки плитки
    void draw(sf::RenderWindow&);

    // Выбор плитки при клике; возвращает true, если произошёл свап
    // Выполняет логику свапа плиток и вызывает соответствующую реакцию поля
    std::optional<std::pair<Pos, Pos>> select(const Pos& position);

    // Выбирает позиции selectionNum произвольных (неповторяющихся) плиток в радиусе rad от originPos
    std::vector<Pos> getRandPosInRad(int selectionNum, int rad, const Pos& originPose);

    // Выбирает позиции selectionNum произвольных (неповторяющихся) плиток вне радиуса rad от originPos
    std::vector<Pos> getRandPosBetweenRad(int selectionNum, int OutRad, int innerRad, const Pos& originPose);

    // "Удаляет" плитки по переданным позициям в selectedPose
    void removeSelected(const std::vector<Pos>& selectedPoses);

    // Устанавливает функцию, которую Board вызовет при выпадении бонуса
    void setBonusCallback(BonusCallback cb);

    // Применяет эффект бонуса type на позицию pos
    void applyBonus(BonusEvent& bonus);

    // Перекрашивает плитки в цвет color на позиции pos
    void repaintByPos(TileType color, const Pos& pos);

    // Возвращает случайный генератор
    std::mt19937& getRng();

    // Выдаёт все позиции плиток, которые метчатся
    std::set<Pos> findMatches();

    // Возвращает ссылку на плитку по её логичким координатам на доске
    Tile& getTileByPos(const Pos& pos);

    // Меняет местами плитки на позициях aPos и aPos 
    bool swapTiles(const Pos& aPos, const Pos& bPos);

    // Делает бесцветными заметченные плитки (const set& metchedPoses)
    void removeMatched(const std::set<Pos>& metchedPoses);

    // Логика выдачи мгновенных бонусов в плитки по переданных позициям
    void addInstantBonus(const std::set<Pos>& metchedPoses);

    // Возвращает список всех перемещений для перехода
    // к нормальному сотоянию доски, но не меняет grid_
    std::vector<TileMove> computeCollapseMoves();

    // Применяет перемещения к grid_ 
    void applyCollapseMoves(std::vector<TileMove> const& moves);

    /// Возвращает пиксельные координаты центра клетки с логической позицией pos
    sf::Vector2f cellCenterPx(const Pos& pos) const;

    // Возвращает размер клетки
    float getTileSize() const;

    void syncTilesToLogical();

    TileMap getTileMap() const {
        TileMap map;
        for (int r = 0; r < ROWS; ++r)
            for (int c = 0; c < COLS; ++c)
                map[{r, c}] = grid_[r][c];
        return map;
    }

private:
    // Параметр плитки
    float tileSize_ = 50.f;

    // Поле-состояние того что происходит анимирование
    bool isAnimationsRunning_ = false;

    // Матричное представление поля
    std::vector<std::vector<Tile>> grid_;

    // Буфер для новых плиток (сверху)
    std::vector<Tile> spawnBuffer_;

    // Координаты первой выбранной плитки
    std::optional<Pos> firstSelection_;

    // Случайный генератор
    std::mt19937 rng_;

    // Экземпляр генератор бонусов
    BonusGenerator bonusGen_;

    // Колбэк для Game о выпадении бонусов
    BonusCallback bonusCb_;

    // карта: для каждого Bonus своя стратегия
    std::unordered_map<Bonus, std::unique_ptr<IBonusStrategy>, BonusHash> stratMap_;

    // Проверяет соседство
    bool isAdjacent(const Pos& aPos, const Pos& bPos);

    
};
