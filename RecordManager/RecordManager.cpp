//
// Created by Melody on 2017/5/24 0024.
//

#include "RecordManager.h"

int RecordManager::getRecordLen(TableStruct &ts){
    vector<AttrInfo> &attrs = ts.attrs;
    int length = 0;
    for(int i = 0; i < attrs.size(); ++i){
        length += attrs[i].length;
    }
    return length;
}

bool RecordManager::isDup(TableStruct &ts, char *record){
    int recordLen = getRecordLen(ts);
    int recordAmountInOneBlock = blockSize / recordLen;
    int blockAmount = (ts.recordAmount - 1) / recordAmountInOneBlock + 1;
    string filename = GET_FILENAME(ts.name);

    vector<AttrInfo> &attrs = ts.attrs;
    vector<int> uniqueList;

    int pos = 0;
    for(size_t i = 0; i < attrs.size(); ++i) {
        if(attrs[i].index == 1 || attrs[i].unique ==1){
            uniqueList.push_back(pos);
            uniqueList.push_back(pos + attrs[i].length);
        }
    }

    char *block = new char[blockSize];
    for(int j = 0; j < blockAmount; ++j) {
        bm.readDataFromFile(filename, j, block);
        for(int k = 0; k < recordAmountInOneBlock; ++k){
            if(isDupEntry(uniqueList, block + k * recordLen, record)){
                return true;
            }
        }
    }

    delete[] block;
    return false;
}

bool RecordManager::isDupEntry(vector<int> &pos, char *src, char *dst){
    for(size_t i = 0; i < pos.size(); i+=2){
        int start = pos[i];
        int stop = pos[i + 1];
        if(!memcmp(src + start, dst + start, (size_t)(stop - start))){
            return true;
        }
    }

    return false;
}

bool RecordManager::createTable(string tableName){
    string filename = GET_FILENAME(tableName);
    bm.createFile(filename);
    return true;
}

bool RecordManager::dropTable(string tableName){
    string filename = GET_FILENAME(tableName);
    bm.deleteFile(filename);
    return true;
}

int RecordManager::insertIntoTable(TableStruct &ts, char *data){
    if(isDup(ts, data)){
        return -1;
    }

    int recordLen = getRecordLen(ts);
    int recordAmountInOneBlock = blockSize / recordLen;


    string filename = GET_FILENAME(ts.name);

    if(ts.recordAmount % recordAmountInOneBlock == 0){ // It's full, write it to another new block;
        char *block = new char[blockSize];
        memcpy(block, data, (size_t)recordLen);

        bm.writeDataToFile(filename, ts.recordAmount / recordAmountInOneBlock, block);

        delete[] block;
    }else{
        char *block = new char[blockSize];
        bm.readDataFromFile(filename, ts.recordAmount / recordAmountInOneBlock, block);
        memcpy(block + ((ts.recordAmount % recordAmountInOneBlock) * recordLen), data, (size_t)recordLen);
        bm.writeDataToFile(filename, ts.recordAmount / recordAmountInOneBlock, block);

        delete[] block;
    }

    return ts.recordAmount++; // the record amount will add one and the index of this record will be returned.
}

int RecordManager::deleteRecord(TableStruct &ts, vector<int> &scope, vector<int> &moved){
    // I assume that the scope is the final list of records which will be deleted. So Index Manager can update the
    // index using the vector<int> scope. I just need to delete these index and I will return the moved records.
    // My strategy of delete records is swap it to the last block and I will delete the last records.
    int recordLen = getRecordLen(ts);
    int recordAmountInOneBlock = blockSize / recordLen;
    int blockAmount = (ts.recordAmount - 1) / recordAmountInOneBlock + 1;
    int currentAmount = ts.recordAmount;
    string filename = GET_FILENAME(ts.name);

    char *block = new char[blockSize];

    int currentBlock = -1;
    bool isModified = false;

    for(int i = (int)scope.size() - 1; i >= 0 ; i--){
        if(scope[i] / recordAmountInOneBlock != currentAmount){
            if(isModified && currentAmount > currentBlock * recordAmountInOneBlock){
                bm.writeDataToFile(filename, currentBlock, block);
            }

            currentBlock = scope[i] / recordAmountInOneBlock;
            bm.readDataFromFile(filename, currentBlock, block);
            isModified = false;
        }

        int j = scope[i] % recordAmountInOneBlock;
        char *buf = new char[blockSize];
        bm.readDataFromFile(filename, (currentAmount - 1) / recordAmountInOneBlock, buf);
        int index = (currentAmount - 1) % recordAmountInOneBlock;

        memcpy(block + j * recordLen, buf + index * recordLen, (size_t)recordLen);

        currentAmount--;
        if(currentAmount % recordAmountInOneBlock == 0){
            bm.deleteLastBlockOfFile(filename);
        }
        delete[] buf;
        isModified = true;
        if(currentAmount != scope[i]){
            moved.push_back(currentAmount);
            moved.push_back(scope[i]);
        }

    }

    if(isModified && currentAmount > currentBlock * recordAmountInOneBlock){
        bm.writeDataToFile(filename, currentBlock, block);
    }

    delete[] block;

    int tmp = ts.recordAmount - currentAmount;

    ts.recordAmount = currentAmount;

    return tmp;
}

bool RecordManager::selectRecord(TableStruct &ts, vector<int> &scope, vector<char *> &result){
    int recordLen = getRecordLen(ts);
    int recordAmountInOneBlock = blockSize / recordLen;
    int blockAmount = (ts.recordAmount - 1) / recordAmountInOneBlock + 1;
    string filename = GET_FILENAME(ts.name);

    result.clear();

    char* block = new char[blockSize];

    int currentBlock = -1;
    for (int i = 0; i < scope.size(); ++i) {
        if(scope[i] / recordAmountInOneBlock != currentBlock){
            currentBlock = scope[i] / recordAmountInOneBlock;
            bm.readDataFromFile(filename, currentBlock, block);
        }

        int j = scope[i] % recordAmountInOneBlock;

        char *target = new char[recordLen]; // this target is also needed to be freed in the higher place.
        memcpy(target, block + j * recordLen, (size_t)recordLen);
        result.push_back(target);
    }

    delete[] block;
    return true;
}

bool RecordManager::selectAttribute(TableStruct &ts, string col, vector<char *> &values){
    int recordLen = getRecordLen(ts);
    int recordAmountInOneBlock = blockSize / recordLen;
    int blockAmount = (ts.recordAmount - 1) / recordAmountInOneBlock + 1;
    string filename = GET_FILENAME(ts.name);

    values.clear();
    int start = 0, length = 0, pointer = 0;
    vector<AttrInfo> &attrs = ts.attrs;
    for (size_t i = 0; i < attrs.size(); ++i) {
        if(attrs[i].name == col){
            start = pointer;
            length = attrs[i].length;
        }
        pointer += attrs[i].length;
    }

    char *block = new char[blockSize];

    for (int j = 0; j < blockAmount; ++j) {
        bm.readDataFromFile(filename, j, block);
        for (int k = 0; k < recordAmountInOneBlock && j * recordAmountInOneBlock + k < ts.recordAmount; ++k) {
            char *buf = new char[length]; // this buf will be pushed into the vector, in the higher place you should free all of them.
            memcpy(buf, block + k * recordLen + start, (size_t)length);
            values.push_back(buf);
        }
    }

    delete[] block;
    return true;
}


