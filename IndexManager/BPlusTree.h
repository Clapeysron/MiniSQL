#pragma once
#include <utility>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <cassert>
#include <iostream>
#include "IndexManager.h"
#include "../BufferManager/BufferManager.h"


const std::string _fileName = "BPlusTree";

template<typename T>
class BPTNode {
	using record_ptr = int;   // RecordPtr is a int
	using block_ptr = int; // the block number in the file
	using key_type = T;

private:
	size_t _degree;
	bool _is_root;
	block_ptr _father;
	bool _changed;

public:
	bool _isLeaf;
	std::vector<block_ptr> _children;
	std::vector<key_type> _keys;
	std::vector<record_ptr> _records;
	block_ptr _next;
	block_ptr _blockNumber;
	BPTNode(std::vector<key_type> keys, size_t degree, block_ptr father, block_ptr blockNumber, block_ptr next, bool isLeaf, std::vector<int> pointers) :
		_keys(keys),
		_degree(degree),
		_father(father),
		_is_root(false),
		_blockNumber(blockNumber),
		_changed(true),
		_isLeaf(isLeaf),
		_next(next) {
		if (_isLeaf) {
			_children = pointers;
		} else {
			_records = pointers;
		}
	}

	BPTNode(std::vector<key_type> keys, block_ptr father, size_t degree, block_ptr blockNumber, bool isLeaf) :
		_keys(keys),
		_is_root(false),
		_degree(degree),
		_father(father),
		_changed(true),
		_blockNumber(blockNumber),
		_isLeaf(isLeaf) {
	}

	BPTNode(size_t degree, block_ptr blockNumber) :
		_is_root(false),
		_degree(degree),
		_father(blockNumber),
		_blockNumber(blockNumber),
		_changed(true),
		_isLeaf(false) {
	}

	BPTNode(block_ptr blockNum) :
		_blockNumber(blockNum),
		_changed(false) {
		int blockSize = BufferManager::Instance().getBlockSize();
		char* block = new char[blockSize];
		BufferManager::Instance().readDataFromFile(_fileName, blockNum, block);
		CharInStream cis(block, blockSize);
		cis >> _degree >> _is_root >> _isLeaf >> _father;
		size_t keys_num;
		cis >> keys_num;
		for (size_t i = 0; i < keys_num; i++) {
			key_type temp;
			cis >> temp;
			_keys.push_back(temp);
		}
		if (_isLeaf) {
			for (size_t i = 0; i < keys_num; i++) {
				record_ptr temp;
				cis >> temp;
				_records.push_back(temp);
			}
		} else {
			for (size_t i = 0; i < keys_num; i++) {
				block_ptr temp;
				cis >> temp;
				_children.push_back(temp);
			}
		}
		delete[] block;

	}

	BPTNode(const BPTNode& r) :
		_keys(r._keys),
		_degree(r._degree),
		_children(r._children),
		_records(r._records),
		_next(r._next),
		_father(r._father),
		_is_root(r._is_root),
		_blockNumber(r._blockNumber),
		_changed(r._changed),
		_isLeaf(r._isLeaf) {
	}

	~BPTNode() {
		if (_changed) {
			//_changed = false;
			int blockSize = BufferManager::Instance().getBlockSize();
			char* block = new char[blockSize];
			CharOutStream charouts(block, blockSize);
			charouts << _degree << _is_root << _isLeaf << _father;
			charouts << _keys.size();
			for (auto i = _keys.begin(); i != _keys.end(); i++) {
				charouts << (*i);
			}
			if (_isLeaf) {
				//charouts << _records.size();
				assert(_records.size() == _keys.size());
				for (auto i = _records.begin(); i != _records.end(); i++) {
					charouts << (*i);
				}
			} else {
				assert(_children.size() == _keys.size());
				for (auto i = _children.begin(); i != _children.end(); i++) {
					charouts << (*i);
				}
			}
			BufferManager::Instance().writeDataToFile(_fileName, _blockNumber, block);

		}
	}

	void set_father(block_ptr father) {
		_father = father;
		_changed = true;
	}

	void set_next(block_ptr next) {
		_next = next;
	}

