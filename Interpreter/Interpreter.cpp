//
//  Interpreter.cpp
//  MiniSQL
//
//  Created by Clapeysron on 22/05/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#include "Interpreter.hpp"
#include "API.hpp"
#include <sstream>
using namespace std;

void Interpreter::main_loop(istream& input)
{
    string sql;
    API sql_api;
    cout << "Minisql> ";
    while( input.peek()!=EOF )
    {
        sql.push_back(input.get());
        if(sql.back()==';')
        {
            // cout << sql_query(sql);
            clean_sql_from_bison();
            yy_switch_to_buffer(yy_scan_string(sql.data()));
            if(!yyparse())
            {
                sql_api.exec();
            }
            sql.clear();
            if(input.peek()=='\n')
            {
                cout << "Minisql> ";
            }
        }
        else if (input.peek()=='\n')
        {
            cout << "      -> ";
        }
    }
}
