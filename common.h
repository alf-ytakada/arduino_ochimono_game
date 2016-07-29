#ifndef __COMMON_H_INCLUDED__
#define __COMMON_H_INCLUDED__

// 移動方向
enum direction {
    dir_up,
    dir_left,
    dir_down,
    dir_right,
    dir_end,
};


// ブロックに配置されるもの
// 青・赤・緑・紫・黄
enum piece {
    piece_none = 0,
    piece_blue,
    piece_red,
    piece_green,
    piece_magenta,
    piece_yellow,
    piece_end, // 番兵
};



#endif
