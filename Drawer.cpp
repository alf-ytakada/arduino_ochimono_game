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
    for (int i = 0 ; i < block->size() ; i++) {
        BlockPiece p    = block->getBlockPiece(i);
        uint16_t color   = this->_translateColor(p.color);
        uint8_t left    = 1 + this->_blockSize * p.x;
        uint8_t top     = 1 + this->_blockSize * p.y;
        // this->tft->fillRect(left, top, this->_blockSize, this->_blockSize, color);
        this->tft->fillCircle(
            left + (this->_blockSize/2),
            top + (this->_blockSize/2),
            this->_blockSize/2, color
        );
    }
}

void Drawer::drawNextBlock(Block *block) {
    // Next:
    this->tft->setCursor(this->_boardWidth * this->_blockSize  + 10, this->_blockSize);
    this->tft->setTextColor(WHITE);
    this->tft->println("Next");

    for (int i = 0 ; i < block->size() ; i++) {
        BlockPiece p    = block->getBlockPiece(i);
        uint16_t color   = this->_translateColor(p.color);
        uint8_t left    = 1 + this->_blockSize * (this->_boardWidth  + 1);
        uint8_t top     = 1 + this->_blockSize * (i + 2);
        this->tft->fillCircle(
            left + (this->_blockSize/2),
            top + (this->_blockSize/2),
            this->_blockSize/2, color
        );
    }

}

void Drawer::drawChain(uint8_t chainCount) {
    uint8_t x   = this->_boardWidth * this->_blockSize  + 10;
    uint8_t y   = this->_blockSize  * 5;

    // 一旦背景で塗りつぶす サイズは適当。
    this->tft->fillRect(x, y, this->_blockSize * 5, this->_blockSize, this->tft->getBackground());

    this->tft->setCursor(x, y);
    this->tft->setTextColor(WHITE);
    this->tft->println(String(chainCount) + " chain!");
}

void Drawer::drawScore(long score) {
    uint8_t x   = this->_boardWidth * this->_blockSize  + 10;
    uint8_t y   = this->_blockSize  * 10;

    // 一旦背景で塗りつぶす サイズは適当。
    this->tft->fillRect(x, y, this->_blockSize * 7, this->_blockSize, this->tft->getBackground());

    this->tft->setCursor(x, y);
    this->tft->setTextColor(WHITE);
    this->tft->println(String("score: ") + score);
}

void Drawer::drawMaxScore(long score) {
    uint8_t x   = this->_boardWidth * this->_blockSize  + 10;
    uint8_t y   = this->_blockSize  * 11;

    // 一旦背景で塗りつぶす サイズは適当。
    this->tft->fillRect(x, y, this->_blockSize * 7, this->_blockSize, this->tft->getBackground());

    this->tft->setCursor(x, y);
    this->tft->setTextColor(WHITE);
    this->tft->println(String("MAX: ") + score);
}

void Drawer::drawGameOver() {
    // game over
    uint8_t boardCenterX    = this->_boardWidth  / 2;
    uint8_t boardCenterY    = this->_boardHeight / 2;
    this->tft->setCursor(
        boardCenterX * this->_blockSize  + 10,
        boardCenterY * this->_blockSize  + 10
    );
    this->tft->setTextColor(WHITE);
    this->tft->println("Game Over");

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

