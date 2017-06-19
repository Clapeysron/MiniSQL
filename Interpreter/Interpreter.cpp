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
                cout << sql_api.exec();
                cout << endl;
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

void Interpreter::exe_loop(istream& input)
{
    string sql;
    API sql_api;
    cout << "> ";
    int line_num=1;
    char temp_char;
    while( input.peek()!=EOF )
    {
        temp_char=input.get();
        sql.push_back(temp_char);
        if(temp_char=='\n') line_num++;
        if(sql.back()==';')
        {
            clean_sql_from_bison();
            yy_switch_to_buffer(yy_scan_string(sql.data()));
            cout << "[line" << line_num << "] " << sql << endl;
            if(!yyparse())
            {
                cout << sql_api.exec();
                cout << endl;
            }
            sql.clear();
            if(input.peek()=='\n')
            {
                input.get();
                line_num++;
                cout << "> ";
            }
        }
    }
}
