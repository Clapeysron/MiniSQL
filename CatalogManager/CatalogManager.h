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

//typedef int Record;


class FieldInfo {
public:
	FieldInfo(TypeInfo type, bool is_unique, std::string name, bool not_null) :
		_type(type),
		_is_unique(is_unique),
		_name(name),
		_not_null(not_null) {
	}

	~FieldInfo() = default;
	static FieldInfo deserialize(CharInStream& cis);
	//TODO: check the deserialize.serialize for not null attribute
	void serialize(CharOutStream& couts)const;
	const std::string get_name() {
		return _name;
	}
	int get_type_magic_num() {
		return _type.get_type_magic();
	}

	const TypeInfo& get_type() {
		return _type;
	}

private:
	TypeInfo _type;
	bool _is_unique;
	std::string _name;
	bool _not_null;
	bool _primary;
};

class TableInfo {

public:

	TableInfo(const std::vector<FieldInfo>& fields, const std::string& name, const size_t& primary) :
		_fields(fields),
		_name(name),
		_primary(primary) {

	}
	~TableInfo() = default;
	static TableInfo deserialize(CharInStream& cis);
	void serialize(CharOutStream& couts)const;
	std::string getName() {
		return _name;
	}

	TableInfo(std::string table_name, std::vector<std::string> name_list, std::vector<int> type_list, std::vector<int> length_list, std::vector<int> primary_flag, std::vector<int> unique_flag, std::vector<int> nnull_flag);


	bool have_column(const std::string& fieldName) {
		for (auto i = _fields.begin(); i != _fields.end(); i++) {
			if ((*i).get_name() == fieldName) {
				return true;
			}
		}
		return false;
	}

	bool have_columns(const std::vector<std::string>&  fieldNames) {
		for (auto i = fieldNames.cbegin(); i != fieldNames.cend(); i++) {
			if (!have_column(*i)) {
				return false;
			}
		}
		return true;
	}

	bool have_index(std::string fieldName);

	std::string find_index(std::string fieldName);

	TypeInfo get_type(std::string fieldName) {
		for (size_t i = 0; i < _fields.size(); i++) {
			if (_fields[i].get_name() == fieldName) {
				return _fields[i].get_type();
			}
		}

		std::cerr << "CatalogManager: have no such fieldName" << std::endl;
		return TypeInfo(0, 0);
		// TODO: throw an error
	}

	const std::vector<std::pair<Type, std::string>>& get_indices() {
		return indices;
	}

private:
	std::vector<FieldInfo> _fields;
	std::string _name;
	size_t _primary;  //which field in fields is primary key

	//size_t _block_num; // how many block the tableinfo uses.
	std::vector<std::pair<Type, std::string>> indices;


};

class CatalogManager {
public:
	CatalogManager(std::string fileName);
	~CatalogManager();
	void add_table(TableInfo& table) {
		if (0 == _tables.emplace(table.getName(), table).second) {
			//throw SomeError("Error: duplicate table name '" + table.getName() + "'");
			std::cerr << "Error: duplicate table name '" + table.getName() + "'" << std::endl;
		}
	}

	void create_table(const std::string& table_name,
		const std::vector<string>& name_list,
		const std::vector<int>& type_list,
		const std::vector<int>& length_list,
		const std::vector<int>& primary_flag,
		const std::vector<int>& unique_flag,
		const std::vector<int>& nnull_flag) {
		//parameter are yi tuo dongxi the same with tableinfo constructor
		// maybe have some thing to do with index manager and recordmanager
		std::vector<FieldInfo> fields;
		for (size_t i = 0; i < name_list.size(); i++) {
			fields.emplace_back(TypeInfo(type_list[i], length_list[i]), unique_flag[i], name_list[i], nnull_flag[i]);
		}
		//TODO:consider get primary flag as a size_t or int
		size_t primary_pos;
		int flag = 0;
		for (size_t i = 0; i < primary_flag.size(); i++) {
			if (primary_flag[i] == 1) {
				primary_pos = i;
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			primary_pos = primary_flag.size();
		}

		_tables.emplace(table_name, TableInfo(fields, table_name, primary_pos));

	}

	void drop_table(std::string& tableName) {
		
		if (0 == _tables.erase(tableName)) {
			//throw SomeError("Error: no such table '" + tableName + "'");
			std::cerr << "Error: no such table '" + tableName + "'" << std::endl;
		}
	}

	TableInfo& find_table(std::string& tableName) {
		auto i = _tables.find(tableName);
		if (i != _tables.end()) {
			return i->second;
		} else {
			//throw SomeError("Error: no such table '" + tableName + "'");
			std::cerr << "Error: no such table '" + tableName + "'" << std::endl;
		}
	}

	void writeBack(BufferManager& bufferManager);

	std::pair<Type, std::string> find_primary_index(std::string tableName);

	std::vector<std::pair<Type, std::string>> find_indices(std::string tableName);

	bool have_table(const std::string& name) {
		return _tables.count(name) == 1;
	}
	bool have_column(std::string tableName,std::vector<std::string> columns) {
		return find_table(tableName).have_columns(columns);
	}

	bool have_column(std::string tableName,std::string column) {
		return find_table(tableName).have_column(column);
	}

	const TypeInfo& get_type(std::string tableName, std::string fieldName) {
		return _tables.at(tableName).get_type(fieldName);
	}

	const std::vector<std::pair<Type, std::string>>& get_indices(const std::string& tableName) {
		return _tables.at(tableName).get_indices();
	}

	std::vector<std::string> show_tables() {
		std::vector<std::string> result;
		
		for_each(_tables.begin(), _tables.end(), [&result](const std::pair<std::string, TableInfo>& i) {
			result.push_back(i.first);
		});
		return result;
	}



private:
	std::map<std::string, TableInfo> _tables;
	std::string _fileName;
	BufferManager& BM;
	int locate_table(std::string& tableName);

	//TODO: serialize and deserialize should add _table_and_index and other function

};


