#pragma once
#include "TypeInfo.h"
#include <string>
#include <vector>
#include <string>
#include "IndexManager.h"

//typedef int Record;


class FieldInfo {
public:
	FieldInfo(TypeInfo type, bool is_unique, std::string name);
	~FieldInfo() = default;

private:
	TypeInfo _type;
	bool is_unique;
	std::string _name;
};

class TableInfo {
public:
	TableInfo(std::vector<FieldInfo> fields, std::vector<Index> _indexes, std::string name, FieldInfo primary, RecordPtr position);
	~TableInfo() = default;

private:
	std::vector<FieldInfo> _fields;
	size_t _index_pos;
	std::string _name;
	FieldInfo _primary;
	RecordPtr _position;

};

class CatalogManager {
public:
	CatalogManager();
	~CatalogManager() = default;
	void add_table(TableInfo& table);
	void remove_table(std::string& table);
	TableInfo& find_table(std::string& table);



private:
	std::vector<TableInfo> _tables;
	
};


