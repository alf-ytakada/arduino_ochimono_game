#ifndef __DRAWER_H_INCLUDED__
#define __DRAWER_H_INCLUDED__

#include "Ochimono.h"
#include "Board.h"
#include "Block.h"
#include "common.h"
#include <TFT_ST7735.h>


class Drawer {
  public:
    TFT_ST7735 *tft;
    Drawer(TFT_ST7735 *tft, uint8_t boardWidth, uint8_t boardHeight) {
        this->tft   = tft;

        this->_screenWidth  = tft->width();
        this->_screenHeight = tft->height();

        this->_boardWidth    = boardWidth;
        this->_boardHeight   = boardHeight;
    }

    // 現在の状態を描画する
    void draw(Ochimono *game);

    void drawBoard(Board *board);
    void drawCurrentBlock(Block *block);
    void drawNextBlock(Block *block);
    void drawChain(uint8_t chainCount);
    void drawScore(long score);
    void drawMaxScore(long score);
    void drawGameOver();

  private:
    uint8_t _blockSize  = 10;
    uint8_t _screenWidth;
    uint8_t _screenHeight;
    uint8_t _boardWidth;
    uint8_t _boardHeight;

    uint16_t _translateColor(piece color);

};

#endif
