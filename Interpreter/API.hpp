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
using namespace std;
extern char sql_from_bison[1000];
class API
{
private:
    string sql_query;
public:
    void exec();
    void set_sql(string sql);
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
