#pragma once
#include <utility>
#include <list>
#include <map>
#include <memory>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <deque>



template<typename T>
class BPTNode {
	using ptr_type = int;   // RecordPtr is a int
	using block_ptr = int;
	using key_type = T;


protected:
	std::list<key_type> _keys;
	size_t _degree;
	bool _exist;
	bool _is_root;
	std::shared_ptr<BPTNode<T>> _father;

public:
	BPTNode(std::list<key_type> keys, size_t degree, std::shared_ptr<BPTNode<T>> father, bool exist, std::list<std::shared_ptr<BPTNode<T>>> children) :
		_keys(keys),
		_degree(degree),
		_exist(exist),
		_children(children),
		_father(father),
		_is_root(false) {
	}

	BPTNode(std::list<key_type> keys, std::shared_ptr<BPTNode<T>> father, size_t degree) :
		_keys(keys),
		_is_root(false),
		_exist(true),
		_degree(degree),
		_father(father) {
	}

	BPTNode(std::shared_ptr<BPTNode<T>> father, size_t degree) :
		_is_root(false),
		_exist(true),
		_degree(degree),
		_father(father) {
	}

	BPTNode(size_t degree) :
		_is_root(false),
		_exist(true),
		_degree(degree) {
	}


	~BPTNode() {
	}

	void set_father(std::shared_ptr<BPTNode<T>> father) {
		_father = father;
	}

	virtual bool is_full() {


		if (_keys.size() != _children.size() - 1) {
			assert(_keys.size() == _children.size() - 1);
			// throw someError("Error! in internal node  keysize should be equal to children -1!!");
		}
		if (_children.size() == _degree + 1) {
			return true;
		} else {
			return false;
		}
	}

	virtual bool isLeaf() {
		return false;
	}

	virtual std::shared_ptr<BPTNode<T>> find_down(const key_type& k) {
		auto j = _children.begin();
		for (auto i = _keys.begin(); i != _keys.end(); i++) {

			if (k < *i) {
				return  *j;
			}
			j++;
		}
		return *j;
	}

