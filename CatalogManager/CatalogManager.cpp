#include "CatalogManager.h"

FieldInfo::FieldInfo(TypeInfo type, bool is_unique, std::string name):
			_type(type),
			_is_unique(is_unique),
			_name(name)
{
}

TableInfo::TableInfo(std::vector<FieldInfo> fields, size_t index_pos, std::string name, size_t primary,  size_t block_num) :
			_fields(fields),
			_index_pos(index_pos),
			_name(name),
			_primary(primary),
			//_position(position),
			_block_num(block_num)
{
}

CatalogManager::CatalogManager(std::string fileName, BufferManager& bufferManger) {
	std::vector<char*> allBlocks = bufferManger.readAll(fileName);
	for (auto i = allBlocks.cbegin(); i != allBlocks.cend(); i++) {
		MemoryInStream mis();
		mis>>
	}
}

