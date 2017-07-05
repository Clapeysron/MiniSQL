#pragma once
#include "TypeInfo.h"
#include <vector>
#include <memory>
#include <map>
#include <utility>
#include "BPlusTree.h"
#include "../BufferManager/BufferManager.h"
#include "../CatalogManager/CatalogManager.h"
#include <sstream>
#include <algorithm>

const size_t name_length_max = 20;

template <typename T>
class Index {
public:
	using record_ptr = int;

	Index(std::string indexName, std::string fieldName, std::string tableName, BPlusTree<T>& tree) :
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


	void insert(const T& key, const record_ptr& record) {
		//for (size_t i = 0; i < keys.size(); i++) {
		//	_tree.insert_entity(keys[i], records[i]);
		//}
		_tree.insert_entity(key, record);
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

private:
	std::string _indexName;			// size = 20
	std::string _fieldName;			//size = 20
	std::string _tableName;			//size = 20
	BPlusTree<T> _tree;            //size = 20
	Type _type;						//size = 4;

	static const size_t IndexSize = 3 * name_length_max + 2 + sizeof(Type);
};


class IndexManager {
	template<typename T>
	friend class BPlusTree;

	template<typename T>
	friend class BPTNode;

	using record_ptr = int;
public:

	static IndexManager& Instance() {
		static IndexManager theSingleton("Index");
		return theSingleton;
	}


	void create_index(const std::string& indexName, const std::string& tableName, const std::string& fieldName, const std::vector<int>& keys) {
		TypeInfo temp_type = CatalogManager::Instance().get_type(tableName, fieldName);
		std::vector<std::pair<int, int>> entities;
		entities.reserve(keys.size());
		int i = 0;
		for_each(keys.begin(), keys.end(),
			[&i, &entities](int key) {entities.push_back(std::make_pair(key, i++));
		});
		std::sort(entities.begin(), entities.end(),
			[](auto &left, auto &right) {
			return left.first < right.first;
		});
		_intIndex.emplace(
			indexName,
			Index<int>(indexName,
				fieldName,
				tableName,
				BPlusTree<int>::create(entities, 4)));
	}

	void create_index(const std::string& indexName, const std::string& tableName, const std::string& fieldName, const std::vector<float>& keys) {
		TypeInfo temp_type = CatalogManager::Instance().get_type(tableName, fieldName);
		std::vector<std::pair<float, int>> entities;
		entities.reserve(keys.size());
		int i = 0;
		for_each(keys.begin(), keys.end(),
			[&i, &entities](float key) {entities.push_back(std::make_pair(key, i++));
		});
		std::sort(entities.begin(), entities.end(),
			[](auto &left, auto &right) {
			return left.first < right.first;
		});
		_floatIndex.emplace(
			indexName,
			Index<float>(indexName,
				fieldName,
				tableName,
				BPlusTree<float>::create(entities, 4)));
	}

	void create_index(const std::string& indexName, const std::string& tableName, const std::string& fieldName, const std::vector<std::string>& keys) {
		TypeInfo temp_type = CatalogManager::Instance().get_type(tableName, fieldName);
		std::vector<std::pair<std::string, int>> entities;
		entities.reserve(keys.size());
		int i = 0;
		for_each(keys.begin(), keys.end(),
			[&i, &entities](std::string key) {entities.push_back(std::make_pair(key, i++));
		});
		std::sort(entities.begin(), entities.end(),
			[](auto &left, auto &right) {
			return left.first < right.first;
		});
		_stringIndex.emplace(
			indexName,
			Index<std::string>(indexName,
				fieldName,
				tableName,
				BPlusTree<std::string>::create(entities, 4)));
	}