	void insert_key_children(key_type& k, std::shared_ptr<BPTNode<T>> second) {
		if (_keys.size() == 0 || k > _keys.back()) {
			_keys.push_back(k);

			_children.push_back(second);
		} else {
			auto j = _children.begin();
			for (auto i = _keys.begin(); i != _keys.end(); i++) {
				if (k < *i) {
					_keys.insert(i, k);
					//_children.erase(j);
					//_children.insert(j, first);
					//j++;
					_children.insert(j, second);
					//j--;
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

		if (is_full() && !BPTNode<T>::is_root()) {
			split_and_propagate();
		}
	}

	virtual void split_and_propagate() {

		auto key_it = _keys.begin();
		std::advance(key_it, _degree / 2);
		key_type up_key = *key_it;
		key_it++;
		std::list<key_type> second_keys;
		second_keys.splice(second_keys.begin(), _keys, key_it, _keys.end());
		_keys.pop_back();


		auto child_it = _children.begin();
		std::advance(child_it, _degree / 2 + 1);
		std::list<std::shared_ptr<BPTNode<T>>> second_children;
		second_children.splice(second_children.begin(), _children, child_it, _children.end());



		std::shared_ptr<BPTNode<T>> second(new BPTNode<key_type>(second_keys, _degree, _father, _exist, second_children));

		for (auto i = second_children.begin(); i != second_children.end(); i++) {
			(*i)->set_father(second);
		}

		if (this->is_root()) {

		} else {
			this->_father->insert_key_children(up_key, second);
		}

	}

	std::list<std::shared_ptr<BPTNode<T>>> get_children() {
		return _children;
	}

	virtual void print() {
		std::cout << "|node: ";
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
	}

	void set_children(std::shared_ptr<BPTNode<T>> a) {
		_children.push_back(a);
	}

private:
	std::list<std::shared_ptr<BPTNode<T>>> _children;

};

template<typename T>
class BPTLeaf : public BPTNode<T> {
	using ptr_type = int;   // RecordPtr is a int
	using block_ptr = int;
	using key_type = T;
	using BPTNode<T>::_keys;
	using BPTNode<T>::_degree;
	using BPTNode<T>::_exist;
	using BPTNode<T>::_father;

public:
	BPTLeaf(std::list<key_type> keys, size_t degree, std::shared_ptr<BPTNode<T>> father, std::list<ptr_type> recordPtrs, std::shared_ptr<BPTLeaf<T>> next) :
		BPTNode<T>(keys, father, degree),
		//BPTNode<T>(keys, degree),
		_recordPtrs(recordPtrs),
		_next(next) {
	}

	BPTLeaf(size_t degree) :
		BPTNode<T>(degree) {
	}


	~BPTLeaf() {
	}


	std::shared_ptr<BPTNode<T>> find_down(const key_type& k) override {
		std::shared_ptr<BPTNode<T>> ret(this);
		return ret;
	}


	void set_father(std::shared_ptr<BPTNode<T>> father) {
		_father = father;
	}



	bool isLeaf() override {
		return true;
	}

	bool is_full() override {
		if (_keys.size() != _recordPtrs.size()) {
			// throw someError("Error! in leaf keysize should be equal to recordsize!!");
			assert(_keys.size() == _recordPtrs.size());
		}
		if (_recordPtrs.size() == _degree) {
			return true;
		} else {
			return false;
		}
	}

	void insert_entity(const key_type& k, ptr_type& p) {
		if (_keys.size() == 0 || k > _keys.back()) {
			_keys.push_back(k);
			_recordPtrs.push_back(p);
		} else {
			for (auto i = _keys.begin(); i != _keys.end(); i++) {
				if (k < *i) {
					_keys.insert(i, k);
					_recordPtrs.push_back(p);
				} else if (k == *i) {
					// throw someError("duplicate keys");
					assert(k != *i);
					break;
				}
			}

		}

		if (is_full() && !BPTNode<T>::is_root()) {
			split_and_propagate();
		}
	}

	void split_and_propagate()override {

		auto key_it = _keys.begin();

		std::advance(key_it, _degree / 2);
		std::list<key_type> second_keys;
		second_keys.splice(second_keys.begin(), _keys, key_it, _keys.end());
		key_type up_key = *key_it;

		auto ptr_it = _recordPtrs.begin();
		std::advance(ptr_it, _degree / 2);
		std::list<ptr_type> second_ptrs;
		second_ptrs.splice(second_ptrs.begin(), _recordPtrs, ptr_it, _recordPtrs.end());

		std::shared_ptr<BPTLeaf<T>> second(new BPTLeaf<key_type>(second_keys, _degree, _father, second_ptrs, _next));
		_next = second;

		if (this->is_root()) {

		} else {
			//std::cout << "now I am not a root" << std::endl;

			this->_father->insert_key_children(up_key, second);
		}

	}





	void print()override {
		std::cout << "|leaf: ";
		for (auto i = _keys.begin(); i != _keys.end(); i++) {
			std::cout << *i << " ";
		}
		std::cout << "|";

	}




private:
	std::list<ptr_type> _recordPtrs;
	std::shared_ptr<BPTLeaf<T>> _next;

};



template<typename T>
class BPlusTree {
public:
	using ptr_type = int;   // RecordPtr is a int
	using key_type = T;

	BPlusTree(size_t key_size, size_t degree) :
		key_size(key_size),
		_degree(degree),
		_root(new BPTLeaf<T>(degree)),
		_header(new BPTNode<T>(NULL, 0)) {
		std::shared_ptr<BPTLeaf<T>> leaf_root = std::dynamic_pointer_cast<BPTLeaf<T>>(_root);
		leaf_root->set_father(_header);
		_root->set_root(true);

	}

	~BPlusTree() {
	}


	std::shared_ptr<BPTLeaf<T>> find_leaf(const key_type& k) {
		std::shared_ptr<BPTNode<T>> nowNode = _root;
		while (!nowNode->isLeaf()) {
			nowNode = nowNode->find_down(k);
		}
		return std::dynamic_pointer_cast<BPTLeaf<T>>(nowNode);
	}

	void insert_entity(const key_type& k, ptr_type p) {
		std::shared_ptr<BPTLeaf<T>> leafNode = find_leaf(k);
		leafNode->insert_entity(k, p);

		if (_root->is_full()) {
			std::cout << "the root is full" << std::endl;
			_root->set_root(false);

			std::shared_ptr<BPTNode<T>> newRoot = std::make_shared<BPTNode<T>>(_header, _degree);
			newRoot->set_root(true);
			newRoot->set_children(_root);
			if (_root->isLeaf()) {
				//std::cout << "		the old root is leaf" << std::endl;
				_root->set_father(newRoot);
				_root->split_and_propagate();
			} else {
				//std::cout << "		the old root is not leaf" << std::endl;
				_root->set_father(newRoot);
				_root->split_and_propagate();
			}
			_root = newRoot;
		}


	}


	void remove_leaf(key_type& k);

	void remove();

	// void serialize(CharOutStream couts) {

	// }

	// static deserialize(CharInStream cis, size_t size) {

	// }
	void print() {
		if (_root->isLeaf()) {
			_root->print();
			std::cout << std::endl;
		} else {


			std::list<std::shared_ptr<BPTNode<T>>> output;
			output.push_back(_root);
			while (output.size()>0) {
				std::list<std::shared_ptr<BPTNode<T>>> temp = output;
				output.clear();
				for (auto i = temp.begin(); i != temp.end(); i++) {
					(*i)->print();
					std::list<std::shared_ptr<BPTNode<T>>> children = (*i)->get_children();
					for (auto j = children.begin(); j != children.end(); j++) {
						output.push_back(*j);
					}
				}
				std::cout << std::endl;
			}

		}
	}

private:
	size_t key_size;
	const size_t _degree; // B plus tree degree
	std::shared_ptr<BPTNode<T>> _root;
	std::shared_ptr<BPTNode<T>> _header;
};
