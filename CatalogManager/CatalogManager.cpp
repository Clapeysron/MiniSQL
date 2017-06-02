#include "CatalogManager.h"
#include "CharStream.h"

FieldInfo::FieldInfo(TypeInfo type, bool is_unique, std::string name) :
	_type(type),
	_is_unique(is_unique),
	_name(name) {
}

FieldInfo FieldInfo::deserialize(CharInStream& cis) {
	bool is_unique;
	std::string name;
	cis >> is_unique;
	cis >> name;

	return FieldInfo(TypeInfo::deserialize(cis), is_unique, name);
}

void FieldInfo::serialize(CharOutStream & couts) const {
	couts << _is_unique;
	couts << _name;
	TypeInfo::deserialize(couts);

}

TableInfo::TableInfo(std::vector<FieldInfo> fields, size_t index_pos, std::string name, size_t primary) :
	_fields(fields),
	_index_pos(index_pos),
	_name(name),
	_primary(primary) {
}

TableInfo TableInfo::deserialize(CharInStream& cis) {
	std::string name;
	size_t index_pos;
	size_t primary;
	size_t field_num;
	std::vector<FieldInfo> fields;
	cis >> name;
	cis >> index_pos;
	cis >> field_num;
	cis >> primary;
	for (size_t i = 0; i < field_num; i++) {
		fields.push_back(FieldInfo::deserialize(cis));
	}
	return TableInfo(fields, index_pos, name, primary);
}

void TableInfo::serialize(CharOutStream& couts)const {
	couts << _name;
	couts << _index_pos;
	couts << _fields.size();
	couts << _primary;
	for (size_t i = 0; i < _fields.size(); i++) {
		_fields[i].serialize(couts);
	}

}

CatalogManager::CatalogManager(std::string fileName, BufferManager& bufferManger) {
	std::vector<char*> allBlocks;
	bufferManger.readDatas(fileName, allBlocks);
	for (size_t i = 0; i < allBlocks.size(); ) {
		CharInStream cis(allBlocks[i], bufferManger.getBlockSize());
		// TODO : want a getBlockSize()
		std::string table_name;

		TableInfo& temp_table = TableInfo::deserialize(cis);
		_tables.emplace(temp_table.getName(), temp_table);
	}
}

CatalogManager::~CatalogManager() {
	/*int block_num = 0;
	for (auto i = _tables.cbegin(); i != _tables.cend(); i++) {
		char* buff = new char[BlockSize];
		CharOutStream couts(buff, BlockSize);
		i->second.serialize(couts);
		BufferManager.writeDataToFile(_);
		delete [] buff;
	}*/
}



void CatalogManager::writeBack(BufferManager& bufferManager) {
	size_t block_index = 0;
	for (auto i = _tables.cbegin(); i != _tables.cend(); i++) {
		char* buff = new char[bufferManager.getBlockSize()];
		CharOutStream couts(buff, bufferManager.getBlockSize());
		i->second.serialize(couts);
		bufferManager.writeDataToFile(_fileName, block_index, buff);
		delete[] buff;
	}
}

void CatalogManager::add_table(TableInfo& table) {
	if (0 == _tables.emplace(table.getName(), table).second) {
		throw SomeError("Error: duplicate table name '" + table.getName() + "'");
	}
	//_tables.emplace(table.getName(), table);
}

void CatalogManager::remove_table(std::string& tableName) {
	if (0 == _tables.erase(tableName) {
		throw SomeError("Error: no such table '" + tableName + "'");
	}
}

TableInfo& CatalogManager::find_table(std::string& tableName) {
	auto i = _tables.find(tableName);
	if (i != _tables.end()) {
		return i->second;
	} else {
		throw SomeError("Error: no such table '" + tableName + "'");
	}
}



