//
//  API.hpp
//  Interpreter
//
//  Created by Clapeysron on 03/06/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#ifndef API_hpp
#define API_hpp
#include <iostream>
#include "sql.tab.h"
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "Interpreter.hpp"
#include "../CatalogManager/CatalogManager.h"
#include "../IndexManager/IndexManager.h"
#include "../BufferManager/BufferManager.h"
using namespace std;
extern "C" char sql_from_bison[1000];
extern "C" int first_flag;
class API
{
private:
    string sql_query;
    
    string select_all(string table_name, vector<string> col_list);
    string select(string table_name, vector<string> col_list,vector<int> indexs);
    string insert(string table_name, vector<int> type_list, vector<string> value_list);
    string update_all(string table_name, string col_name, int update_type, string update_value);
    string update_part(string table_name, string col_name, int update_type, string update_value, vector<int> indexs);
    string delete_all(string table_name);
    string delete_part(string table_name, vector<int> indexs);
    string drop_table(string table_name);
    string drop_index(string table_name, string index_name);
    string show_tables();
    string show_status();
    string create(string table_name, vector<string> name_list, vector<int> type_list, vector<int> length_list, vector<int> primary_flag, vector<int> unique_flag, vector<int> nnull_flag);
    string create_index(string table_name, string index_name, vector<string> col_list);
    vector<int> search_where(string table_name, int comparison_type, int type_1, string comp_1, int type_2, string comp_2);
    vector<int> search_between(string table_name, int type_1, string comp, int type_2, string between_1, int type_3, string between_2);
    vector<int> and_indexs (vector<int> indexs_1, vector<int> indexs_2);
public:
    string exec();
    void set_sql(string sql);
	static CatalogManager CM;
	//static RecordManager& RM;
	static IndexManager IM;
};

#ifdef __cplusplus
extern "C"
#endif
char* get_sql_from_bison();

#ifdef __cplusplus
extern "C"
#endif
void clean_sql_from_bison();

#endif /* API_hpp */
