#pragma warning(disable:4996)
#include <iostream>
#include "BufferManager.h"
#include "RecordManager.h"

#include <iostream>
#include <cstdio>
using namespace std;

void showHex(char *data, int length) {
    for (int i = 0; i < length; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main() {
    BufferManager bm = BufferManager(32, 4);
    RecordManager rm = RecordManager(bm, 32);
//    rm.createTable("lalala");
    vector<AttrInfo> ats;
    ats.push_back(AttrInfo("username", 1, 11, 0, 0));
    ats.push_back(AttrInfo("password", 1, 11, 0, 0));
    ats.push_back(AttrInfo("id", 0, 3, 1, 1));
    TableStruct ts("lalala", 0, ats);
    char *data = new char[26];

    memset(data, 0, 26);
    strcpy(data, "melody");
    strcpy(data + 11, "melody123");
    *(int *)(data + 22) = 1;
    if(rm.insertIntoTable(ts, data) != -1){
        cout << "Insert successfully" << endl;
    }
    memset(data, 0, 26);
    strcpy(data, "meonva");
    strcpy(data + 11, "whis");
    *(int *)(data + 22) = 2;
    if (rm.insertIntoTable(ts, data) != -1)
        cout << "Insert successfully" << endl;
    memset(data, 0, 26);
    strcpy(data, "gygygy");
    strcpy(data + 11, "951357");
    *(int *)(data + 22) = 3;
    if (rm.insertIntoTable(ts, data) != -1)
        cout << "Insert successfully" << endl;

    vector<int> scope;
    vector<char *> result;
    scope.push_back(0);
    scope.push_back(1);
    scope.push_back(2);
    rm.selectRecord(ts, scope, result);
    for (int i = 0; i < result.size(); ++i) {
        showHex(result[i], 26);
        delete[] result[i];
    }
    cout << result.size() << endl;

    rm.selectAttribute(ts, "username", result);

    for (int i = 0; i < result.size(); ++i) {
        showHex(result[i], 11);
        delete[] result[i];
    }

    scope.clear();
    scope.push_back(0);
    vector<int> results;
    rm.deleteRecord(ts, scope, results);

}