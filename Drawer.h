#ifndef __OCHIMONO_H_INCLUDED__
#define __OCHIMONO_H_INCLUDED__

#include <TFT_ST7735.h>

// 移動方向
enum direction {
    dir_left,
    dir_down,
    dir_right,
    dir_end,
};

// ゲーム本体
// 描画は描画クラスに任せる
class Ochimono {
  public:
    Ochimono(TFT_ST7735 *tft) {
        _tft    = tft;
        init();
    }

    //////////////////
    //// ユーザ用関数
    // ゲーム初期化
    void init();

    // ゲームスタート
    void start();

    // ポーズ(true:ポーズする、false:ポーズ解除)
    void pause(boolean is_pause);

    // ブロックを動かす
    void moveBlock(direction dir);

    // ブロックを回転する
    void rotateBlock(direction dir);
    //////////////////

    //////////////////
    //// 描画クラス用関数
    // 


    //////////////////
  protected:
    TFT_ST7735 *_tft;
    
};




#endif
