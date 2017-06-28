#include "CatalogManager.h"
#include "CharStream.h"
#include <iostream>
#include "../BufferManager/BufferManager.h"


//BufferManager CatalogManager::BM = BufferManager(4096, 256);`

TableInfo::TableInfo(const std::vector<FieldInfo>& fields, const std::string & name, const size_t & primary) :
	_fields(fields),
	_name(name),
	_primary(primary) {

}

TableInfo TableInfo::deserialize(CharInStream& cis) {
	std::string name;
	size_t primary;
	size_t field_num;

	std::vector<FieldInfo> fields;

	cis >> name >> field_num >> primary;

	for (size_t i = 0; i < field_num; i++) {
		//FieldInfo temp = FieldInfo::deserialize(cis);
		//fields.push_back(temp);
		fields.push_back(FieldInfo::deserialize(cis));
	}

	return TableInfo(fields, name, primary);
}

void TableInfo::serialize(CharOutStream& couts)const {
	couts << _name << _fields.size() << _primary;

	for (size_t i = 0; i < _fields.size(); i++) {
		_fields[i].serialize(couts);
	}



}

bool TableInfo::have_column(const std::string & fieldName) {
	for (auto i = _fields.begin(); i != _fields.end(); i++) {
		if ((*i).get_name() == fieldName) {
			return true;
		}
	}
	return false;
}

bool TableInfo::have_columns(const std::vector<std::string>& fieldNames) {
	for (auto i = fieldNames.cbegin(); i != fieldNames.cend(); i++) {
		if (!have_column(*i)) {
			return false;
		}
	}
	return true;
}

bool TableInfo::have_index(std::string fieldName) {
	for (size_t i = 0; i < _fields.size(); i++) {
		if (_fields[i].get_name() == fieldName) {
			if (_fields[i].get_index() != "") {
				return true;
			} else {
				return false;
			}
		}
	}
	std::cerr << "CatalogManager: TableInfo: no such field" << std::endl;
	return false;
}

std::pair<Type, std::string> TableInfo::find_index(const std::string& fieldName) {
	for (size_t i = 0; i < _fields.size(); i++) {
		if (_fields[i].get_name() == fieldName) {
			std::string indexName = _fields[i].get_index();
			if (indexName == "") {
				std::cerr << "CatalogManager: TableInfo: field have no index" << std::endl;
			}
			return std::make_pair(_fields[i].get_type().get_type(), indexName);
		}
	}
	std::cerr << "CatalogManager: TableInfo: no such field" << std::endl;
	return std::make_pair(Int, "");
}

TypeInfo TableInfo::get_type(std::string fieldName) {
	for (size_t i = 0; i < _fields.size(); i++) {
		if (_fields[i].get_name() == fieldName) {
			return _fields[i].get_type();
		}
	}

	std::cerr << "CatalogManager: have no such fieldName" << std::endl;
	return TypeInfo(0, 0);
	// TODO: throw an error
}

const std::vector<std::pair<Type, std::string>>& TableInfo::get_indices() {
	std::vector<std::pair<Type, std::string>> results;
	for (size_t i = 0; i < _fields.size(); i++) {
		std::string indexName = _fields[i].get_index();
		if (indexName == "") {
			results.emplace_back(_fields[i].get_type().get_type(), indexName);
		}
	}
	return results;
}

const std::pair<Type, std::string>& TableInfo::get_primary_index() {

	return std::make_pair(_fields[_primary].get_type().get_type(), _fields[_primary].get_index());
}

CatalogManager::CatalogManager(std::string fileName) :
	_fileName(fileName) {
	std::vector<char*> allBlocks;
	if (BufferManager::Instance().createFile(fileName)) {

	} else {
		BufferManager::Instance().readDatas(fileName, allBlocks);
		for (size_t i = 0; i < allBlocks.size(); i++) {
			CharInStream cis(allBlocks[i], BufferManager::Instance().getBlockSize());

			std::string table_name;

			TableInfo temp_table = TableInfo::deserialize(cis);
			_tables.emplace(temp_table.getName(), temp_table);
		}
		for (size_t i = 0; i != allBlocks.size(); i++) {
			delete[] allBlocks[i];
		}
	}

}

CatalogManager::~CatalogManager() {
	size_t block_index = 0;
	for (auto i = _tables.cbegin(); i != _tables.cend(); i++) {
		char* buff = new char[BufferManager::Instance().getBlockSize()];
		CharOutStream couts(buff, BufferManager::Instance().getBlockSize());
		i->second.serialize(couts);
		BufferManager::Instance().writeDataToFile(_fileName, block_index, buff);
		delete[] buff;
		block_index++;
	}
}

void CatalogManager::add_table(TableInfo & table) {
	if (0 == _tables.emplace(table.getName(), table).second) {
		//throw SomeError("Error: duplicate table name '" + table.getName() + "'");
		std::cerr << "Error: duplicate table name '" + table.getName() + "'" << std::endl;
	}
}

void CatalogManager::create_table(const std::string & table_name, const std::vector<string>& name_list, const std::vector<int>& type_list, const std::vector<int>& length_list, const std::vector<int>& primary_flag, const std::vector<int>& unique_flag, const std::vector<int>& nnull_flag) {
	//parameter are yi tuo dongxi the same with tableinfo constructor
	// maybe have some thing to do with index manager and recordmanager
	std::vector<FieldInfo> fields;
	for (size_t i = 0; i < name_list.size(); i++) {
		fields.emplace_back(TypeInfo(type_list[i], length_list[i]), unique_flag[i], name_list[i], nnull_flag[i], "");
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

void CatalogManager::drop_table(std::string & tableName) {

	if (0 == _tables.erase(tableName)) {
		//throw SomeError("Error: no such table '" + tableName + "'");
		std::cout << "Error:  CatalogManager:no such table '" + tableName + "'" << std::endl;
	}
}

std::pair<Type, std::string> CatalogManager::find_primary_index(std::string tableName) {
	//TableInfo& table = ;
	return find_table(tableName).get_primary_index();
}

std::vector<std::string> CatalogManager::show_tables() {
	std::vector<std::string> result;

	for_each(_tables.begin(), _tables.end(), [&result](const std::pair<std::string, TableInfo>& i) {
		result.push_back(i.first);
	});
	return result;
}

FieldInfo::FieldInfo(TypeInfo type, bool is_unique, std::string name, bool not_null, std::string indexName) :
	_type(type),
	_is_unique(is_unique),
	_name(name),
	_not_null(not_null),
	_indexName(indexName) {
}

FieldInfo FieldInfo::deserialize(CharInStream & cis) {
	bool is_unique;
	bool is_not_null;
	std::string name;
	std::string indexName;

	cis >> is_unique;
	cis >> is_not_null;
	cis >> name;
	cis >> indexName;

	//TypeInfo temp = TypeInfo::deserialize(cis);
	//return FieldInfo(temp, is_unique, name, is_not_null, indexName);
	return FieldInfo(TypeInfo::deserialize(cis), is_unique, name, is_not_null, indexName);
}

void FieldInfo::serialize(CharOutStream & couts) const {
	couts << _is_unique;
	couts << _not_null;
	couts << _name;
	couts << _indexName;
	_type.serialize(couts);
}

int FieldInfo::get_type_magic_num() const {
	return _type.get_type_magic();
}
