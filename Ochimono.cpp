#include "Ochimono.h"


void Ochimono::init() {
    if (this->_board) {
        delete this->_board;
    }
    this->_board    = new Board(this->width, this->height);
    if (this->_currentBlock) {
        delete this->_currentBlock;
    }
    if (this->_nextBlock) {
        delete this->_nextBlock;
    }
    this->_isStarted   = false;
    this->_isGameOver  = false;

    this->redrawBoard       = true;
    this->redrawNextBlock   = true;
    this->redrawCurrentBlock    = true;
}


void Ochimono::start() {
    this->_isStarted   = true;

    this->_nextBlock    = this->_generateBlock();
    this->_dropNextBlock();
}

void Ochimono::mainLoop() {
    if (! this->_isStarted)  return;

    if (this->_isGameOver)   return this->_gameOver();

    bool stepped    = this->_currentBlock->step();
    if (stepped) {
        this->redrawCurrentBlock  = true;
        this->redrawBoard   = true;
    }
    // �Q�[���I�[�o�[����
    if (! this->_canContinue()) {
        this->_isGameOver   = true;
    }

    // �Փ˔���
    if (this->_isCollided(this->_currentBlock)) {
        this->_collisionHandler();
        this->redrawBoard   = true;
        this->redrawCurrentBlock    = true;
        this->redrawNextBlock    = true;
    }

}

void Ochimono::pause(bool is_pause) {
    // ��邩��
}

void Ochimono::moveBlock(direction dir) {
    this->_currentBlock->move(dir);

    if (this->_isCollided(this->_currentBlock)) {
        // �������ړ��������Ȃ�z�u����
        if (dir == dir_down) {
            this->_collisionHandler();
        }
        // ����ȊO�Ȃ�ΒP���Ɉʒu��߂�
        else {
            this->_currentBlock->undo();
        }
    }
}

void Ochimono::rotateBlock(direction dir) {
}

Board *Ochimono::getBoard() {
    return this->_board;
}

Block *Ochimono::getCurrentBlock() {
    return this->_currentBlock;
}

Block *Ochimono::getNextBlock() {
    return this->_nextBlock;
}

///////////////////
// private method
Block *Ochimono::_generateBlock() {
    uint8_t color   = 1 + rand() % (piece_end -1);
    piece pieceColor    = (piece)color;

    // TODO Lv����
    uint8_t lv  = 10;
    return new Block(2, 0, pieceColor, lv);
}

bool Ochimono::isGameOver() {
    return this->_isGameOver;
}

void Ochimono::_dropNextBlock() {
    if (this->_currentBlock) {
        delete this->_currentBlock;
    }
    this->_currentBlock = this->_nextBlock;
    this->_nextBlock    = this->_generateBlock();
}

bool Ochimono::_canContinue() {
    if (this->_board->get(2, 0) != piece_none) {
        // �o�������ӂ������Ă��遁NG
        return false;
    }
    return true;
}

void Ochimono::_gameOver () {
    // �Ȃ񂩂��
}

bool Ochimono::_isCollided(Block *block) {
    int i;
    for (i = 0 ; i < block->size() ; i++) {
        BlockPiece p    = block->getBlockPiece(i);
        if (this->_board->get(p.x, p.y) != piece_none) {
            return true;
        }
    }
    return false;
}

void Ochimono::_collisionHandler() {
    // �ꏊ��߂���
    this->_currentBlock->undo();
    // �z�u����
    this->_placeCurrentBlock();
    // ����Block��z�u����
    this->_dropNextBlock();
}

void Ochimono::_placeCurrentBlock() {
    int i;
    for (i = 0 ; i < this->_currentBlock->size() ; i++) {
        BlockPiece p    = this->_currentBlock->getBlockPiece(i);
        this->_board->set(p.x, p.y, this->_currentBlock->getColor());
    }
}
