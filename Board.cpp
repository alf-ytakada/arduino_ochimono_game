#include "Board.h"

piece Board::get(int x, int y) {
    if (y < 0 || this->height <= y) {
        return piece_end;
    }
    if (x < 0 || this->width <= x) {
        return piece_end;
    }
    return this->_pieces[y * this->width + x];
}

void Board::set(int x, int y, piece p) {
    this->_pieces[y * this->width + x]   = p;
}

// _pieces自体
piece *Board::getPieces() {
    return this->_pieces;
}

