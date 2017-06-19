//
// Created by Melody on 2017/5/23 0023.
//

#include "BufferManager.h"

int BufferManager::getIndexByFileName(string filename){
    for(int i = 0; i < files.size(); ++i){
        if(filename == files[i]){
            return i;
        }
    }
    return -1;
}

ifstream::pos_type BufferManager::getFileSize(string filename){
    return ifstream(filename, ifstream::ate | ifstream::binary).tellg();
}

bool BufferManager::buildBuffer(string filename) {
    int bufferIndex = getIndexByFileName(filename);
    if(bufferIndex != -1){
        return true;
    }else{
        BufferUnit *b = new BufferUnit(filename, blockSize, bufferSize);
        buffers.push_back(*b);
        files.push_back(filename);
        return true;
    }
    return false;
}

bool BufferManager::destroyBuffer(string filename){
    int bufferIndex = getIndexByFileName(filename);
    if(bufferIndex == -1){
        return false;
    }else{
        vector<BufferUnit>::iterator ite_a = buffers.begin();
        vector<string>::iterator ite_b = files.begin();
        while(bufferIndex--){
            ite_a++;
            ite_b++;
        }
        buffers.erase(ite_a);
        files.erase(ite_b);
        return true;
    }
}

bool BufferManager::createFile(string filename){
    destroyBuffer(filename);
    if((int)getFileSize(filename) != -1){
        cout << "Fatal error, the table you want to create has already exist." << endl;
        return false;
    }
    ofstream fp(filename, ios::binary);
    fp.close();
    buildBuffer(filename);
    return true;
}

bool BufferManager::deleteFile(string filename){
    destroyBuffer(filename);
    if((int)getFileSize(filename) == -1){
        cout << "Fatal error, the table you want to delete doesn't exist." << endl;
        return false;
    }else{
        remove(filename.c_str());
        return true;
    }
}

bool BufferManager::readDataFromFile(string filename, int blockIndex, char *readBuffer){
    int bufferIndex = getIndexByFileName(filename);
    if(bufferIndex == -1){
        buildBuffer(filename);
        bufferIndex = (int)buffers.size() - 1;
    }
    return buffers[bufferIndex].readBlock(blockIndex, readBuffer);
}

bool BufferManager::readDatas(string filename, vector<char *> results){
    int bufferIndex = getIndexByFileName(filename);
    if(bufferIndex == -1){
        buildBuffer(filename);
        bufferIndex = (int)buffers.size() - 1;
    }
    for(int i = 0; i < (int)(getFileSize(filename) * 1.0 / blockSize + 0.5); ++i){
        char *block = new char[blockSize];
        buffers[bufferIndex].readBlock(i, block);
        results.push_back(block);
    }
	return true;
	//TODO: to be fixed to return a proper bool value
}

bool BufferManager::writeDataToFile(string filename, int blockIndex, char *writeBuffer){
    int bufferIndex = getIndexByFileName(filename);
    if(bufferIndex == -1){
        buildBuffer(filename);
        bufferIndex = (int)buffers.size() - 1;
    }
    return buffers[bufferIndex].writeBlock(blockIndex, writeBuffer);
}

bool BufferManager::deleteLastBlockOfFile(string filename){
    int bufferIndex = getIndexByFileName(filename);
    if(bufferIndex == -1){
        buildBuffer(filename);
        bufferIndex = (int)buffers.size() - 1;
    }
    buffers[bufferIndex].deleteLastBlock();
    return true;
}

bool BufferManager::lockBlock(string filename, int blockIndex, int lock){
    int bufferIndex = getIndexByFileName(filename);
    if(bufferIndex == -1){
        buildBuffer(filename);
        bufferIndex = (int)buffers.size() - 1;
    }
    return buffers[bufferIndex].lockBlock(blockIndex, lock);
}

BufferManager::BufferManager(int blockSize = 4096, int bufferSize = 256) : blockSize(blockSize), bufferSize(bufferSize) {
}

BufferManager::~BufferManager() {
}