#ifndef __BLOCK_H_INCLUDED__
#define __BLOCK_H_INCLUDED__

#include <stdio.h>
#include "common.h"

class BlockPiece {
  public:
    int8_t x;
    int8_t y;
    piece color;
    BlockPiece(){};
    BlockPiece(int8_t x, int8_t y, piece color) : x(x), y(y), color(color) {};
    BlockPiece(const BlockPiece &p) { this->x = p.x; this->y = p.y; this->color = p.color; };
};

// Block
class Block {
  public:
    uint8_t x;
    uint8_t y;
    BlockPiece blockPiece[2];
    // 落下速度 小さいほどすぐ落ちる
    uint8_t defaultStep;

    // コンストラクタ
    // (x,y) = 初期座標、 piece = 色、 step = 落ちる速度 小さいほどすぐ落ちる
    Block(int8_t x, int8_t y, piece p1, piece p2, uint8_t step) {
        this->x = x;
        this->y = y;
        blockPiece[0].x = x;
        blockPiece[0].y = y;
        blockPiece[0].color = p1;
        blockPiece[1].x = x;
        blockPiece[1].y = y + 1;
        blockPiece[1].color = p2;

        _movedDir   = dir_end;
        this->defaultStep    = step;
        this->_step  = step;
    };

    // サイズ 2固定
    uint8_t size();
    BlockPiece getBlockPiece(int pos);

    // 1ステップ進める 0になったら一つ下に動く
    // true: 動いた
    bool step();

    // 動かす
    void move(direction dir);

    // 先ほど動かしたのをなかったことにする。
    void undo();

    // 回転
    // dir_left, dir_rightのみ対応
    void rotate(direction dir);

    // 上下反転
    void swap();

  private:
    direction _movedDir;
    uint8_t _step;

};


#endif
