#ifndef __OCHIMONO_H_INCLUDED__
#define __OCHIMONO_H_INCLUDED__

#include "Board.h"
#include "Block.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

// ゲームの状態
enum game_state {
    gs_before_start,
    gs_start,
    gs_block_dropping,
    gs_block_placed,
    gs_block_erasing,
    gs_block_wait_next_erasing,
    gs_drop_next_block,
    gs_gameover,
};


// ゲーム本体
// 描画は描画クラスに任せる
class Ochimono {
  public:
    uint8_t width;
    uint8_t height;

    // 再描画フラグ
    // trueならば該当部分の再描画が必要
    // 画面描画が遅いので、毎フレーム描画はできない。
    bool redrawBoard;
    bool redrawNextBlock;
    bool redrawCurrentBlock;


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
    // blockShift : 左右枠外にかかった場合、1マスシフトして回転させるか否か
    void rotateBlock(direction dir, bool blockShift = true);

    // ゲームオーバー？
    bool isGameOver();

    // ブロック消去中？
    bool isErasing();

    // チェイン数を取得
    uint8_t currentChain();

    // スコアを取得
    long currentScore();
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
    // 現在の状態
    game_state _state;
    // チェイン数
    uint8_t _chain;
    // スコア
    long  _score;
    // 次のブロック消去待ち時間
    int8_t _waitErasing;

    // ゲーム状態遷移
    void _changeStateTo(game_state newState);
    // ブロックをランダム生成
    Block *_generateBlock();
    // 次のブロックを画面に配置
    void _dropNextBlock();
    // ゲーム継続可能？
    bool _canContinue();
    // ゲームオーバー時の処理
    void _gameOver();
    // 指定したブロックが衝突した？
    bool _isCollided(Block *block);
    // ブロック衝突時の処理
    void _collisionHandler();
    // ブロックの配置
    void _placeCurrentBlock();
    // 配置可能なY座標の最大値
    uint8_t _getDroppableY(uint8_t x, uint8_t y);
    // 消去出来るブロックがあるか？
    bool _hasErasableBlock();
    // 消去できるブロックを取得
    List<BlockPiece> *_getErasableBlock();
    // 消去できるブロックを消す
    void _eraseBlock();
    // 指定座標の周りの同じ色のブロックを返す
    List<BlockPiece> *_getAroundSameColor(uint8_t x, uint8_t y, bool *checked);
    // 消去できるブロックの一覧を返す
    List<BlockPiece> *_findSameColors(piece color, bool *checked);
    
};




#endif
