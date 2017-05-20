#ifndef _BUFFERMANAGER_H_
#define _BUFFERMANAGER_H_

#include <vector>
#include <string>
#include <cstdio>
#include <BufferUnit.h>

using namespace std

class BufferManager{
private:
	vector<BufferUnit> buffers;
	vector<string> files;
	int BufferSize;
	int BlockSize;

	int getIndexByFileName(string filename); // Get the index of buffer in the buffers vector by filename.
	ifstream::pos_type getFileSize(string filename); // Get the size of one file.
	bool buildBuffer(string filename); // Build a buffer on a file.
	bool destroyBuffer(string filename); // Destroy the specific file's buffer.

public:
	bool createFile(string filename);
	bool deleteFile(string filename);
	bool readDataFromFile(string filename, int blockIndex, char *readBuffer);
	bool writeDataToFile(string filename, int blockIndex, char *writeBuffer);
	bool lockBlock(string filename, int blockIndex);
	BufferManager(int blockSize, int bufferSize);
	~BufferManager();
}

#endif