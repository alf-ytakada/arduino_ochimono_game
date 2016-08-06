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
    this->_isErasing   = false;

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

    if (this->_isErasing)   return this->_eraseBlock();

    bool stepped    = this->_currentBlock->step();
    if (stepped) {
        this->redrawCurrentBlock  = true;
        this->redrawBoard   = true;
    }
    // ゲームオーバー判定
    if (! this->_canContinue()) {
        this->_isGameOver   = true;
    }

    // 衝突判定
    if (this->_isCollided(this->_currentBlock)) {
        this->_collisionHandler();
        if (this->_hasErasableBlock()) {
            // 削除出来るものがあったら、削除シーケンスへ
            this->_isErasing = true;
        }
        this->redrawBoard   = true;
        this->redrawCurrentBlock    = true;
        this->redrawNextBlock    = true;
    }

}

void Ochimono::pause(bool is_pause) {
    // 作るかも
}

void Ochimono::moveBlock(direction dir) {
    this->_currentBlock->move(dir);

    if (this->_isCollided(this->_currentBlock)) {
        // 下方向移動だったなら配置する
        if (dir == dir_down) {
            this->_collisionHandler();
            if (this->_hasErasableBlock()) {
                // 削除出来るものがあったら、削除シーケンスへ
                this->_isErasing = true;
            }
        }
        // それ以外ならば単純に位置を戻す
        else {
            this->_currentBlock->undo();
        }
    }
    this->redrawBoard   = true;
    this->redrawCurrentBlock    = true;
    this->redrawNextBlock    = true;
}

void Ochimono::rotateBlock(direction dir) {
    if (this->_currentBlock) {
        this->_currentBlock->rotate(dir);
        // もし回転後がヒットしちゃったら戻す
        if (this->_isCollided(this->_currentBlock)) {
            this->_currentBlock->rotate((dir == dir_left) ? dir_right : dir_left);
        }
        else {
            this->redrawBoard   = true;
            this->redrawCurrentBlock    = true;
        }
    }
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
    uint8_t color1   = 1 + rand() % (piece_end -1);
    uint8_t color2   = 1 + rand() % (piece_end -1);
    piece pc1       = (piece)color1;
    piece pc2       = (piece)color2;

    // TODO Lv調整
    uint8_t lv  = 100;
    return new Block(2, 0, pc1, pc2, lv);
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
        // 出現口がふさがっている＝NG
        return false;
    }
    return true;
}

void Ochimono::_gameOver () {
    // なんかやる
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
    // 場所を戻して
    this->_currentBlock->undo();
    // 配置する
    this->_placeCurrentBlock();
    // 次のBlockを配置する
    this->_dropNextBlock();

}

void Ochimono::_placeCurrentBlock() {
    int i;
    // alias
    Block *cb   = this->_currentBlock;


    // Y座標が大きい物順に並べたBlockPieceのindex保持配列。
    uint8_t *ySortedIdx = new uint8_t[this->_currentBlock->size()];

    for (i = 0 ; i < cb->size() ; i++) {
        ySortedIdx[i]   = i;
        for (int j = i ; j > 0 ; j--) {
            if (cb->getBlockPiece(j).y > cb->getBlockPiece(j-1).y) {
                uint8_t tmp     = ySortedIdx[j-1];
                ySortedIdx[j-1] = ySortedIdx[j];
                ySortedIdx[j]    = tmp;
            }
        }
    }

    for (i = 0 ; i < this->_currentBlock->size() ; i++) {
        BlockPiece p    = this->_currentBlock->getBlockPiece(ySortedIdx[i]);
        // 下に落とせるだけ落とす
        uint8_t placeY  = this->_getDroppableY(p.x, p.y);

        this->_board->set(p.x, placeY, p.color);
    }
}


uint8_t Ochimono::_getDroppableY(uint8_t x, uint8_t y) {
    while (this->_board->get(x, y+1) == piece_none) {
        y++;
    }
    return y;
}

