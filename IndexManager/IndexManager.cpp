#include "TypeInfo.h"
#include "IndexManager.h"
#include "../BufferManager/BufferManager.h"
#include "../CatalogManager/CharStream.h"
#include "BPlusTree.h"



IndexManager::IndexManager(std::string fileName, BufferManager& bufferManager):_fileName(fileName) {
	std::vector<char*> allBlocks;
	bufferManager.readDatas(_fileName, allBlocks);
	for (size_t i = 0; i < allBlocks.size();) {
		//std::string name;
		CharInStream cis(allBlocks[i], bufferManager.getBlockSize());
		
		size_t blockNum;
		Type t;
		cis >> blockNum >> t;

		cis.resize(blockNum*bufferManager.getBlockSize());
		cis.reset();

		switch (t) {
		case Int:
			Index<int>&& index_temp = Index<int>::deserialize(cis);
			_intIndex.emplace(index_temp.getName(), index_temp);
		case Float:
			Index<float>&& index_temp = Index<float>::deserialize(cis);
			_floatIndex.emplace(index_temp.getName(), index_temp);
		case Chars:
			Index<float>&& index_temp = Index<float>::deserialize(cis);
			_stringIndex.emplace(index_temp.getName(), index_temp);
		default:
			throw SomeError("no such type");
		}
		
	}
}

void IndexManager::writeBack(BufferManager & bufferManager) {
	size_t block_index = 0;
	for (auto i = _intIndex.cbegin(); i != _intIndex.cend(); i++) {
		size_t blockNum = i->second.getblockNum();
		char* buff = new char[blockNum*bufferManager.getBlockSize()];
		CharOutStream couts(buff, bufferManager.getBlockSize());
		i->second.serialize(couts);
		for (size_t j = block_index; j < block_index+blockNum; j++) {
			bufferManager.writeDataToFile(_fileName, j, buff+(j-block_index)*BLOCKSIZE);
		}
		delete[] buff;
		block_index += blockNum;
	}
	
	for (auto i = _intIndex.cbegin(); i != _intIndex.cend(); i++) {
		size_t blockNum = i->second.getblockNum();
		char* buff = new char[blockNum*bufferManager.getBlockSize()];
		CharOutStream couts(buff, bufferManager.getBlockSize());
		i->second.serialize(couts);
		for (size_t j = block_index; j < block_index + blockNum; j++) {
			bufferManager.writeDataToFile(_fileName, j, buff + (j - block_index)*BLOCKSIZE);
		}
		delete[] buff;
		block_index += blockNum;
	}

	for (auto i = _intIndex.cbegin(); i != _intIndex.cend(); i++) {
		size_t blockNum = i->second.getblockNum();
		char* buff = new char[blockNum*bufferManager.getBlockSize()];
		CharOutStream couts(buff, bufferManager.getBlockSize());
		i->second.serialize(couts);
		for (size_t j = block_index; j < block_index + blockNum; j++) {
			bufferManager.writeDataToFile(_fileName, j, buff + (j - block_index)*BLOCKSIZE);
		}
		delete[] buff;
		block_index += blockNum;
	}
}


