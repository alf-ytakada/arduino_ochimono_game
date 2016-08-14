#include "Block.h"

uint8_t Block::size() {
    return 2;
}

BlockPiece Block::getBlockPiece(int pos) {
    return this->blockPiece[pos];
}

bool Block::step() {
    this->_step--;
    if (this->_step <= 0) {
        this->move(dir_down);
        this->_step = this->defaultStep;
        return true;
    }
    return false;
}


void Block::move(direction dir) {
    int dx  = 0;
    int dy  = 0;

    if (dir == dir_left) {
        dx  = -1;
    }
    else if (dir == dir_right) {
        dx  = 1;
    }
    else if (dir == dir_up) {
        dy  = -1;
    }
    else if (dir == dir_down) {
        dy  = 1;
        this->_step = this->defaultStep;
    }

    x   += dx;
    y   += dy;
    blockPiece[0].x += dx;
    blockPiece[1].x += dx;
    blockPiece[0].y += dy;
    blockPiece[1].y += dy;

    this->_movedDir = dir;
}

void Block::swap() {
    piece color = this->blockPiece[0].color;
    this->blockPiece[0].color   = this->blockPiece[1].color;
    this->blockPiece[1].color   = color;
}

void Block::undo() {
    if (_movedDir == dir_end)  return;
    else if (_movedDir == dir_left)   move(dir_right);
    else if (_movedDir == dir_right)  move(dir_left);
    else if (_movedDir == dir_down)   move(dir_up);
    else if (_movedDir == dir_up)     move(dir_down);
    
    // undoは何度もさせない
    this->_movedDir = dir_end;
}

void Block::rotate(direction dir) {
    // 回転行列 デフォルトは90度 (dir_right)
    uint8_t rotMatrix[][2]  = {{0, -1}, {1, 0}};

    if (dir == dir_left) {
        // -90度回転行列にする (dir_left)
        rotMatrix[0][1] = 1;
        rotMatrix[1][0] = -1;
    }

    // 原点基点で回転
    int i;
    for (i = 0 ; i < this->size() ; i++) {
        uint8_t oldX    = blockPiece[i].x;
        uint8_t oldY    = blockPiece[i].y;

        uint8_t nx  = (this->x - oldX) * rotMatrix[0][0] + (this->y - oldY) * rotMatrix[0][1];
        uint8_t ny  = (this->x - oldX) * rotMatrix[1][0] + (this->y - oldY) * rotMatrix[1][1];
        blockPiece[i].x = this->x + nx;
        blockPiece[i].y = this->y + ny;
    }
}