#include <Arduino.h>
// 消去出来るブロックがあるか？
bool Ochimono::_hasErasableBlock() {
    auto *blocks    = this->_getErasableBlock();
    if (blocks == NULL) {
        Serial.println("no erasable blocks");
        return false;
    }
    delete blocks;
    Serial.println("yes erasable blocks");
    return true;
}


List<BlockPiece> *Ochimono::_getErasableBlock() {
    List<BlockPiece> *ret   = new List<BlockPiece>(16);
    //bool checked[this->height * this->width]    = {false};
    bool *checked = new bool[this->height * this->width];
    for (int i = 0 ; i < this->height * this->width ; i++) {
        checked[i]  = false;
    }

    for (uint8_t y = 0 ; y < this->height ; y++) {
        for (uint8_t x = 0 ; x < this->width ; x++) {

            piece color = this->_board->get(x, y);
            uint8_t pos = y * this->width + x;
            checked[pos]    = true;
            if (color == piece_none)  continue;

            // とりあえず16個分メモリ確保
            List<BlockPiece> sameColors(16);
            sameColors.push(BlockPiece(x, y, color));

            while (sameColors.size() > 0) {
                Serial.println(sameColors.size());
                BlockPiece bp   = sameColors.pop();
                ret->push(bp);
                // 周りの同じ色のブロックをチェック
                List<BlockPiece> *aroundSameColors    = 
                    this->_getAroundSameColor(bp.x, bp.y, checked);

                // 同じ色のブロック群をチェック対象に追加
                while (aroundSameColors->size() > 0) {
                    sameColors.push(aroundSameColors->pop());
                }
                delete aroundSameColors;
            }

            if (ret->size() >= 4) {
                delete checked;
                return ret;
            }
            ret->clear();
        }
    }

    delete checked;
    delete ret;
    return NULL;
}

// 消去できるブロックを消す
void Ochimono::_eraseBlock() {
    auto *blocks    = this->_getErasableBlock();

    while (blocks->size() > 0) {
        BlockPiece bp   = blocks->pop();
        // 消す
        Serial.println(String("delete x = ") + bp.x + ", y = " + bp.y);
        this->_board->set(bp.x, bp.y, piece_none);
    }

    // 空白部分より上にブロックが有るなら全部落とす
    for (uint8_t x = 0 ; x < this->width ; x++) {
        for (int8_t y = this->height -1 ; y >= 0 ; y--) {
            Serial.println(String("(") + x + "," + y + ")");
            piece color = this->_board->get(x, y);
            if (color != piece_none) continue;
            // 上にブロックある？
            for (int8_t upperY = y-1 ; upperY >= 0 ; upperY--) {
                piece upperColor    = this->_board->get(x, upperY);
                if (upperColor != piece_none) {
                    // 見つけたので、動かす
                    this->_board->set(x, y, upperColor);
                    this->_board->set(x, upperY, piece_none);
                    break;
                }
            }
        }
    }

    delete blocks;
    if (! this->_hasErasableBlock()) {
        // 削除完了とする
        this->_isErasing    = false;
    }
    this->redrawBoard   = true;
    this->redrawCurrentBlock    = true;
}

// 指定した位置のブロックの周りをチェックし、
// 同じ色の座標一覧を返す
List<BlockPiece> *Ochimono::_getAroundSameColor(uint8_t x, uint8_t y, bool *checked) {
    List<BlockPiece> *ret    = new List<BlockPiece>(4);

    piece color = this->_board->get(x, y);
    checked[y * this->width + x] = true;


    // 上 下 左 右 の差分
    uint8_t diffX[]  = {0,  0, -1, 1};
    uint8_t diffY[]  = {-1, 1,  0, 0};

    for (int i = 0 ; i < 4 ; i++) {
        uint8_t cx  = x + diffX[i];
        uint8_t cy  = y + diffY[i];
        if (checked[cy * this->width + cx])  continue;

        if (this->_board->get(cx, cy)  == color) {
            ret->push(BlockPiece(cx, cy, color));
            checked[cy * this->width + cx] = true;
        }
    }

    return ret;
}

List<BlockPiece> *Ochimono::_findSameColors(piece color, bool *checked) {

    return NULL;
}