	virtual bool is_full() {
		if (!_isLeaf) {
			if (_keys.size() != _children.size() - 1) {
				assert(_keys.size() == _children.size() - 1);
				// throw someError("Error! in internal node  keysize should be equal to children -1!!");
			}
			if (_children.size() == _degree + 1) {
				return true;
			} else {
				return false;
			}
		} else {
			if (_keys.size() != _records.size()) {
				// throw someError("Error! in leaf keysize should be equal to recordsize!!");
				assert(_keys.size() == _records.size());
			}
			if (_records.size() == _degree) {
				return true;
			} else {
				return false;
			}
		}

	}

	virtual bool isLeaf() {
		return _isLeaf;
	}

	virtual block_ptr find_down(const key_type& k) {
		if (_isLeaf) {
			return _blockNumber;
		} else {
			auto j = _children.begin();
			for (auto i = _keys.begin(); i != _keys.end(); i++) {
				if (k < *i) {
					return  *j;
				}
				j++;
			}
			return *j;
		}

	}

	int insert_key_children(key_type& k, block_ptr& second_block_num) {
		assert(!_isLeaf);
		if (_keys.size() == 0 || k > _keys.back()) {
			_keys.push_back(k);
			_children.push_back(second_block_num);
		} else {
			auto j = _children.begin();
			for (auto i = _keys.begin(); i != _keys.end(); i++) {
				if (k < *i) {
					_keys.insert(i, k);
					_children.insert(j, second_block_num);
					break;
				} else if (k == *i) {
					std::cerr << "duplicate keys" << std::endl;
					// throw someError("duplicate keys");
					assert(k != *i);
					//assert("duplicate keys");
					break;
				}
				j++;
			}
		}

		_changed = true;

		if (is_full()) {
			return split_and_propagate();
		} else {
			return 0;
		}
		// return 0 for no new  root, or the number of the new root;

	}

	int insert_entity(const key_type& k, const record_ptr& p) {
		assert(_isLeaf);
		if (_keys.size() == 0 || k > _keys.back()) {
			_keys.push_back(k);
			_records.push_back(p);
		} else {
			auto j = _records.begin();
			for (auto i = _keys.begin(); i != _keys.end() && j != _records.end(); i++, j++) {
				if (k < *i) {
					_keys.insert(i, k);
					_records.insert(j, p);
				} else if (k == *i) {
					if (*j == -1) {
						*j = p;
					} else {
						// throw someError("duplicate keys");
						std::cerr << "duplicate keys" << endl;
						assert(k != *i);
						break;
					}

				}
			}


		}

		if (is_full()) {
			return split_and_propagate();
		} else {
			return 0;
		}
	}

	int split_and_propagate() {
		_changed = true;
		if (_isLeaf) {
			auto key_it = _keys.begin();
			std::advance(key_it, _degree / 2);


			std::vector<key_type> second_keys(key_it, _keys.end());
			_keys.erase(key_it, _keys.end());
			//second_keys.splice(second_keys.begin(), _keys, key_it, _keys.end());


			key_type up_key = *key_it;
			auto ptr_it = _records.begin();
			std::advance(ptr_it, _degree / 2);


			std::vector<record_ptr> second_ptrs(ptr_it, _records.end());
			_records.erase(ptr_it, _records.end());
			//second_ptrs.splice(second_ptrs.begin(), _recordPtrs, ptr_it, _recordPtrs.end());

			block_ptr second_block_num = BufferManager::Instance().getNewBlockNum(_fileName);

			BPTNode<T> second(second_keys, _degree, _father, second_block_num, _next, true, second_ptrs);

			_next = second_block_num;
			if (this->is_root()) {
				set_root(false);
				block_ptr new_block_num = BufferManager::Instance().getNewBlockNum(_fileName);
				BPTNode<T> newRoot(_degree, new_block_num);
				newRoot.set_root(true);
				newRoot.insert_key_children(up_key, second_block_num);


				set_father(new_block_num);

				return new_block_num;


			} else {
				//std::cout << "now I am not a root" << std::endl;
				BPTNode<T> father(_father);
				return father.insert_key_children(up_key, second_block_num);
			}

		} else {

			auto key_it = _keys.begin();
			std::advance(key_it, _degree / 2);
			key_type up_key = *key_it;
			key_it++;

			std::vector<key_type> second_keys(key_it, _keys.end());
			//second_keys.splice(second_keys.begin(), _keys, key_it, _keys.end());
			_keys.erase(key_it - 1, _keys.end());

			auto child_it = _children.begin();
			std::advance(child_it, _degree / 2);

			std::vector<block_ptr> second_children(child_it, _children.end());
			_children.erase(child_it, _children.end());


			// new a block
			block_ptr second_block_num = BufferManager::Instance().getNewBlockNum(_fileName);
			BPTNode<T> second(second_keys, _degree, _father, second_block_num, _next, false, second_children);


			for (auto i = second_children.begin(); i != second_children.end(); i++) {
				BPTNode<T> temp_children(*i);
				temp_children.set_father(second_block_num);
			}
			if (this->is_root()) {
				set_root(false);
				block_ptr new_block_num = BufferManager::Instance().getNewBlockNum(_fileName);
				BPTNode<T> newRoot(_degree, new_block_num);
				newRoot.set_root(true);
				newRoot.insert_key_children(up_key, second_block_num);


				set_father(new_block_num);

				return new_block_num;


			} else {
				//std::cout << "now I am not a root" << std::endl;
				BPTNode<T> father(_father);
				return father.insert_key_children(up_key, second_block_num);
			}

		}

	}

