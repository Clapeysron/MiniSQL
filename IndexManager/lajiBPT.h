#pragma once
#include <utility>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <deque>
#include "IndexManager.h"
#include "../CatalogManager/CharStream.h"



template<typename T>
class BPTNode {
	using record_ptr = int;   // RecordPtr is a int
	using block_ptr = int; // the block number in the file
	using key_type = T;

private:
	std::vector<block_ptr> _children;
	size_t _degree;
	bool _exist;
	bool _is_root;
	block_ptr _father;
	std::vector<key_type> _keys;
	block_ptr _blockNumber;
	bool _changed;
	bool _isLeaf;
	block_ptr _next;
	std::vector<record_ptr> _records;


public:
	BPTNode(std::vector<key_type> keys, size_t degree, block_ptr father, bool exist, block_ptr blockNumber, block_ptr next, bool isLeaf, std::vector<int> pointers) :
		_keys(keys),
		_degree(degree),
		_exist(exist),
		_father(father),
		_is_root(false),
		_blockNumber(blockNumber),
		_changed(true),
		_isLeaf(isLeaf),
		_next(next),
		_children(_isLeaf ? () : pointers),
		_records(_isLeaf ? pointers : ()) {
	}

	BPTNode(std::vector<key_type> keys, block_ptr father, size_t degree, block_ptr blockNumber, bool isLeaf) :
		_keys(keys),
		_is_root(false),
		_exist(true),
		_degree(degree),
		_father(father),
		_changed(true),
		_blockNumber(blockNumber),
		_isLeaf(isLeaf) {
	}

	BPTNode(size_t degree, block_ptr blockNumber) :
		_is_root(false),
		_exist(true),
		_degree(degree),
		_father(blockNumber),
		_blockNumber(blockNumber),
		_changed(true),
		_isLeaf(false) {
	}

	BPTNode(block_ptr blockNum) :
		_blockNumber(blockNum),
		_changed(false) {
		int blockSize = IndexManager::BM.getBlockSize();
		char* block = new char[blockSize];
		IndexManager::BM.readDataFromFile("Index", blockNum, block);
		// todo come up with a filename and check the (de)serialize correctness
		CharInStream cis(block, blockSize);
		cis >> _degree >> _is_root >> _isLeaf >> _father;
		size_t key_size;
		cis >> key_size;
		for (size_t i = 0; i < key_size; i++) {
			key_type temp;
			cis >> temp;
			_keys.push_back(temp);
		}
		if (_isLeaf) {
			size_t ptr_size;
			for (size_t i = 0; i < ptr_size; i++) {
				record_ptr temp;
				cis >> temp;
				_records.push_back(temp);
			}
		} else {
			size_t child_size;
			for (size_t i = 0; i < child_size; i++) {
				block_ptr temp;
				cis >> temp;
				_children.push_back(temp);
			}
		}
		delete block[];

	}

