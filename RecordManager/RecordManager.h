//
// Created by Melody on 2017/5/24 0024.
//

#ifndef SQL_RECORDMANAGER_H
#define SQL_RECORDMANAGER_H
//#include "BufferManager.h"
#include "../BufferManager/BufferManager.h"
#include "TableStruct.h"
#include "../CatalogManager/CharStream.h"

#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#define DATA_FILE_EXTENSION (".MYD")
#define DATA_FILE_PATH ("./minisql_data/")
//#define GET_FILENAME(tn) (DATA_FILE_PATH + (tn) + DATA_FILE_EXTENSION)
#define GET_FILENAME(tn) ((tn) + DATA_FILE_EXTENSION)

#define LT 1	// <
#define GT 2	// >
#define NE 3	// <>
#define EQ 4	// =
#define LE 5	// <=
#define GE 6	// >=

class Condition {
public:
    string column;

    int op;

    string _str;
    int _int;
    float _flo;
};
#define COL 81
#define STR 82
#define INT 83
#define DOUBLE 84

// string 82, int 83, double 84
#define COMPRASION	\
{ \
	switch (con.op) { \
		case EQ: if (vala != valb) return false; break; \
		case NE: if (vala == valb) return false; break; \
		case LT: if (vala >= valb) return false; break; \
		case GT: if (vala <= valb) return false; break; \
		case LE: if (vala > valb) return false; break; \
		case GE: if (vala < valb) return false; break; \
		default: break; \
	} \
}

class RecordManager {
private:
    BufferManager &bm;
    int blockSize;
    int getRecordLen(TableStruct &ts);
    bool isDup(TableStruct &ts, char* record);
    bool isDupEntry(vector<int> &pos, char *src, char *dst);

public:
    bool isConditionSatisfied(TableStruct &ts, Condition &con, char* str);
    bool createTable(string tableName);
    bool dropTable(string tableName);
    int insertIntoTable(TableStruct &ts, char* data);
    int deleteRecord(TableStruct &ts, vector<int> &scope);
	bool selectRecord(TableStruct & ts, vector<string>& col_list, vector<int>& scope, vector<string>& result);
    bool selectRecordWithCondition(TableStruct &ts, vector<int> &results, int comparison_type, int type_1, string &comp_1, int type_2, string comp_2);
    bool selectAll(TableStruct &ts, vector<string> &result, vector<string> &col_list);
    bool selectAttribute(TableStruct &ts, string col, vector<string> &values);

    RecordManager(BufferManager &bm, int blockSize):bm(bm), blockSize(blockSize){

    };
    ~RecordManager(){
        blockSize = 4096;
    };
};



#endif //SQL_RECORDMANAGER_H