	std::vector<block_ptr> get_children() {
		return _children;
	}

	virtual void print() {
		if (_isLeaf) {
			std::cout << "|leaf: ";
		} else {
			std::cout << "|node: ";
		}

		for (auto i = _keys.begin(); i != _keys.end(); i++) {
			std::cout << *i << " ";
		}
		std::cout << "|";
	}

	bool is_root() {
		return _is_root;
	}

	bool set_root(bool is_root) {
		_is_root = is_root;
		return is_root;
		_changed = true;
	}

	void set_children(block_ptr a) {
		_children.push_back(a);
		_changed = true;
	}

	std::vector<record_ptr> get_records() {
		if (isLeaf) {
			return _records;
		} else {
			std::cerr << "no-leaf node has no records" << std::endl;
			return std::vector<record_ptr> result;
		}
	}

	void set_records(const T& key, int record) {
		if (isLeaf) {
			if (_records.size() != _keys.size()) {
				std::cerr << "records and keys have different size!!!!!!!!!" << std::endl;
			}
			int flag = 0;
			for (size_t i = 0; i < _keys.size(); i++) {
				if (_keys[i] == key) {
					_records[i] = record;
					flag = 1;
				}
			}
			if (flag == 0) {
				std::cerr << "no such key when set record" << std::endl;
			}
		} else {
			std::cerr << "no-leaf node has no records" << std::endl;
		}
	}

};

template<typename T>
class BPlusTree {
	using block_ptr = int;
	using record_ptr = int;
	using key_type = T;


public:

	BPlusTree(size_t key_size) :
		key_size(key_size),
		_degree((BufferManager::Instance().getBlockSize()
			- sizeof(size_t)
			- sizeof(bool)
			- sizeof(bool)
			- sizeof(block_ptr)
			- sizeof(size_t))
			/ (key_size + sizeof(int))),
		_root_block_num(BufferManager::Instance().getNewBlockNum(_fileName)) {
		BPTNode<T> root(_root_block_num);
		root.set_root(true);
	}

	BPlusTree(size_t key_size, block_ptr root_block_num) :
		key_size(key_size),
		_degree((BufferManager::Instance().getBlockSize()
			- sizeof(size_t)
			- sizeof(bool)
			- sizeof(bool)
			- sizeof(block_ptr)
			- sizeof(size_t))
			/ (key_size + sizeof(int))),
		_root_block_num(root_block_num) {
		BPTNode<T> root(_root_block_num);
		root.set_root(true);
	}



	BPTNode<T> find_leaf(const key_type& k) {
		BPTNode<T> nowNode(_root_block_num);
		while (!nowNode._isLeaf) {
			nowNode = BPTNode<T>(nowNode.find_down(k));
		}
		return nowNode;
	}

	BPTNode<T> find_first_leaf() {
		BPTNode<T> nowNode(_root_block_num);
		while (!nowNode.isLeaf()) {
			//T key_temp = *(nowNode._children.begin());
			nowNode = BPTNode<T>(nowNode._children[0]);
		}
		return nowNode;
	}

	void insert_entity(const key_type& k, const record_ptr& p) {
		BPTNode<T> leafNode = find_leaf(k);
		block_ptr now_root = leafNode.insert_entity(k, p);
		if (now_root != 0) {
			_root_block_num = now_root;
		}
	}

