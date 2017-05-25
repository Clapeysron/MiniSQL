#pragma once

class BPlusTreeBase {
public:
	BPlusTreeBase() {
	}

	virtual ~BPlusTreeBase() {}

private:

};

template<T>
class BPlusTree : public BPlusTreeBase {
public:
	using ptr_type = int;   // BlockPtr is a int
	using key_type = T;

	BPlusTree() {
	}

	~BPlusTree() {
	}
	class BPTNode {
	private:
		bool _is_leaf;
		size_t num_key;
		size_t num_ptr;
		key_type keys[degree];
		ptr_type ptrs[ptr_count];
		BPTNode *_next;
	};
	BPTNode find_leaf(key_type& k);
	void insert_leaf(key_type& k, ptr_type p);
	void remove_leaf(key_type& k);

	
	void remove();

private:
	
	const static size_t key_size = sizeof(key_type);
	const static size_t degree; // B plus tree degree
	const static size_t ptr_count;
	BPTNode _root;
};


