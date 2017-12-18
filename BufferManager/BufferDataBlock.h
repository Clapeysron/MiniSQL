//
// Created by Melody on 2017/5/23 0023.
//

#ifndef SQL_BUFFERDATABLOCK_H
#define SQL_BUFFERDATABLOCK_H

#include <iostream>
#include <cstring>

using namespace std;

class BufferDataBlock {
public:
    int lock;
    int edited;
    int index;
    int size;
    int valid;
    int flag;
    char *data;
    BufferDataBlock(int size){
        lock = 0;
        edited = 0;
        this->size = size;
        index = -1;
        valid = 0;
        flag = 0;// Clock algorithm flag.
        data = new char[size];
    }

    BufferDataBlock(const BufferDataBlock &b){
        lock = 0;
        edited = 0;
        this->size = b.size;
        index = -1;
        valid = 0;
        flag = 0;
        data = new char[size];
        memcpy(data, b.data, (size_t)b.size);
    }

    ~BufferDataBlock(){
        delete[] data;
    }

};


#endif //SQL_BUFFERDATABLOCK_H
