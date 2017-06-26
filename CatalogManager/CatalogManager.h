#pragma once

#include <string>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include "../IndexManager/TypeInfo.h"
//#include "..\BufferManager\BufferManager.h"
//#include "..\RecordManager\RecordManager.h"
#include <iostream>
#include <algorithm>


class FieldInfo {
public:
	FieldInfo(TypeInfo type, bool is_unique, std::string name, bool not_null, std::string indexName);

	~FieldInfo() = default;

	static FieldInfo deserialize(CharInStream& cis);

	void serialize(CharOutStream& couts)const;

	std::string get_name()const {
		return _name;
	}

	int get_type_magic_num();

	TypeInfo& get_type() {
		return _type;
	}

	const TypeInfo& cget_type() const {
		return _type;
	}

	std::string get_index()const {
		return _indexName;
	}

	bool get_unique()const {
		return _is_unique;
	}

	/*AttrInfo convert_to_attr() const  {
		return AttrInfo(_name, _type.get_type_magic(), _type.get_size(), _is_unique, _indexName=="");
	}*/

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

	bool have_columns_type(const std::vector<int>& types) {
		if (types.size() != _fields.size()) {
			return false;
		} else {
			for (size_t i = 0; i < types.size(); i++) {
				if (types[i] != _fields[i].get_type_magic_num()) {
					return false;
				}
			}
		}
		return true;
	}

	bool have_index(std::string fieldName);

	std::pair<Type, std::string> find_index(const std::string& fieldName);

	TypeInfo get_type(std::string fieldName);

	const std::vector<std::pair<Type, std::string>>& get_indices();

	const std::pair<Type, std::string>& get_primary_index();

	const std::vector<FieldInfo>& get_columns() {
		return _fields;
	}

	const FieldInfo& get_column(const std::string& name) {
		for (size_t i = 0; i < _fields.size(); i++) {
			if (_fields[i].get_name() == name) {
				return _fields[i];
			}
		}
		std::cout << "CatalogManager: no such column in '" + _name + "'";
		return FieldInfo(Int, false, "", false, "");
	}


	void show_fields() {
		for (size_t i = 0; i < _fields.size(); i++) {
			std::cout << _fields[i].get_name() << " " << _fields[i].get_type().name() << std::endl;
		}
	}




private:
	std::vector<FieldInfo> _fields;
	std::string _name;   //at most 56
	size_t _primary;  //which field in fields is primary key



};

class CatalogManager {
public:
	static CatalogManager& Instance() {
		static CatalogManager theSingleton("Catalog");
		return theSingleton;
	}
	

	void add_table(TableInfo& table);

	void create_table(const std::string& table_name,
		const std::vector<std::string>& name_list,
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
			std::cout << "Error: no such table '" + tableName + "'" << std::endl;
			
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

	bool have_column_type(const std::string& tableName, const std::vector<int>& types) {
		return _tables.at(tableName).have_columns_type(types);
	}

	const TypeInfo& get_type(std::string tableName, std::string fieldName) {
		return _tables.at(tableName).get_type(fieldName);
	}

	const std::vector<std::pair<Type, std::string>>& get_indices(const std::string& tableName) {
		return _tables.at(tableName).get_indices();
	}

	void show_fields(const std::string& tableName) {
		_tables.at(tableName).show_fields();
	}

	std::vector<std::string> show_tables();



private:
	std::map<std::string, TableInfo> _tables;
	std::string _fileName;

	CatalogManager(std::string fileName);
	~CatalogManager();

};




