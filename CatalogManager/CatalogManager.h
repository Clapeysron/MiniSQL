#pragma once

#include <string>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include "..\IndexManager\TypeInfo.h"
#include "..\BufferManager\BufferManager.h"
#include <iostream>
#include <algorithm>


class FieldInfo {
public:
	FieldInfo(TypeInfo type, bool is_unique, std::string name, bool not_null, std::string indexName);

	~FieldInfo() = default;

	static FieldInfo deserialize(CharInStream& cis);

	void serialize(CharOutStream& couts)const;

	std::string get_name() {
		return _name;
	}

	int get_type_magic_num();

	TypeInfo& get_type() {
		return _type;
	}

	std::string get_index() {
		return _indexName;
	}

private:
	TypeInfo _type;
	bool _is_unique;
	std::string _name;      // at most 56
	bool _not_null;
	/*bool _primary;*/
	std::string _indexName; // at most 56
};

class TableInfo {

public:

	TableInfo(const std::vector<FieldInfo>& fields, const std::string& name, const size_t& primary);

	~TableInfo() = default;

	static TableInfo deserialize(CharInStream& cis);

	void serialize(CharOutStream& couts)const;

	std::string getName() {
		return _name;
	}

	bool have_column(const std::string& fieldName);

	bool have_columns(const std::vector<std::string>&  fieldNames);

	bool have_index(std::string fieldName);

	std::pair<Type, std::string> find_index(const std::string& fieldName);

	TypeInfo get_type(std::string fieldName);

	const std::vector<std::pair<Type, std::string>>& get_indices();

	const std::pair<Type, std::string>& get_primary_index();



private:
	std::vector<FieldInfo> _fields;
	std::string _name;   //at most 56
	size_t _primary;  //which field in fields is primary key



};

class CatalogManager {
public:
	CatalogManager(std::string fileName);
	~CatalogManager();

	void add_table(TableInfo& table);

	void create_table(const std::string& table_name,
		const std::vector<string>& name_list,
		const std::vector<int>& type_list,
		const std::vector<int>& length_list,
		const std::vector<int>& primary_flag,
		const std::vector<int>& unique_flag,
		const std::vector<int>& nnull_flag);

	void drop_table(std::string& tableName);

	TableInfo& find_table(const std::string& tableName) {
		auto i = _tables.find(tableName);
		if (i != _tables.end()) {
			return i->second;
		} else {
			//throw SomeError("Error: no such table '" + tableName + "'");
			std::cerr << "Error: no such table '" + tableName + "'" << std::endl;
		}
	}


	std::pair<Type, std::string> find_primary_index(std::string tableName);

	std::vector<std::pair<Type, std::string>> find_indices(std::string tableName) {
		return find_table(tableName).get_indices();
	}

	std::pair<Type, std::string> find_index(const std::string& tableName, const std::string& fieldName) {
		return find_table(tableName).find_index(fieldName);
	}

	bool have_table(const std::string& name) {
		return _tables.count(name) == 1;
	}
	bool have_column(std::string tableName, std::vector<std::string> columns) {
		return find_table(tableName).have_columns(columns);
	}

	bool have_column(std::string tableName, std::string column) {
		return find_table(tableName).have_column(column);
	}

	const TypeInfo& get_type(std::string tableName, std::string fieldName) {
		return _tables.at(tableName).get_type(fieldName);
	}

	const std::vector<std::pair<Type, std::string>>& get_indices(const std::string& tableName) {
		return _tables.at(tableName).get_indices();
	}

	std::vector<std::string> show_tables();



private:
	std::map<std::string, TableInfo> _tables;
	std::string _fileName;
	
	static BufferManager BM;

};




