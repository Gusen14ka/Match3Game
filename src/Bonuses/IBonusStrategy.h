#pragma once
#include <random>
#include "../Pos.h"

// ����� forward-���������. ��� ��������� ������� � ����������� ��������
class Board;

enum class Bonus : int { None = 0, Bomb, Painter, Count };

struct BonusEvent {
    Bonus type;    // ����� �����
    Pos pos;       // �� ����� ������ ������
    Pos posBonus;  // � ����� ������ ����
};

// ���-������� ��� enum class Bonus 
// ����� ������� ����� ��� �������� ������ ������������ ������
struct BonusHash {
    std::size_t operator()(Bonus b) const noexcept {
        // ����������� �������� Bonus � size_t
        return static_cast<std::size_t>(b);
    }
};

class IBonusStrategy {
public:
    virtual ~IBonusStrategy() = default;

    // ���� ����� ����� � ������� ���, ����� Bonus::None
    virtual Bonus tryGenerate(const Pos& p, std::mt19937& rng) = 0;

    // �������� ������ ������ � �����
    virtual void apply(Board& board, const Pos& originPos, const Pos& originBonusPos) = 0;
};