	//template<typename T>
	//void create_index(const std::string& indexName, const std::string& tableName, const std::string& fieldName, const std::vector<T>& keys) {
	//	TypeInfo temp_type = CatalogManager::Instance().get_type(tableName, fieldName);
	//	std::vector<std::pair<T, int>> entities;
	//	entities.reserve(keys.size());
	//	int i = 0;
	//	for_each(keys.begin(), keys.end(),
	//		[&i, &entities](T key) {entities.push_back(std::make_pair(key, i++));
	//	});
	//	std::sort(entities.begin(), entities.end(),
	//		[](auto &left, auto &right) {
	//		return left.first < right.first;
	//	});
	//	switch (temp_type.get_type()) {
	//	case Int:
	//		_intIndex.emplace(
	//			indexName,
	//			Index<int>(indexName,
	//				fieldName,
	//				tableName,
	//				BPlusTree<int>::create(entities, 4)));
	//		break;
	//	case Float:
	//		_intIndex.emplace(
	//			indexName,
	//			Index<float>(indexName,
	//				fieldName,
	//				tableName,
	//				BPlusTree<float>::create(entities, 4)));
	//		break;
	//	case Chars:
	//		_intIndex.emplace(
	//			indexName,
	//			Index<std::string>(indexName,
	//				fieldName,
	//				tableName,
	//				BPlusTree<std::string>::create(entities, temp_type.get_size())));
	//		break;
	//	}
	//}

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
		const std::vector<std::pair<Type, std::string>>& indices = CatalogManager::Instance().get_indices(tableName);
		for (auto i = indices.cbegin(); i != indices.cend(); i++) {
			drop_index((*i).first, (*i).second);
		}
	}

	void drop_table(std::string& tableName) {
		std::vector<std::pair<Type, std::string>> index_names = CatalogManager::Instance().find_indices(tableName);
		for (auto i = index_names.begin(); i != index_names.end(); i++) {
			switch ((*i).first) {
			case Int:
				(*_floatIndex.find((*i).second)).second.clear();
				_floatIndex.erase(i->second);
				break;
			case Float:
				(*_floatIndex.find((*i).second)).second.clear();
				_floatIndex.erase(i->second);
				break;
			case Chars:
				(*_stringIndex.find((*i).second)).second.clear();
				_floatIndex.erase(i->second);
				break;
			default:
				std::cout << "no such type" << std::endl;
			}
		}
	}

	int  delete_all(std::string& tableName) {
		std::vector<std::pair<Type, std::string>> index_names = CatalogManager::Instance().find_indices(tableName);
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
		std::vector<std::pair<Type, std::string>> index_names = CatalogManager::Instance().find_indices(tableName);
		std::vector<record_ptr> real_ptrs = get_real_ptrs(tableName);
		for (size_t i = 0; i < indices.size(); i++) {
			real_ptrs[indices[i]] = -1;
		}
		return indices.size();
	}
	// delete index  entity according to record_ptrs

	void insert_real_index(std::string& table_name) {
		_real_record_ptrs.at(table_name).push_back(1);
	}

	void clear_record(std::string table_name) {
		_real_record_ptrs.at(table_name).clear();
	}


	void insert(const string& table_name, const string& fieldName, int key) {


		/*_real_record_ptrs.at(table_name).push_back(1);*/
		/*std::pair<Type, std::string> indexName_type = CatalogManager::Instance().find_index(table_name, fieldName);
		if (indexName_type.first == Int) {
			_intIndex.at(indexName_type.second).insert(key, get_record_size(table_name));

		} else {
			std::cout << "IndexManager: insert: type doesn't match";

		}*/

	}

	void insert(const string& table_name, const string& fieldName, float key) {
		//_real_record_ptrs.at(table_name).push_back(1);
		std::pair<Type, std::string> indexName_type = CatalogManager::Instance().find_index(table_name, fieldName);
		if (indexName_type.first == Int) {
			_floatIndex.at(indexName_type.second).insert(key, get_record_size(table_name));

		} else {
			std::cout << "IndexManager: insert: type doesn't match";
		}

	}

