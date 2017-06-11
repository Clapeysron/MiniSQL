/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_SQL_TAB_H_INCLUDED
# define YY_YY_SQL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NAME = 258,
    ERROR_STR = 259,
    STRING = 260,
    INTNUM = 261,
    FLOAT = 262,
    OR = 263,
    ANDOP = 264,
    BETWEEN = 265,
    COMPARISON = 266,
    AND = 267,
    CREATE = 268,
    DISTINCT = 269,
    DELETE = 270,
    DROP = 271,
    INDEX = 272,
    INSERT = 273,
    INTO = 274,
    INT = 275,
    INTEGER = 276,
    KEY = 277,
    KEYS = 278,
    FROM = 279,
    NULLX = 280,
    NOT = 281,
    ON = 282,
    PRIMARY = 283,
    SELECT = 284,
    STATUS = 285,
    SET = 286,
    SHOW = 287,
    TABLE = 288,
    TABLES = 289,
    UPDATE = 290,
    VALUES = 291,
    VARCHAR = 292,
    WHERE = 293,
    EXIT = 294
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 12 "sql.y" /* yacc.c:1915  */

	int intval;
	double floatval;
	char *strval;
	int subtok;

#line 101 "sql.tab.h" /* yacc.c:1915  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

#ifdef __cplusplus
extern "C"
#endif
int yyparse (void);

#endif /* !YY_YY_SQL_TAB_H_INCLUDED  */
