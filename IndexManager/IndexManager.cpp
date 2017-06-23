#include "TypeInfo.h"
#include "IndexManager.h"
#include "../BufferManager/BufferManager.h"
#include "../CatalogManager/CharStream.h"

BufferManager& IndexManager::BM = BufferManager(4096, 256);
const std::string IndexManager::bplustree_filename = "BPlusTree";