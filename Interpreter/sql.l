%option noyywrap nodefault yylineno case-insensitive
%{
#include "sql.tab.h"
#include <stdarg.h>
#include <string.h>

void yyerror(char *s, ...);

int oldstate;

%}

%s BTWMODE

%%

  /* keywords */

<BTWMODE>AND	{ BEGIN INITIAL; return AND; }
AND	{ return ANDOP; }
BETWEEN	{ BEGIN BTWMODE; return BETWEEN; }
CREATE	{ return CREATE; }
DISTINCT	{ return DISTINCT; }
DROP	{ return DROP; }
DELETE	{ return DELETE; }
INSERT	{ return INSERT; }
INT|INTEGER	{ return INTEGER; }
INTO	{ return INTO; }
FROM  { return FROM; }
NULL	{ return NULLX; }
INDEX|KEY { return KEY; }
ON { return ON; }
PRIMARY	{ return PRIMARY; }
SELECT	{ return SELECT; }
STATUS  { return STATUS; }
SET	{ return SET; }
SHOW	{ return SHOW; }
TABLES  { return TABLES; }
TABLE	{ return TABLE; }
UPDATE	{ return UPDATE; }
VALUES?	{ return VALUES; }
VARCHAR(ACTER)?	{ return VARCHAR; }
WHERE	{ return WHERE; }
QUIT 	{ return EXIT; }
EXIT	{ return EXIT; }

   /* numbers */

-?[0-9]+	        { yylval.intval = atoi(yytext); return INTNUM; } 

-?[0-9]+"."[0-9]* |
-?"."[0-9]+	|
-?[0-9]+E[-+]?[0-9]+	|
-?[0-9]+"."[0-9]*E[-+]?[0-9]+ |
-?"."[0-9]*E[-+]?[0-9]+	{ yylval.floatval = atof(yytext); return FLOAT; }

   /* strings */

'(\\.|''|[^'\n])*'	|
\"(\\.|\"\"|[^"\n])*\"  { yylval.strval = strdup(yytext); return STRING; }

'(\\.|[^'\n])*$	        { yyerror("Unterminated string %s", yytext); }
\"(\\.|[^"\n])*$	{ yyerror("Unterminated string %s", yytext); }

   /* operators */

[-+&~|^/%*(),.;!]   { return yytext[0]; }

"&&"		{ return ANDOP; }
"||"            { return OR; }

"="	{ yylval.subtok = 4; return COMPARISON; }
">="	{ yylval.subtok = 6; return COMPARISON; }
">"	{ yylval.subtok = 2; return COMPARISON; }
"<="	{ yylval.subtok = 5; return COMPARISON; }
"<"	{ yylval.subtok = 1; return COMPARISON; }
"!="	|
"<>"	{ yylval.subtok = 3; return COMPARISON; }

	/* peek ahead and return function if name */

[A-Za-z][A-Za-z0-9_]*	{ yylval.strval = strdup(yytext);
                          return NAME; }
`[^`/\\.\n]+`           { yylval.strval = strdup(yytext+1);
                          yylval.strval[yyleng-2] = 0;
                          return NAME; }

`[^`\n]*$               { yyerror("unterminated quoted name %s", yytext); return ERROR_STR; }

	/* else */

[ \t\n]         /* white space */
.               { yyerror("mystery character '%c'", *yytext); return ERROR_STR; }

%%

