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

private:
	std::vector<FieldInfo> _fields;
	size_t _index_pos;
	std::string _name;
	size_t _primary;  //which field in fields is primary key
	
	//size_t _block_num; // how many block the tableinfo uses.
	

};

class CatalogManager {
public:
	CatalogManager(std::string fileName, BufferManager& bufferManager);
	~CatalogManager();
	void add_table(TableInfo& table);
	void remove_table(std::string& tableName);
	TableInfo& find_table(std::string& table);
	void writeBack(BufferManager& bufferManager);


private:
	std::map<std::string, TableInfo> _tables;
	std::string _fileName;
	BufferManager catalogBufferManager;
	int locate_table(std::string& tableName);
	
};


