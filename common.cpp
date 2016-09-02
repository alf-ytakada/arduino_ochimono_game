#include "common.h"
#include "Block.h"


template<typename T>
T List<T>::first() {
    return this->_array[0];
}

template<typename T>
T List<T>::last() {
    return this->_array[this->_size -1];
}

template<typename T>
uint8_t List<T>::size() {
    return this->_size;
}

template<typename T>
void List<T>::clear() {
    this->_size = 0;
}

template<typename T>
void List<T>::push(T t) {
    *this->_tail = t;

    if (this->_tail == &(this->_array[this->_arraySize-1])) {
        this->_tail = this->_array;
    }
    else {
        this->_tail++;
    }
    this->_size++;
    //this->_array[this->_size] = t;
}

template<typename T>
T List<T>::pop() {
    if (this->_tail == this->_array) {
        this->_tail = &(this->_array[this->_arraySize -1]);
    }
    else {
        this->_tail--;
    }
    this->_size--;
    return *this->_tail;
}
//T List<T>::shift() {
//}
//void List<T>::unshift(T t) {
//}


// 使うクラス
//template class List<BlockPiece>;
template void  List<BlockPiece>::push(BlockPiece);
template BlockPiece  List<BlockPiece>::pop();
template uint8_t  List<BlockPiece>::size();
template void List<BlockPiece>::clear();
