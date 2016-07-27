#ifndef __OCHIMONO_H_INCLUDED__
#define __OCHIMONO_H_INCLUDED__

#include "Board.h"
#include "Block.h"
#include "common.h"
#include <stdio.h>


// ゲーム本体
// 描画は描画クラスに任せる
class Ochimono {
  public:
    uint8_t width;
    uint8_t height;
    // コンストラクタ
    // width: 盤面の横幅、height:盤面の縦幅
    Ochimono(uint8_t width, uint8_t height) {
        this->width     = width;
        this->height    = height;
        init();
    }

    //////////////////
    //// ユーザ用関数
    // ゲーム初期化
    void init();

    // ゲームスタート
    void start();

    // メインループ
    void mainLoop();

    // ポーズ(true:ポーズする、false:ポーズ解除)
    void pause(bool is_pause);

    // ブロックを動かす
    void moveBlock(direction dir);

    // ブロックを回転する
    void rotateBlock(direction dir);
    //////////////////

    //////////////////
    //// 描画クラス用関数
    // 盤面を返す
    Board *getBoard();
    Block *getCurrentBlock();
    Block *getNextBlock();


    //////////////////
  protected:
    Board *_board;
    Block *_currentBlock;
    Block *_nextBlock;
    bool _isStarted;
    bool _isGameOver;

    // ブロックをランダム生成
    Block *_generateBlock();
    // 次のブロックを画面に配置
    void _dropNextBlock();
    // ゲーム継続可能？
    void _canContinue();
    // ゲームオーバー時の処理
    void _gameOver();
    // 指定したブロックが衝突した？
    void _isCollided(Block &block);
    // ブロック衝突時の処理
    void _collisionHandler();
    // ブロックの配置
    void _placeCurrentBlock();
    
};




#endif
