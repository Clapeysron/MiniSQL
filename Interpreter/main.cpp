//
//  main.cpp
//  Interpreter
//
//  Created by Clapeysron on 01/06/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#include <iostream>
#include "Interpreter.hpp"
int main(int argc, const char * argv[])
{
	if (isatty(fileno(stdin))) {
		Interpreter terminal;
		terminal.main_loop(cin);
	} else {
		Interpreter pipe;
		pipe.exe_loop(cin);
	}
}
