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



    string filename = GET_FILENAME(ts.name);

}

