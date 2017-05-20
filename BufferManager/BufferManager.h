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
	bool createFile(string filename); // Create a file.
	bool deleteFile(string filename); // Delete a file.
	bool readDataFromFile(string filename, int blockIndex, char *readBuffer); // Read the specific blockIndex data in the file to the memory.
	bool writeDataToFile(string filename, int blockIndex, char *writeBuffer); // Write the data to the specific blockIndex to the file from the memory.
	bool lockBlock(string filename, int blockIndex); // Lock or Unlock the Block in the specific file.
	BufferManager(int blockSize, int bufferSize); // Constructer
	~BufferManager(); // Destructor
}

#endif