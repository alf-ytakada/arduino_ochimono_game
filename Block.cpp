#include "Block.h"

uint8_t Block::size() {
    return 2;
}

BlockPiece Block::getBlockPiece(int pos) {
    return this->blockPiece[pos];
}

piece Block::getColor() {
    return this->color;
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
    }

    x   += dx;
    y   += dy;
    blockPiece[0].x += dx;
    blockPiece[1].x += dx;
    blockPiece[0].y += dy;
    blockPiece[1].y += dy;

    this->_movedDir = dir;
}

void Block::undo() {
    if (_movedDir == dir_end)  return;
    else if (_movedDir == dir_left)   move(dir_right);
    else if (_movedDir == dir_right)  move(dir_left);
    else if (_movedDir == dir_down)   move(dir_up);
    else if (_movedDir == dir_up)     move(dir_down);
    
    // undo‚Í‰½“x‚à‚³‚¹‚È‚¢
    this->_movedDir = dir_end;
}
