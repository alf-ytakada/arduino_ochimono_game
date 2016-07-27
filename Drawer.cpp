#include "Drawer.h"


void Drawer::draw(Ochimono *game) {
    Board *board    = game->getBoard();
    Block *currentBlock = game->getCurrentBlock();
    Block *nextBlock    = game->getNextBlock();


    // ボード
    this->drawBoard(board);
    // ブロック
    this->drawCurrentBlock(currentBlock);
    this->drawNextBlock(nextBlock);
}


void Drawer::drawBoard(Board *board) {
    this->tft->drawRect(
        0,
        0,
        this->_blockSize * board->width + 2,
        this->_blockSize * board->height + 2,
        WHITE
    );

    // 中身
    int x, y;
    uint8_t offsetX = 1, offsetY    = 1;
    for (y = 0 ; y < board->height ; y++) {
        for (x = 0 ; x < board->width ; x++) {
            uint8_t left    = offsetX + this->_blockSize * x;
            uint8_t top     = offsetY + this->_blockSize * y;
            uint16_t color  = this->_translateColor(board->get(x, y));
            this->tft->fillRect(left, top, this->_blockSize, this->_blockSize, color);
        }
    }
}


void Drawer::drawCurrentBlock(Block *block) {
    int i;
    for (i = 0 ; i < block->size() ; i++) {
        BlockPiece p    = block->getBlockPiece(i);
        uint16_t color   = this->_translateColor(block->getColor());
        uint8_t left    = 1 + this->_blockSize * p.x;
        uint8_t top     = 1 + this->_blockSize * p.y;
        this->tft->fillRect(left, top, this->_blockSize, this->_blockSize, color);
    }
}

void Drawer::drawNextBlock(Block *block) {
}

uint16_t Drawer::_translateColor(piece color) {
    uint16_t retColor    = BLACK;
    switch (color) {
        case piece_none:  break;
        case piece_blue:  retColor = BLUE;     break;
        case piece_red:   retColor = RED;      break;
        case piece_green: retColor = GREEN;    break;
        case piece_magenta: retColor   = MAGENTA;  break;
        case piece_yellow:  retColor   = YELLOW;   break;
    }
    return retColor;
}

