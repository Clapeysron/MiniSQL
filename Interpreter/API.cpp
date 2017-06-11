//
//  API.cpp
//  Interpreter
//
//  Created by Clapeysron on 03/06/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#include "API.hpp"
#include <sstream>
#include <string>

void API::set_sql(string sql)
{
    sql_query.clear();
    sql_query = sql;
}

void API::exec()
{
    string sql = get_sql_from_bison();
    string buf;
    cout << get_sql_from_bison();
    stringstream sql_stream(sql);
    while (sql_stream >> buf)
    {
        cout << buf << endl;
    }
//    sql_stream >> buf; // STMT
//    sql_stream >> buf; // SELECT INSERT
//    if(buf=="SELECT")
//    {
//        cout<<buf;
//    }
//    else if(buf=="INSERT")
//    {
//        cout<<buf;
//    }
//    else if(buf=="UPDATE")
//    {
//        cout<<buf;
//    }
//    else if(buf=="DELETE")
//    {
//        cout<<buf;
//    }
//    else if(buf=="CREATE")
//    {
//        cout<<buf;
//    }
//    else if(buf=="DROP")
//    {
//        cout<<buf;
//    }
//    else if(buf=="SHOW")
//    {
//        cout<<buf;
//    }
//    else if(buf=="EXIT")
//    {
//        cout<<buf;
//    }
//    else
//    {
//        cout << "Error with" << buf <<endl;
//    }
}

char* get_sql_from_bison()
{
    return sql_from_bison;
}

void clean_sql_from_bison()
{
    memset(sql_from_bison, 0, 1000*sizeof(char));
}