	static BPlusTree<T> create(const std::vector<std::pair<T, record_ptr>>& entities, size_t key_size) {
		size_t degree = (BufferManager::Instance().getBlockSize()
			- sizeof(size_t)
			- sizeof(bool)
			- sizeof(bool)
			- sizeof(block_ptr)
			- sizeof(size_t))
			/ (key_size + sizeof(int));

		auto i = entities.begin();

		int root_block_num = BufferManager::Instance().getNewBlockNum(_fileName);
		BPTNode<T> root(root_block_num);

		//std::unique_ptr<BPTNode<T>>  father = &root;
		//std::unique_ptr<BPTNode<T>> last = nullptr;
		int father_num = root_block_num;
		int last_num = -1;
		while (i != entities.end()) {
			std::vector<T> keys; keys.resize(degree);
			std::vector<record_ptr> records; records.resize(degree);

			for (auto j = i; j != entities.end() && j != i + degree; j++) {
				keys.push_back((*j).first);
				records.push_back((*j).second);
			}
			i = j;
			//std::vector<key_type> keys, size_t degree, block_ptr father, block_ptr blockNumber, block_ptr next, bool isLeaf, std::vector<int> pointers;
			int new_block_num = BufferManager::Instance().getNewBlockNum();
			BPTNode<T> node = new BPTNode<T>(std::move(keys),
				degree,
				father_num,
				new_block_num,
				(-1),
				true,
				std::move(records));

			if (last_num != -1) {

				root_block_num = BPTNode<T>(father_num).insert_key_children(keys[0], new_block_num);
				BPTNode<T> last(last_num);
				last._next = new_block_num;
				father = last._father;
				last_num = new_block_num;

			} else {
				last_num = new_block_num;
			}

		}
		return BPlusTree<T>(key_size, root_block_num);

	}

	void serialize(CharOutStream couts) const {
		couts << key_size << _degree << _root_block_num;
	}

	static BPlusTree<T> deserialize(CharInStream cis) {
		size_t key_size;
		size_t degree;
		block_ptr root_block_num;
		cis >> key_size >> degree >> root_block_num;
		return BPlusTree<T>(key_size, root_block_num);
	}

	void print() {
		BPTNode<T> root(_root_block_num);
		if (root.isLeaf()) {
			root.print();
			std::cout << std::endl;
		} else {
			std::vector<block_ptr> output;
			output.push_back(_root_block_num);
			while (output.size() > 0) {
				std::vector<block_ptr> temp = output;
				output.clear();
				for (auto i = temp.begin(); i != temp.end(); i++) {
					BPTNode<T> nowNode(*i);
					nowNode.print();
					std::vector<block_ptr> children = nowNode.get_children();
					for (auto j = children.begin(); j != children.end(); j++) {
						output.push_back(*j);
					}
				}
				std::cout << std::endl;
			}
		}
	}

	std::vector<record_ptr> find_range(T low, T high) {
		BPTNode<T> now_leaf = find_leaf(low);
		BPTNode<T> terimal_leaf = find_leaf(low);
		std::vector<record_ptr> results;
		do {
			for (auto i = now_leaf._records.cbegin(); i < now_leaf._records.cend(); i++) {
				if (*i != -1) {
					results.push_back(*i);
				}
			}
			now_leaf = BPTNode<T>(now_leaf._next);
		} while (now_leaf._blockNumber != terimal_leaf._blockNumber);
		return results;
	}

	std::vector<record_ptr> find_smaller(T high) {
		BPTNode<T> now_leaf = find_first_leaf();
		BPTNode<T> terimal_leaf = find_leaf(high);
		std::vector<record_ptr> results;
		do {
			for (auto i = now_leaf._records.cbegin(); i < now_leaf._records.cend(); i++) {
				if (*i != -1) {
					results.push_back(*i);
				}
			}
			now_leaf = BPTNode<T>(now_leaf._next);
		} while (now_leaf._blockNumber != terimal_leaf._blockNumber);
		return results;
	}

	std::vector<record_ptr> find_larger(T low) {
		BPTNode<T> now_leaf = find_leaf(low);
		std::vector<record_ptr> results;
		do {
			for (auto i = now_leaf._records.cbegin(); i < now_leaf._records.cend(); i++) {
				if (*i != -1) {
					results.push_back(*i);
				}
			}
			now_leaf = BPTNode<T>(now_leaf._next);
		} while (now_leaf._blockNumber != -1);
		return results;
	}

	void clear() {
		// TODO: want a block clear;
	}

	void drop() {
		// TODO: want a block drop;
	}



private:
	size_t key_size;
	const size_t _degree; // B plus tree degree
	block_ptr _root_block_num;
	size_t keys_num;

};


