//
// Created by Melody on 2017/5/24 0024.
//

#ifndef SQL_RECORDMANAGER_H
#define SQL_RECORDMANAGER_H
//#include "BufferManager.h"
#include "../BufferManager/BufferManager.h"
#include "TableStruct.h"

#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#define DATA_FILE_EXTENSION (".MYD")
#define DATA_FILE_PATH ("./minisql_data/")
#define GET_FILENAME(tn) (DATA_FILE_PATH + (tn) + DATA_FILE_EXTENSION)

class RecordManager {
private:
    BufferManager &bm;
    int blockSize;
    int getRecordLen(TableStruct &ts);
    bool isDup(TableStruct &ts, char* record);
    bool isDupEntry(vector<int> &pos, char *src, char *dst);

public:
    bool createTable(string tableName);
    bool dropTable(string tableName);
    int insertIntoTable(TableStruct &ts, char* data);
    int deleteRecord(TableStruct &ts, vector<int> &scope, vector<int> &moved);
    bool selectRecord(TableStruct &ts, vector<int> &scope, vector<char *> &result);
    bool selectAttribute(TableStruct &ts, string col, vector<char *> &values);

    RecordManager(BufferManager &bm, int blockSize):bm(bm), blockSize(blockSize){

    };
    ~RecordManager(){
        blockSize = 4096;
    };
};



#endif //SQL_RECORDMANAGER_H
