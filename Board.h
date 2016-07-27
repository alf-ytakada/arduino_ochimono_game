#ifndef __BOARD_H_INCLUDED__
#define __BOARD_H_INCLUDED__

#include <stdio.h>
#include "common.h"

// 今回はオチモノ用のボードとしています。


class Board {
  public:
    uint8_t length;
    uint8_t width;
    uint8_t height;
    // コンストラクタ
    Board(uint8_t width, uint8_t height) {
        this->_pieces  = new piece[width * height + 1];
        this->length   = width * height;
        this->width    = width;
        this->height   = height;
        int i;
        for (i = 0 ; i < length ; i++) {
            this->_pieces[i] = piece_none;
        }
        this->_pieces[this->length] = piece_end;
    };

    // 指定位置のpiece
    piece get(int x, int y);
    void set(int x, int y, piece p);

    // _pieces自体
    piece *getPieces();


    ~Board() {
        delete _pieces;
    }
  private:
    piece *_pieces;


};


#endif
