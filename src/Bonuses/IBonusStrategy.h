#pragma once
#include <random>
#include "../Pos.h"

// Пишем forward-деклараци. для избежания проблем с циклическим инклудом
class Board;

enum class Bonus : int { None = 0, Bomb, Painter, Count };

struct BonusEvent {
    Bonus type;    // какой бонус
    Pos pos;       // из какой клетки «выпал»
    Pos posBonus;  // в какую клетку упал
};

// Хэш-функтор для enum class Bonus 
// чтобы создать карту для удобного выбора применяемого бонуса
struct BonusHash {
    std::size_t operator()(Bonus b) const noexcept {
        // Преобразуем значение Bonus в size_t
        return static_cast<std::size_t>(b);
    }
};

class IBonusStrategy {
public:
    virtual ~IBonusStrategy() = default;

    // Если бонус выпал — вернуть его, иначе Bonus::None
    virtual Bonus tryGenerate(const Pos& p, std::mt19937& rng) = 0;

    // Приеняет эффект бонуса к доске
    virtual void apply(Board& board, const Pos& originPos, const Pos& originBonusPos) = 0;
};