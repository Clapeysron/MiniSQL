#include "CatalogManager.h"
#include "CharStream.h"
#include <iostream>



	

FieldInfo FieldInfo::deserialize(CharInStream& cis) {
	bool is_unique;
	bool is_not_null;
	std::string name;
	cis >> is_unique;
	cis >> is_not_null;
	cis >> name;

	return FieldInfo(TypeInfo::deserialize(cis), is_unique, name, is_not_null);
}

void FieldInfo::serialize(CharOutStream & couts) const {
	couts << _is_unique;
	couts << _name;
	_type.serialize(couts);

}



TableInfo TableInfo::deserialize(CharInStream& cis) {
	std::string name;
	
	size_t primary;
	size_t field_num;
	std::vector<FieldInfo> fields;
	cis >> name;
	cis >> field_num;
	cis >> primary;
	for (size_t i = 0; i < field_num; i++) {
		fields.push_back(FieldInfo::deserialize(cis));
	}
	return TableInfo(fields,  name, primary);
}

void TableInfo::serialize(CharOutStream& couts)const {
	couts << _name;
	couts << _fields.size();
	couts << _primary;
	for (size_t i = 0; i < _fields.size(); i++) {
		_fields[i].serialize(couts);
	}

}

CatalogManager::CatalogManager(std::string fileName) :
	_fileName(fileName)
{
	std::vector<char*> allBlocks;
	BM.readDatas(fileName, allBlocks);
	for (size_t i = 0; i < allBlocks.size(); i++) {
		CharInStream cis(allBlocks[i], BM.getBlockSize());

		std::string table_name;

		TableInfo temp_table = TableInfo::deserialize(cis);
		_tables.emplace(temp_table.getName(), temp_table);
	}
}





void CatalogManager::writeBack(BufferManager& bufferManager) {
	size_t block_index = 0;
	for (auto i = _tables.cbegin(); i != _tables.cend(); i++) {
		char* buff = new char[bufferManager.getBlockSize()];
		CharOutStream couts(buff, bufferManager.getBlockSize());
		i->second.serialize(couts);
		bufferManager.writeDataToFile(_fileName, block_index, buff);
		delete[] buff;
		block_index++;
	}
}

