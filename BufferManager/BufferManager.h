//
// Created by Melody on 2017/5/23 0023.
//

#ifndef _BUFFERMANAGER_H_
#define _BUFFERMANAGER_H_

#include <vector>
#include <string>
#include <cstdio>
#include "BufferUnit.h"

using namespace std;

class BufferManager{
private:
    vector<BufferUnit> buffers;
    vector<string> files;
    int bufferSize;
    int blockSize;

    int getIndexByFileName(string filename); // Get the index of buffer in the buffers vector by filename.
    ifstream::pos_type getFileSize(string filename); // Get the size of one file.
    bool buildBuffer(string filename); // Build a buffer on a file.
    bool destroyBuffer(string filename); // Destroy the specific file's buffer.

public:
    bool createFile(string filename); // Create a file.
    bool deleteFile(string filename); // Delete a file.
    bool readDataFromFile(string filename, int blockIndex, char *readBuffer); // Read the specific blockIndex data in the file to the memory.
    bool readDatas(string filename, vector<char *> results);
    bool writeDataToFile(string filename, int blockIndex, char *writeBuffer); // Write the data to the specific blockIndex to the file from the memory.
    bool lockBlock(string filename, int blockIndex, int lock); // lockBlockck or Unlock the Block in the specific file.
    bool deleteLastBlockOfFile(string filename);
    BufferManager(int blockSize , int bufferSize); // Constructor
    ~BufferManager(); // Destructor

	int getNewBlockNum(string filename);
	//some api wanted by dfz
	int getBlockSize() {
		return blockSize;
	}

	bool writeDataToMultiBlock(string filename, int blockIndexBegin, int blockIndexEnd, char *writeBuffer);
	
};

#endif
