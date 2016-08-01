#ifndef __COMMON_H_INCLUDED__
#define __COMMON_H_INCLUDED__

#include <stdio.h>

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

template<typename T>
class List {
  public:
    List(uint8_t size = 10) {
        this->_array = new T[size];
        this->_head  = this->_array;
        this->_tail  = this->_array;
        this->_size  = 0;
        this->_arraySize    = size;
    };
    ~List() {
        delete [] this->_array;
    };

    T first();
    T last();
    uint8_t size();
    void clear();
    // 後ろに追加
    void push(T t);
    // 後ろから取得
    T pop();
    // TODO
    //// 前から取得
    //T shift();
    //// 前に追加
    //void unshift(T t);

  private:
    T *_head;
    T *_tail;
    T *_array;
    uint8_t _size;

    uint8_t _arraySize;

    

};


#endif
