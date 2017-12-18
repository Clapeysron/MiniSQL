//
// Created by Melody on 2017/5/24 0024.
//

#ifndef SQL_TABLESTRUCT_H
#define SQL_TABLESTRUCT_H

#include <vector>
#include <string>

using namespace std;

class AttrInfo{
public:
    string name; // attribute name
    int type; // attribute type(int, float, varchar)
    int length;
    bool unique; // is it unique
    bool index; // is it in index column.

    AttrInfo(){

    };

    AttrInfo(string name, int type, int length , bool unique, bool index):name(name), type(type), length(length) , unique(unique), index(index){

    };
};

class TableStruct{
public:
    string name; // table name
    int recordAmount; // record's amount
    vector<AttrInfo> attrs; // the information of columns;

    TableStruct(){

    };

    TableStruct(string name, int recordAmount, vector<AttrInfo> attrs):name(name), recordAmount(recordAmount), attrs(attrs){

    };
};


#endif //SQL_TABLESTRUCT_H
