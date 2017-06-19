#include "TypeInfo.h"
#include "IndexManager.h"
#include "../BufferManager/BufferManager.h"
#include "../CatalogManager/CharStream.h"



IndexManager::IndexManager(std::string fileName) :_fileName(fileName) {
	std::vector<char*> allBlocks;
	BM.readDatas(_fileName, allBlocks);
	for (size_t i = 0; i < allBlocks.size();) {
		//std::string name;

		
		CharInStream cis(allBlocks[i], BM.getBlockSize());

		size_t blockNum;
		Type t;
		cis >> blockNum >> t;

		cis.resize(blockNum*BM.getBlockSize());
		cis.reset();

		switch (t) {
		case Int: {
			Index<int>&& index_temp = Index<int>::deserialize(cis);
			_intIndex.emplace(index_temp.getName(), index_temp);
		}
		case Float: {
			Index<float>&& index_temp = Index<float>::deserialize(cis);
			_floatIndex.emplace(index_temp.getName(), index_temp);
		}
		case Chars: {
			Index<float>&& index_temp = Index<float>::deserialize(cis);
			_stringIndex.emplace(index_temp.getName(), index_temp);
		}
		default:
			assert(1 == 2);
			//throw SomeError("no such type");
		}

	}
}