	void insert(const string& table_name, const string& fieldName, std::string key) {
		//_real_record_ptrs.at(table_name).push_back(1);
		std::pair<Type, std::string> indexName_type = CatalogManager::Instance().find_index(table_name, fieldName);
		if (indexName_type.first == Int) {
			_stringIndex.at(indexName_type.second).insert(key, get_record_size(table_name));

		} else {
			std::cout << "IndexManager: insert: type doesn't match";

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
		Index<int>& index = _intIndex.at(indexName);
		if (low == nullptr&&high == nullptr) {
			
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
			
			std::vector<record_ptr> result(1, -1);
			return result;
		} else if (high == nullptr) {
			return convert_to_real_ptrs(index.get_table_name(), index.get_tree().find_larger(*high));
		} else if (low == nullptr) {
			return convert_to_real_ptrs(index.get_table_name(), index.get_tree().find_smaller(*high));
		}
		return convert_to_real_ptrs(index.get_table_name(), index.get_tree().find_range(*low, *high));
	}


	void init(std::string& tableName) {
		//TODO : add a primary index 

		_real_record_ptrs[tableName] = std::vector<int>();
	}

	//will delete in the future
	int get_record_size(std::string tableName) {
		return _real_record_ptrs.at(tableName).size();
	}

	int get_real_record_amount(std::string tableName) {
		int result = 0;
		std::vector<int> record_ptrs = _real_record_ptrs.at(tableName);
		for (size_t i = 0; i < record_ptrs.size(); i++) {
			if (record_ptrs[i] != -1) {
				result++;
			}
		}
		return result;
	}




private:
	std::map<std::string, std::vector<int>> _real_record_ptrs;
	std::map<std::string, Index<int>> _intIndex;
	std::map<std::string, Index<float>> _floatIndex;
	std::map<std::string, Index<std::string>> _stringIndex;
	const std::string _fileName;

	//static CatalogManager& CatalogManager::Instance();
	//static const std::string bplustree_filename ;


	std::vector<record_ptr> convert_to_real_ptrs(const std::string& table_name, std::vector<record_ptr>& fake_ptrs) {
		std::vector<record_ptr> result;
		std::vector<record_ptr> real_records = _real_record_ptrs[table_name];
		for (size_t i = 0; i < fake_ptrs.size(); i++) {
			if (real_records[fake_ptrs[i]] != -1) {
				result.push_back(fake_ptrs[i]);
			}

		}
		return result;
	}

	std::vector<record_ptr> get_real_ptrs(const std::string& table_name) {
		return _real_record_ptrs[table_name];
	}

	IndexManager(std::string fileName) :
		_fileName(fileName) {
		if (BufferManager::Instance().createFile(fileName)) {

		} else {
			size_t indexSize = 3 * name_length_max + 20 + sizeof(Type);

			std::vector<char*> allBlocks;
			BufferManager::Instance().readDatas(_fileName, allBlocks);
			size_t int_index_num;
			size_t float_index_num;
			size_t string_index_num;
			size_t block_count = 0;
			size_t block_size = BufferManager::Instance().getBlockSize();
			if (block_count < allBlocks.size()) {
				CharInStream cis(allBlocks[block_count], block_size);
				cis >> int_index_num >> float_index_num >> string_index_num;
				//while (block_count+1 < (allBlocks.size())) {


				for (size_t i = 0; i < int_index_num; i++) {
					if (cis.remain() > indexSize) {
						Index<int> temp = Index<int>::deserialize(cis);
						_intIndex.emplace(temp.getName(), temp);
					} else {
						block_count++;
						cis = CharInStream(allBlocks[block_count], block_size);
						Index<int> temp = Index<int>::deserialize(cis);
						_intIndex.emplace(temp.getName(), temp);
					}
				}

				for (size_t i = 0; i < float_index_num; i++) {
					if (cis.remain() > indexSize) {
						Index<float> temp = Index<float>::deserialize(cis);
						_floatIndex.emplace(temp.getName(), temp);
					} else {
						block_count++;
						cis = CharInStream(allBlocks[block_count], block_size);
						Index<float> temp = Index<float>::deserialize(cis);
						_floatIndex.emplace(temp.getName(), temp);
					}
				}

				for (size_t i = 0; i < string_index_num; i++) {
					if (cis.remain() > indexSize) {
						Index<std::string> temp = Index<std::string>::deserialize(cis);
						_stringIndex.emplace(temp.getName(), temp);
					} else {
						block_count++;
						cis = CharInStream(allBlocks[block_count], block_size);
						Index<std::string> temp = Index<std::string>::deserialize(cis);
						_stringIndex.emplace(temp.getName(), temp);
					}
				}

				block_count++;
				cis = CharInStream(allBlocks[block_count], block_size);
				char* a = allBlocks[0];
				char* b = allBlocks[1];
				size_t real_records_num;
				cis >> real_records_num;

				for (size_t i = 0; i < real_records_num; i++) {
					size_t record_amount;
					std::string table_name;

					if (cis.remain() > sizeof(record_amount) + sizeof(table_name)) {
						cis >> table_name >> record_amount;
					} else {
						block_count++;
						cis = CharInStream(allBlocks[block_count], block_size);
						cis >> table_name >> record_amount;
					}

					std::vector<record_ptr> real_records;
					for (size_t i = 0; i < record_amount; i++) {
						if (cis.remain() > sizeof(record_ptr)) {
							record_ptr temp;
							cis >> temp;
							real_records.push_back(temp);
						} else {
							block_count++;
							cis = CharInStream(allBlocks[block_count], block_size);
							record_ptr temp;
							cis >> temp;
							real_records.push_back(temp);
						}

					}
					_real_record_ptrs.emplace(table_name, real_records);
					/*	block_count++;
					cis = CharInStream(allBlocks[block_count], block_size);*/
				}

				for (size_t i = 0; i < allBlocks.size(); i++) {
					delete[] allBlocks[i];
				}
				//}

			}

		}
	}


	~IndexManager() {
		size_t indexSize = 3 * name_length_max + 20 + sizeof(Type);

		size_t block_index = 0;
		size_t block_size = BufferManager::Instance().getBlockSize();

		size_t index_num_per_block = block_size / indexSize;
		char* buff = new char[block_size];
		CharOutStream couts(buff, block_size);
		couts << _intIndex.size() << _floatIndex.size() << _stringIndex.size();
		for (auto i = _intIndex.cbegin(); i != _intIndex.cend(); i++) {
			if (couts.remain() > indexSize) {
				i->second.serialize(couts);
			} else {
				BufferManager::Instance().writeDataToFile(_fileName, block_index, buff);

				block_index++;
				couts.reset();
				i->second.serialize(couts);
			}
		}



		for (auto i = _floatIndex.cbegin(); i != _floatIndex.cend(); i++) {
			if (couts.remain() > indexSize) {
				i->second.serialize(couts);
			} else {
				BufferManager::Instance().writeDataToFile(_fileName, block_index, buff);

				block_index++;
				couts.reset();
				i->second.serialize(couts);
			}
		}

		for (auto i = _stringIndex.cbegin(); i != _stringIndex.cend(); i++) {
			if (couts.remain() > indexSize) {
				i->second.serialize(couts);
			} else {
				BufferManager::Instance().writeDataToFile(_fileName, block_index, buff);

				block_index++;
				couts.reset();
				i->second.serialize(couts);
			}
		}

		BufferManager::Instance().writeDataToFile(_fileName, block_index, buff);
		couts.reset();
		block_index++;

		couts << _real_record_ptrs.size();
		for (auto i = _real_record_ptrs.cbegin(); i != _real_record_ptrs.cend(); i++) {

			if (couts.remain() > sizeof(i->first) + sizeof(i->second.size())) {
				couts << i->first << i->second.size();
			} else {
				BufferManager::Instance().writeDataToFile(_fileName, block_index, buff);
				couts.reset();
				block_index++;
				couts << i->first << i->second.size();
			}

			for (auto j = i->second.cbegin(); j != i->second.cend(); j++) {
				if (couts.remain() > sizeof(*j)) {
					couts << *j;
				} else {
					BufferManager::Instance().writeDataToFile(_fileName, block_index, buff);
					couts.reset();
					block_index++;
					couts << *j;
				}
			}
			/*	couts.reset();
				block_index++;*/
		}

		BufferManager::Instance().writeDataToFile(_fileName, block_index, buff);
		delete[] buff;
	}


};

