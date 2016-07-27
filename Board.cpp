#include "Board.h"

piece Board::get(int x, int y) {
    return this->_pieces[y * this->width + x];
}

void Board::set(int x, int y, piece p) {
    this->_pieces[y * this->width + x]   = p;
}

// _pieces自体
piece *Board::getPieces() {
    return this->_pieces;
}

