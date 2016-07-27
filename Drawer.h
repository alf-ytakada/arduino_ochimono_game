#ifndef __DRAWER_H_INCLUDED__
#define __DRAWER_H_INCLUDED__

#include "Ochimono.h"
#include "Board.h"
#include "Block.h"
#include <TFT_ST7735.h>


class Drawer {
  public:
    TFT_ST7735 *tft;
    Drawer(TFT_ST7735 *tft) {
        this->tft   = tft;

        this->_screenWidth  = tft->width();
        this->_screenHeight = tft->height();
    }

    // 現在の状態を描画する
    void draw(Ochimono *game);

    void drawBoard(Board *board);
    void drawBlock(Block *block);

  private:
    uint8_t _blockSize  = 10;
    uint8_t _screenWidth;
    uint8_t _screenHeight;

};

#endif
