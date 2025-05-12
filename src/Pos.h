#pragma once

// —труктура дл€ хранени€ "координат" плитки
struct Pos {
    int row, col;
    // ќпредел€ем опрератор дл€ использовани€ set
    auto operator<=>(Pos const&) const = default;
    Pos() : row(0), col(0) {}
    Pos(int row_, int col_) : row(row_), col(col_){}
};