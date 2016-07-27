#include "Drawer.h"


void Drawer::draw(Ochimono *game) {
    Board *board    = game->getBoard();
    Block *currentBlock = game->getCurrentBlock();
    Block *nextBlock    = game->getNextBlock();


    // ボード
    this->drawBoard(board);
    // ブロック
    this->drawBlock(currentBlock);
}


void Drawer::drawBoard(Board *board) {
    this->tft->drawRect(
        0,
        0,
        this->_blockSize * board->width,
        this->_blockSize * board->height,
        WHITE
    );

    // 中身
    int x, y;
    for (y = 0 ; y < board->height ; y++) {
        for (x = 0 ; x < board->width ; x++) {
            uint8_t left    = 0 + this->_blockSize * x;
            uint8_t top     = 0 + this->_blockSize * y;
            uint8_t color   = BLACK;
            switch (board->get(x, y)) {
                case piece_none:  break;
                case piece_blue:  color = BLUE;     break;
                case piece_red:   color = RED;      break;
                case piece_green: color = GREEN;    break;
                case piece_magenta: color   = MAGENTA;  break;
                case piece_yellow:  color   = YELLOW;   break;
            }
            this->tft->fillRect(left, top, this->_blockSize, this->_blockSize, color);
        }
    }
}


void Drawer::drawBlock(Block *block) {
}

