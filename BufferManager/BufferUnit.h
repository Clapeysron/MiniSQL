//
// Created by Melody on 2017/5/23 0023.
//

#ifndef SQL_BUFFERUNIT_H
#define SQL_BUFFERUNIT_H
#include <fstream>
#include <string>
#include <vector>
#include "BufferDataBlock.h"
using namespace std;

class BufferUnit {
private:
    int bufferSize = 256;
    int blockSize = 4096;

    string filename;
    int filesize;
    vector<int> blockIndexInBuffer;
    vector<BufferDataBlock> blocks;

    int clockIndex = 0;
    int success = 0;

    bool swapBlock(int fileindex, int blockindex);
    int getValidBlock();
    int upFloor(double size);

public:
    bool readBlock(int index, char *readBuffer);
    bool writeBlock(int index, char*writeBuffer);
    bool lockBlock(int index, int status);
    bool deleteLastBlock();

    BufferUnit(string filename, int blockSize, int bufferSize);
    ~BufferUnit();

};


#endif //SQL_BUFFERUNIT_H