	BPTNode(const BPTNode& r) :
		_keys(r._keys),
		_degree(r._degree),
		_exist(r._exist),
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
		//TODO serialize
		if (_changed) {
			//_changed = false;
			int blockSize = IndexManager::BM.getBlockSize();
			char* block = new char[blockSize];
			CharOutStream charouts(block, blockSize);
			charouts << _degree << _is_root << _isLeaf << _father;
			charouts << _keys.size();
			for (auto i = _keys.begin(); i != _keys.end(); i++) {
				charouts << (*i);
			}
			if (_isLeaf) {
				charouts << _records.size();
				for (auto i = _records.begin(); i != _records.end(); i++) {
					charouts << (*i);
				}
			} else {
				charouts << _children.size();
				for (auto i = _children.begin(); i != _children.end(); i++) {
					charouts << (*i);
				}
			}
			IndexManager::BM.writeDataToFile(filename, _blockNumber, block);
			// TODO: come up with a filename
		}
	}

	void set_father(block_ptr father) {
		_father = father;
		_changed = true;
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

	void insert_key_children(key_type& k, block_ptr& second_block_num) {
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
					// throw someError("duplicate keys");
					assert(k != *i);
					//assert("duplicate keys");
					break;
				}
				j++;
			}
		}

		if (is_full() && !is_root()) {
			split_and_propagate();
		}
		_changed = true;
	}

	void insert_entity(const key_type& k, record_ptr& p) {
		assert(_isLeaf);
		if (_keys.size() == 0 || k > _keys.back()) {
			_keys.push_back(k);
			_records.push_back(p);
		} else {
			for (auto i = _keys.begin(); i != _keys.end(); i++) {
				if (k < *i) {
					_keys.insert(i, k);
					_records.push_back(p);
				} else if (k == *i) {
					// throw someError("duplicate keys");
					assert(k != *i);
					break;
				}
			}

		}

		if (is_full() && !is_root()) {
			split_and_propagate();
		}
	}

	virtual void split_and_propagate() {
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

			block_ptr second_block_num = IndexManager::BM.getNewBlockNum("Index");
			// TODO: replace it with a correct filename

			BPTNode<T> second(second_keys, _degree, _father, _exist, second_block_num, _next, true, second_ptrs);

			_next = second_block_num;

			if (this->is_root()) {

			} else {
				//std::cout << "now I am not a root" << std::endl;
				BPTNode<T> father(_father);
				father.insert_key_children(up_key, second_block_num);
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
			//second_children.splice(second_children.begin(), _children, child_it, _children.end());


			// new a block
			//std::shared_ptr<BPTNode<T>> second(new BPTNode<key_type>(second_keys, _degree, _father, _exist, second_children));
			block_ptr second_block_num = IndexManager::BM.getNewBlockNum("Index");
			// TODO: come up with a name for index
			BPTNode<T> second(second_keys, _degree, _father, _exist, second_block_num, _next, false, second_children);


			for (auto i = second_children.begin(); i != second_children.end(); i++) {
				BPTNode<T> temp_children(*i);
				temp_children.set_father(second_block_num);
			}

			if (this->is_root()) {

			} else {
				BPTNode<T> father(_father);
				father.insert_key_children(up_key, second_block_num);
			}
		}


		_changed = true;

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

	friend class BPTNode<T>;
public:



	BPlusTree(size_t key_size, size_t degree) :
		key_size(key_size),
		_degree(degree),
		_root_block_num(IndexManager::BM.getNewBlockNum("Index")),
		_root(BPTNode<T>(_root_block_num)) {
		_root->set_root(true);
	}

	BPlusTree(size_t key_size, size_t degree, block_ptr root_block_num) :
		key_size(key_size),
		_degree(degree),
		_root_block_num(root_block_num),
		_root(BPTNode<T>(_root_block_num)) {
		_root->set_root(true);
	}

	~BPlusTree() {
	}


	BPTNode<T> find_leaf(const key_type& k) {
		BPTNode<T> nowNode = _root;
		while (!nowNode._isLeaf) {
			nowNode = BPTNode<T>(nowNode.find_down(k));
		}
		return nowNode;
	}

	BPTNode<T> find_first_leaf() {
		BPTNode<T> nowNode = _root;
		while (!nowNode.isLeaf()) {
			nowNode = BPTNode<T>(nowNode.find_down(*(nowNode._children.begin())));
		}
		return nowNode;
	}

	void insert_entity(const key_type& k, const record_ptr& p) {
		BPTNode<T> leafNode = find_leaf(k);
		leafNode.insert_entity(k, p);

		if (_root.is_full()) {
			//std::cout << "the root is full" << std::endl;
			_root.set_root(false);
			block_ptr new_block_num = IndexManager::BM.getNewBlockNum("Index");
			BPTNode<T> newRoot(_degree, new_block_num);
			newRoot.set_root(true);
			newRoot.set_children(_root_block_num);
			if (_root._isLeaf) {
				//std::cout << "		the old root is leaf" << std::endl;
				_root.set_father(new_block_num);
				_root.split_and_propagate();
			} else {
				//std::cout << "		the old root is not leaf" << std::endl;
				_root.set_father(newRoot);
				_root.split_and_propagate();
			}
			_root = newRoot;
			_root_block_num = new_block_num;
		}


	}


	void remove_entity(key_type& k) {
		BPTNode<T> leafNode = find_leaf(k);
		leafNode.set_records(k, -1);
	}

	void remove();

	void serialize(CharOutStream couts) {
		couts << key_size << _degree << _root_block_num;
	}

	static BPlusTree<T> deserialize(CharInStream cis) {
		size_t key_size;
		size_t degree;
		block_ptr root_block_num;
		cis >> key_size >> degree >> root_block_num;
		return BPlusTree<T>(key_size, degree, root_block_num);
	}


	void print() {
		if (_root.isLeaf()) {
			_root.print();
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

	std::vector<record_ptr> find_all() {
		BPTNode<T> now_leaf = find_first_leaf();
		std::vector<record_ptr> results;

		do {
			for (auto i = now_leaf._records.cbegin(); i < now_leaf._records.cend(); i++) {
				if (*i != -1) {
					results.push_back(*i);
				}
			}
			now_leaf = BPTNode<T>(now_leaf._next);
		} while (now_leaf.get_next() != -1);
		return results;
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
		return result;
	}

	std::vector<record_ptr> find_smaller(T high) {
		BPTNode<T> now_leaf = find_first_leaf(low);
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
		return result;
	}

	std::vector<record_ptr> find_larger(T low) {
		BPTNode<T> now_leaf = find_first_leaf(low);
		std::vector<record_ptr> results;
		do {
			for (auto i = now_leaf._records.cbegin(); i < now_leaf._records.cend(); i++) {
				if (*i != -1) {
					results.push_back(*i);
				}
			}
			now_leaf = BPTNode<T>(now_leaf._next);
		} while (now_leaf._blockNumber != -1);
		return result;
	}

private:
	size_t key_size;
	const size_t _degree; // B plus tree degree
	BPTNode<T> _root;
	block_ptr _root_block_num;
};


