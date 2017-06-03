#pragma once
#include "TypeInfo.h"
#include <vector>
#include <memory>
#include <map>
#include "BPlusTree.h"
#include "../BufferManager/BufferManager.h"

typedef size_t RecordPtr;

const static size_t BLOCKSIZE = 4096;

template <typename T>
class Index {
public:
	
	Index(std::string indexName, std::string fieldName, std::string tableName, BPlusTree* treePtr) :
		_indexName(indexName),
		_fieldName(fieldName),
		_tableName(tableName),
		_treePtr(treePtr)
	{
	}

	~Index() {
	}

	

	static Index deserialize(CharInStream& cis) {
		std::string indexName;
		std::string tableName;
		std::string fieldName;
		Type type;
		size_t blockNum;
		//size_t blockNum;
		cis >> blockNum >> type>> indexName >> tableName >> fieldName;
		return Index(indexName, fieldName, tableName, BPlusTree<T>::recoverTree(cis, size););
	}

	void serialize(CharOutStream& couts) const {
		size_t blockNum;
		//blockNum = (sizeof(Type) + _indexName.size() + _tableName.size() + _fieldName.size() + _treePtr->size())/blockSize+1;
		blockNum = getblockNum();
		couts << size << _type << _indexName <<  _tableName << _fieldName;
		BPlusTree<T>::serialize(couts);
	}

	std::string getName() const {
		return _indexName;
	}

	size_t getblockNum() const {
		return (sizeof(Type) + _indexName.size() + _tableName.size() + _fieldName.size() + _treePtr->size()) / BLOCKSIZE+1;
	}
private:
	std::string _indexName;
	std::string _fieldName;
	std::string _tableName;
	std::unique_ptr<BPlusTree<T>> _treePtr;
	Type _type;
};


class IndexManager {
public:
	IndexManager(std::string fileName, BufferManager& bufferManager);
	~IndexManager() = default;
	void writeBack(BufferManager& bufferManager);

	template<typename T>
	Index<T>& create_index(std::string& indexName, std::string& tableName, std::string& fieldName, TypeInfo& type) {
		switch (type) {
		default:
			break;
		}
	}

	//Index& create_index(std::string& indexName, std::string& tableName, std::string& fieldName, TypeInfo& Type);
	void drop_index(std::string index_name, std::string& tableName, std::string& fieldName);
	std::vector<RecordPtr> search(std::string& tableName);
	
	// TODO what kind of way to search is not determined. possible soltions: write all kind of search for three types and different operation(equal or range search)

private:
	std::map<std::string, Index<int>> _intIndex;
	std::map<std::string, Index<float>> _floatIndex;
	std::map<std::string, Index<std::string>> _stringIndex;
	std::string _fileName;
};


