//
// Created by Melody on 2017/5/23 0023.
//

#include "BufferUnit.h"

bool BufferUnit::swapBlock(int fileindex, int blockindex){
    BufferDataBlock &block = blocks[blockindex];

    if(block.valid == 1 && block.edited == 1){
        fstream rp(filename);
        rp.seekp(block.index * blockSize);
        rp.write(block.data, blockSize);
        rp.close();
    }

    if(fileindex == -1){
        return true;
    }

    ifstream fp(filename, ifstream::binary);
    fp.seekg(blockSize * fileindex);
    fp.read(block.data, blockSize);
    fp.close();

    block.index = fileindex;
    block.flag = 1;
    block.valid = 1;
    block.edited = 0;
    block.lock = 0;

    return true;
}

int BufferUnit::getValidBlock(){ // get a block which doesn't locked
    while(true){
        clockIndex = (clockIndex + 1) % bufferSize;
        if(blocks[clockIndex].lock == 0){
            if(blocks[clockIndex].flag == 1){ // if a block has been visited recently, this flag will be changed to 0.
                blocks[clockIndex].flag = 0;
            }else{
                return clockIndex; // if this block has been edited, this block must be writen back.
            }
        }
    }
}

bool BufferUnit::readBlock(int index, char *readBuffer){
    if(index < 0 || index >= blockIndexInBuffer.size()){
        return false;
    }else{
        if(blockIndexInBuffer[index] != -1){
            int bufferIndex = blockIndexInBuffer[index];
            char *src = blocks[bufferIndex].data;
            memcpy(readBuffer, src, blockSize);
            blocks[bufferIndex].flag = 1;
        }else{
            int bufferIndex = getValidBlock();
            if(blocks[bufferIndex].valid == 1){
                blockIndexInBuffer[blocks[bufferIndex].index] = -1;
            }
            swapBlock(index, bufferIndex);
            char *src = blocks[bufferIndex].data;
            memcpy(readBuffer, src, blockSize);
        }
        return true;
    }
}

bool BufferUnit::writeBlock(int index, char *writeBuffer){
    if(index < 0 || index > blockIndexInBuffer.size()){
        return false;
    }else{
        if(index == blockIndexInBuffer.size()){  // write a block at the end of file.
            ofstream fp(filename, ios::binary | ios::app);
            fp.write(writeBuffer, blockSize);
            fp.close();

            int newIndex = getValidBlock();
            blockIndexInBuffer.push_back(newIndex);
            filesize += blockSize;
            swapBlock(index, newIndex);
        }else{ // write a block between the start block and the end block.
            if(blockIndexInBuffer[index] != -1){ // if the block is in the buffer, write to the data and change the flags.
                memcpy(blocks[blockIndexInBuffer[index]].data, writeBuffer, (size_t)blockSize);
                blocks[blockIndexInBuffer[index]].edited = 1;
                blocks[blockIndexInBuffer[index]].flag = 1;
            }else{
                int newIndex = getValidBlock();
                blockIndexInBuffer[index] = newIndex;
                swapBlock(index, newIndex);
                if(blocks[newIndex].valid){
                    blockIndexInBuffer[blocks[newIndex].index] = -1;
                }
                memcpy(blocks[newIndex].data, writeBuffer, (size_t)blockSize);
                blocks[newIndex].edited = 1;
                blocks[newIndex].flag = 1;
            }
        }
		return true;
    }
}

bool BufferUnit::lockBlock(int index, int status){
    if((blockIndexInBuffer[index] == -1 )|| (index < 0) || (index >= blockIndexInBuffer.size())){
        return false;
    }
    blocks[index].lock = status;
    return true;
}

int BufferUnit::upFloor(double size){
    if((double)(int)size == size){
        return (int)size;
    }
    return (int)size + 1;
}

bool BufferUnit::deleteLastBlock(){
    if(filesize < 0){
        return false;
    }

    filesize -= blockSize;
    int index = blockIndexInBuffer.back();

    blockIndexInBuffer.erase(blockIndexInBuffer.end() - 1);

    char *buf = new char[filesize];
    ifstream fp(filename, ios::binary);
    fp.read(buf, filesize);
    fp.close();

    ofstream op(filename, ios::binary);
    op.write(buf, filesize);
    op.close();
    delete[] buf;

    // the block is in the buffer, kick it out
    if (index != -1) {
        blocks[index].edited = false;
        blocks[index].index = -1;
        blocks[index].lock = false;
        blocks[index].flag = false;
        blocks[index].valid = false;
    }
	return true;
}

BufferUnit::BufferUnit(string filename, int blockSize, int bufferSize){
    this->filename = filename;
    this->blockSize = blockSize;
    this->bufferSize = bufferSize;
    this->success = 0;

    int size = (int)ifstream(filename, ifstream::ate | ifstream::binary).tellg();

    if(size == -1){
        cout << "Error, may be the file doesn't exist." << endl;
        return;
    }
    // align the filesize to the block size, if not, put the random data into the file.
    filesize = upFloor((size * 1.0 / blockSize) * blockSize);

    if(filesize != size){
        char *random = new char[filesize - size];
        ofstream out(filename, ios::binary | ios::app);
        out.write(random, filesize - size);
        delete []random;
        out.close();
    }

    blockIndexInBuffer = vector<int>(filesize / blockSize, -1);
    for (int i = 0; i < bufferSize; ++i) {
        //blocks.push_back(BufferDataBlock(blockSize));
		/*BufferDataBlock a(blockSize);
		blocks.push_back(a);*/
		//BufferDataBlock()
		blocks.emplace_back(blockSize);
    }

    clockIndex = bufferSize - 1;

    this->success = 1;
}

BufferUnit::~BufferUnit(){
    for (int i = 0; i < bufferSize; i++) {
        swapBlock(-1, i);
    }
}