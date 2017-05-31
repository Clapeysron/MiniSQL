#pragma once

#include <string>
#include <vector>
#include <string>
#include <map>
#include "..\IndexManager\IndexManager.h"
#include "..\IndexManager\TypeInfo.h"
#include "..\BufferManager\BufferManager.h"


//typedef int Record;


class FieldInfo {
public:
	FieldInfo(TypeInfo type, bool is_unique, std::string name);
	~FieldInfo() = default;

private:
	TypeInfo _type;
	bool _is_unique;
	std::string _name;
};

class TableInfo {
public:
	TableInfo(std::vector<FieldInfo> fields, size_t index_pos, std::string name, size_t primary, size_t block_num);
	~TableInfo() = default;

private:
	std::vector<FieldInfo> _fields;
	size_t _index_pos;
	std::string _name;
	size_t _primary;
	
	size_t _block_num; // how many block the tableinfo uses.


};

class CatalogManager {
public:
	CatalogManager(std::string fileName, BufferManager& bufferManager);
	~CatalogManager() = default;
	void add_table(TableInfo& table);
	void remove_table(std::string& table);
	TableInfo& find_table(std::string& table);
	void writeBack();


private:
	std::map<std::string, TableInfo> _tables;
	std::string _fileName;
	BufferManager catalogBufferManager;
	
};


