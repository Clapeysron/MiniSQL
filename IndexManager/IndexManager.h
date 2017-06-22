#pragma once
#include "TypeInfo.h"
#include <vector>
#include <memory>
#include <map>
#include <utility>
//#include "BPlusTree.h"
#include "lajiBPT.h"
#include "../BufferManager/BufferManager.h"
#include "../CatalogManager/CatalogManager.h"
#include <sstream>
#include <algorithm>


template <typename T>
class Index {
public:
	using record_ptr = int;
	Index() = delete;
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

	std::string get_table_name() const {
		return _tableName;
	}



	BPlusTree<T> get_tree() {
		return _tree;
	}

	void drop_tree() {
		_tree.drop();
	}

	void clear() {
		_tree.clear();
	}
	// for delete all;


	void remove_records(const std::vector<record_ptr>& indices) {
		_tree.remove_records(indices);
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
		//TODO: implement ~IndexManager
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
	void create_index(const std::string& indexName, const std::string& tableName, const std::string& fieldName, const std::vector<T> keys) {
		TypeInfo temp_type = CM.get_type(tableName, fieldName);
		std::vector<std::pair<T, int>> entities;
		entities.reserve(keys.size());
		int i = 0;
		for_each(keys.begin(), keys.end(),
			[&i, &entities](T key) {entities.push_back(std::make_pair(key, i++));
		});
		std::sort(entities.begin(), entities.end(),
			[](auto &left, auto &right) {
			return left.first < right.first;
		});

		switch (temp_type.get_type()) {
		case Int:
			_intIndex.emplace(
				indexName,
				Index<int>(indexName,
					fieldName,
					tableName,
					BPlusTree<int>::create_tree(entities, 4)));
			break;
		case Float:
			_intIndex.emplace(
				indexName,
				Index<float>(indexName,
					fieldName,
					tableName,
					BPlusTree<float>::create_tree(entities, 4)));
			break;
		case Chars:
			_intIndex.emplace(
				indexName,
				Index<std::string>(indexName,
					fieldName,
					tableName,
					BPlusTree<std::string>::create_tree(entities, temp_type.get_size())));
			break;
		}
	}
	//create_index(string table_name, string index_name, vector<string> col_list)



	//Index& create_index(std::string& indexName, std::string& tableName, std::string& fieldName, TypeInfo& Type);
	void drop_index(const Type& type, const std::string& index_name) {
		switch (type) {
		case Int:
			_intIndex.at(index_name).drop_tree();
			_intIndex.erase(index_name);
			break;
		case Float:
			_floatIndex.at(index_name).drop_tree();
			_floatIndex.erase(index_name);
			break;
		case Chars:
			_stringIndex.at(index_name).drop_tree();
			_stringIndex.erase(index_name);
			break;
		}
	}

	void drop_table_indices(const std::string& tableName) {
		const std::vector<std::pair<Type, std::string>>& indices = CM.get_indices(tableName);
		for (auto i = indices.cbegin(); i != indices.cend(); i++) {
			drop_index((*i).first, (*i).second);
		}
	}

	int  delete_all(std::string& tableName) {
		std::vector<std::pair<Type, std::string>> index_names = CM.find_indices(tableName);
		for (auto i = index_names.begin(); i != index_names.end(); i++) {
			switch ((*i).first) {
			case Int:
				(*_floatIndex.find((*i).second)).second.clear();
				break;
			case Float:
				(*_floatIndex.find((*i).second)).second.clear();
				break;
			case Chars:
				(*_stringIndex.find((*i).second)).second.clear();
				break;
			default:
				std::cerr << "no such type" << std::endl;
				return -1;
			}
		}

		return 0;
		// TODO: consider return nothing . If not, then I must traverse all the record for check the unvalid records
	}
	// return how many records

	int delete_part(std::string& tableName, const std::vector<record_ptr>& indices) {
		std::vector<std::pair<Type, std::string>> index_names = CM.find_indices(tableName);
		std::vector<record_ptr> real_ptrs = get_real_ptrs(tableName);
		for (size_t i = 0; i < indices.size(); i++) {
			real_ptrs[indices[i]] = -1;
		}
		return indices.size();
	}
	// delete index  entity according to record_ptrs



	int insert(string table_name, std::vector<int> type_list, std::vector<string> value_list) {
		//TODO: I just want keys and ptr_type. I think this should be down after record manager
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
		Index<int>& index = _intIndex.at(indexName);
		if (low == nullptr&&high == nullptr) {
			//TODO: consider a porper way
			/*return convert_to_real_ptrs(index.get_table_name(), index.get_tree().find_all());*/
			std::vector<record_ptr> result(1, -1);
			return result;
		} else if (high == nullptr) {
			return convert_to_real_ptrs(index.get_table_name(), index.get_tree().find_larger(*high));
		} else if (low == nullptr) {
			return convert_to_real_ptrs(index.get_table_name(), index.get_tree().find_smaller(*high));
		}
		return convert_to_real_ptrs(index.get_table_name(), index.get_tree().find_range(*low, *high));
	}

	std::vector<record_ptr> search_range(std::string& indexName, float* low, float* high) {
		Index<float>& index = _floatIndex.at(indexName);
		if (low == nullptr&&high == nullptr) {
			//TODO: consider a porper way
			/*return convert_to_real_ptrs(index.get_table_name(), index.get_tree().find_all());*/
			std::vector<record_ptr> result(1, -1);
			return result;
		} else if (high == nullptr) {
			return convert_to_real_ptrs(index.get_table_name(), index.get_tree().find_larger(*high));
		} else if (low == nullptr) {
			return convert_to_real_ptrs(index.get_table_name(), index.get_tree().find_smaller(*high));
		}
		return convert_to_real_ptrs(index.get_table_name(), index.get_tree().find_range(*low, *high));
	}

	std::vector<record_ptr> search_range(std::string& indexName, std::string* low, std::string* high) {
		Index<std::string>& index = _stringIndex.at(indexName);
		if (low == nullptr&&high == nullptr) {
			//TODO: consider a porper way
			/*return convert_to_real_ptrs(index.get_table_name(), index.get_tree().find_all());*/
			std::vector<record_ptr> result(1, -1);
			return result;
		} else if (high == nullptr) {
			return convert_to_real_ptrs(index.get_table_name(), index.get_tree().find_larger(*high));
		} else if (low == nullptr) {
			return convert_to_real_ptrs(index.get_table_name(), index.get_tree().find_smaller(*high));
		}
		return convert_to_real_ptrs(index.get_table_name(), index.get_tree().find_range(*low, *high));
	}




private:
	std::map<std::string, std::vector<int>> _real_record_ptrs;
	std::map<std::string, Index<int>> _intIndex;
	std::map<std::string, Index<float>> _floatIndex;
	std::map<std::string, Index<std::string>> _stringIndex;
	const std::string _fileName;
	static BufferManager& BM;
	static CatalogManager& CM;


	std::vector<record_ptr> convert_to_real_ptrs(const std::string& table_name, std::vector<record_ptr>& fake_ptrs) {
		std::vector<record_ptr> result;
		std::vector<record_ptr> real_records = _real_record_ptrs[table_name];
		for (size_t i = 0; i < fake_ptrs.size(); i++) {
			result.push_back(real_records[fake_ptrs[i]]);
		}
		return result;
	}

	std::vector<record_ptr> get_real_ptrs(const std::string& table_name) {
		return _real_record_ptrs[table_name];
	}
};

