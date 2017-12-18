//
//  Interpreter.hpp
//  MiniSQL
//
//  Created by Clapeysron on 22/05/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#ifndef Interpreter_hpp
#define Interpreter_hpp

#include <stdio.h>
#include <iostream>
#include "lex.yy.h"
#include "sql.tab.h"
using namespace std;

class Interpreter
{
public:
    void main_loop (istream& input);
    void exe_loop (istream& input);
};

#endif /* Interpreter_hpp */
