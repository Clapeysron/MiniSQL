#pragma once

#include <string>
#include <vector>
#include <string>
#include <map>
#include "..\IndexManager\TypeInfo.h"
#include "..\BufferManager\BufferManager.h"


//typedef int Record;


class FieldInfo {
public:
	FieldInfo(TypeInfo type, bool is_unique, std::string name);
	~FieldInfo() = default;
	static FieldInfo deserialize(CharInStream& cis);
	void serialize(CharOutStream& couts)const;
	std::string get_name() {
		return _name;
	}
	int get_type() {
		return _type.get_type_magic();
	}

private:
	TypeInfo _type;
	bool _is_unique;
	std::string _name;
};

class TableInfo {
	//friend CharOutStream;
	//friend CatalogManager;
public:
	TableInfo(std::vector<FieldInfo> fields, size_t index_pos, std::string name, size_t primary);
	~TableInfo() = default;
	static TableInfo deserialize(CharInStream& cis);
	void serialize(CharOutStream& couts)const;
	std::string getName() {
		return _name;
	}

	TableInfo(std::string table_name, std::vector<std::string> name_list, std::vector<int> type_list, std::vector<int> length_list, std::vector<int> primary_flag, std::vector<int> unique_flag, std::vector<int> nnull_flag);


	bool have_field(std::string fieldName) {
		for (auto i = _fields.begin(); i < _fields.end(); i++) {
			if ((*i).get_name() == fieldName) {
				return true;
			} 
		}
		return false;
	}

	bool have_index(std::string fieldName);

	std::string find_index(std::string fieldName);

private:
	std::vector<FieldInfo> _fields;
	size_t _index_pos;
	std::string _name;
	size_t _primary;  //which field in fields is primary key
	
	//size_t _block_num; // how many block the tableinfo uses.
	std::vector<std::string> indices;
	

};

class CatalogManager {
public:
	CatalogManager(std::string fileName);
	~CatalogManager();
	void add_table(TableInfo& table);
	void create_table(); //parameter are yi tuo dongxi the same with tableinfo constructor
	// maybe have some thing to do with index manager and recordmanager
	void remove_table(std::string& tableName);
	TableInfo& find_table(std::string& table);
	bool have_table(std::string& table);
	void writeBack(BufferManager& bufferManager);

	std::string find_primary_index(std::string tableName);

	bool have_table(std::string name);
	bool have_column(std::vector<std::string> columns);
	bool have_column(std::string columns);

	std::vector<std::string> show_tables();



private:
	std::map<std::string, TableInfo> _tables;
	std::string _fileName;
	BufferManager& BM;
	int locate_table(std::string& tableName);
	std::map<std::string, std::vector<std::string>> _table_and_index;
	//TODO: serialize and deserialize should add _table_and_index and other function
	
};


