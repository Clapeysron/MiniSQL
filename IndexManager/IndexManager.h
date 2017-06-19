#pragma once
#include "TypeInfo.h"
#include <vector>
#include <memory>
#include <map>
//#include "BPlusTree.h"
#include "lajiBPT.h"
#include "../BufferManager/BufferManager.h"
#include "../CatalogManager/CatalogManager.h"
#include <sstream>


template <typename T>
class Index {
public:
	using record_ptr = int;
	Index(std::string indexName, std::string fieldName, std::string tableName, BPlusTree<T> tree) :
		_indexName(indexName),
		_fieldName(fieldName),
		_tableName(tableName),
		_tree(tree) {
	}

	~Index() = default;
	static Index deserialize(CharInStream& cis) {
		std::string indexName;
		std::string tableName;
		std::string fieldName;
		Type type;
		//size_t blockNum;
		cis >> type >> indexName >> tableName >> fieldName;
		return Index(indexName, fieldName, tableName, BPlusTree<T>::deserialize(cis));
	}

	void serialize(CharOutStream& couts) const {
		couts << _type << _indexName << _tableName << _fieldName;
		_tree.serialize(couts);
	}

	std::string getName() const {
		return _indexName;
	}

	//std::vector<record_ptr> find_all() {
	//	return _tree.find_all();
	//}

	BPlusTree<T> get_tree() {
		return _tree;
	}

	


private:
	std::string _indexName;
	std::string _fieldName;
	std::string _tableName;
	BPlusTree<T> _tree;
	Type _type;
};


class IndexManager {
	template<typename T>
	friend class BPlusTree;
	template<typename T>
	friend class BPTNode;
	using record_ptr = int;
public:
	IndexManager(std::string fileName);
	~IndexManager() {
		//size_t block_index = 0;
		//size_t block_size = BM.getBlockSize();
		//for (auto i = _intIndex.cbegin(); i != _intIndex.cend(); i++) {
		//	//size_t blockNum = i->second.getblockNum();
		//	char* buff = new char[block_size];
		//	CharOutStream couts(buff, block_size);
		//	i->second.serialize(couts);
		//	if (cout.remain) {

		//	}
		//	for (size_t j = block_index; j < block_index + blockNum; j++) {
		//		BM.writeDataToFile(_fileName, j, buff + (j - block_index)*BLOCKSIZE);
		//	}
		//	delete[] buff;
		//	block_index += blockNum;
		//}

		//for (auto i = _intIndex.cbegin(); i != _intIndex.cend(); i++) {
		//	size_t blockNum = i->second.getblockNum();
		//	char* buff = new char[blockNum*bufferManager.getBlockSize()];
		//	CharOutStream couts(buff, bufferManager.getBlockSize());
		//	i->second.serialize(couts);
		//	for (size_t j = block_index; j < block_index + blockNum; j++) {
		//		bufferManager.writeDataToFile(_fileName, j, buff + (j - block_index)*BLOCKSIZE);
		//	}
		//	delete[] buff;
		//	block_index += blockNum;
		//}

		//for (auto i = _intIndex.cbegin(); i != _intIndex.cend(); i++) {
		//	size_t blockNum = i->second.getblockNum();
		//	char* buff = new char[blockNum*bufferManager.getBlockSize()];
		//	CharOutStream couts(buff, bufferManager.getBlockSize());
		//	i->second.serialize(couts);
		//	for (size_t j = block_index; j < block_index + blockNum; j++) {
		//		bufferManager.writeDataToFile(_fileName, j, buff + (j - block_index)*BLOCKSIZE);
		//	}
		//	delete[] buff;
		//	block_index += blockNum;
		//}
	}

	template<typename T>
	Index<T>& create_index(std::string& indexName, std::string& tableName, std::string& fieldName, TypeInfo& type) {
		switch (type) {
		default:
			break;
		}
	}
	//create_index(string table_name, string index_name, vector<string> col_list)



