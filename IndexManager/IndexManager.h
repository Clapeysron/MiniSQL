#pragma once
#include "TypeInfo.h"
#include <vector>
#include <map>
#include "BPlusTree.h"

typedef size_t RecordPtr;
class Index {
public:
	Index(Type _type, size_t _size);

private:
	Type _type;
	size_t _size;
	BPlusTreeBase* tree;

};


class IndexManager {
public:
	IndexManager();
	~IndexManager() = default;
	//void add_index()
	Index& create_index(std::string& tableName, std::string& fieldName, TypeInfo Type);
	void drop_index(std::string& tableName, std::string& fiedlName);
	std::vector<RecordPtr> search(std::string& tableName);
	// TODO what kind of way to search is not determined. possible soltions: write all kind of search for three types and different operation(equal or range search)

private:
	std::multimap<std::string, Index> _tableAndIndexes;
};