	//Index& create_index(std::string& indexName, std::string& tableName, std::string& fieldName, TypeInfo& Type);
	void drop_index(std::string index_name, std::string& tableName, std::string& fieldName);

	int  delete_all(std::string& tableName);
	// return how many records

	int delete_part(std::string& tableName, std::vector<record_ptr> indices);
	// delete index  entity according to record_ptrs
	

	//drop table

	std::vector<record_ptr> search_all(std::string& tableName) {
		// TODO: consider simply the method , just push_back -2 into the result
		std::string index_name_primary = CM.find_primary_index(tableName);
		if (_intIndex.find(index_name_primary) != _intIndex.end()) {
			return (*_intIndex.find(index_name_primary)).second.get_tree().find_all();
		} else if (_floatIndex.find(index_name_primary) != _floatIndex.end()) {
			return (*_floatIndex.find(index_name_primary)).second.get_tree().find_all();
		} else if (_stringIndex.find(index_name_primary) != _stringIndex.end()) {
			return (*_stringIndex.find(index_name_primary)).second.get_tree().find_all();
		} else {
			std::cerr << "can't find index";
			//return std::vector
			std::vector<record_ptr> result;
			result.push_back(-1);
			return result;
		}
	}


	template<typename T>
	std::vector<record_ptr> search_where(std::string& indexName, int compare_type, T key) {
		std::vector<record_ptr> result;
		T low, high;
		switch (compare_type) {
		case 1:
			high = key;
			result = search_range(indexName, nullptr, &high);
			result.pop_back();
			break;
		case 2:
			low = key;
			result = search_range(indexName, &low, nullptr);
			result.erase(result.begin());
			break;
		case 3:
			low = key;
			result = search_range(indexName, &low, nullptr);
			result.erase(result.begin());
			break;
		case 4:
			low = key;
			high = key;
			result = search_range(indexName, &low, &high);
			break;
		case 5:
			low = key;
			result = search_range(indexName, &low, nullptr);
			break;
		case 6:
			high = key;
			result = search_range(indexName, nullptr, &high);
			break;
		default:
			std::cerr << "wrong compare_type" << std::endl;
			break;
		}

		return result;
	}


	std::vector<record_ptr> search_range(std::string& indexName, int * low, int * high) {
		Index<int>& index = _intIndex[indexName];
		if (low == nullptr&&high == nullptr) {
			return index.get_tree().find_all();
		} else if (high == nullptr) {
			return index.get_tree().find_larger(*high);
		} else if (low == nullptr) {
			return index.get_tree().find_smaller(*high);
		}
		return index.get_tree().find_range(*low, *high);
	}

	std::vector<record_ptr> search_range(std::string& indexName, float* low, float* high) {
		Index<float>& index = _floatIndex[indexName];
		if (low == nullptr&&high == nullptr) {
			//return index.find_all();
			return index.get_tree().find_all();
		} else if (high == nullptr) {
			return index.get_tree().find_larger(*high);
		} else if (low == nullptr) {
			return index.get_tree().find_smaller(*high);
		}
		return index.get_tree().find_range(*low, *high);
	}

	std::vector<record_ptr> search_range(std::string& indexName, std::string* low, std::string* high) {
		Index<std::string>& index = _stringIndex[indexName];
		if (low == nullptr&&high == nullptr) {
			return index.get_tree().find_all();
		} else if (high == nullptr) {
			return index.get_tree().find_larger(*high);
		} else if (low == nullptr) {
			return index.get_tree().find_smaller(*high);
		}
		return index.get_tree().find_range(*low, *high);
	}



	// TODO what kind of way to search is not determined. possible soltions: write all kind of search for three types and different operation(equal or range search)

private:
	std::map<std::string, Index<int>> _intIndex;
	std::map<std::string, Index<float>> _floatIndex;
	std::map<std::string, Index<std::string>> _stringIndex;
	const std::string _fileName;
	static BufferManager& BM;
	static CatalogManager& CM;
};

