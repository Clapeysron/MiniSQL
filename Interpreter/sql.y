%{
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

int yylex();
void yyerror(char *s, ...);
void emit(char *s, ...);
%}

%union {
	int intval;
	double floatval;
	char *strval;
	int subtok;
}
	
/* names and literal values */

%token <strval> NAME
%token <strval> ERROR_STR
%token <strval> STRING
%token <intval> INTNUM
%token <floatval> FLOAT
  
/* operators and precedence levels */

%left OR
%left ANDOP
%left BETWEEN
%left <subtok> COMPARISON

%token AND
%token BETWEEN
%token CREATE
%token DISTINCT
%token DELETE
%token DROP
%token INDEX
%token INSERT
%token INTO
%token INT
%token INTEGER
%token KEY
%token KEYS
%token FROM
%token NULLX
%token NOT
%token OR
%token ON
%token PRIMARY
%token SELECT
%token STATUS
%token SET
%token SHOW
%token TABLE
%token TABLES
%token UPDATE
%token VALUES
%token VARCHAR
%token WHERE
%token EXIT

%type <intval> select_opts select_expr_list data_type opt_length
%type <intval> table_references column_list create_col_list column_atts
%type <intval> insert_stmt insert_vals_list insert_vals
%type <intval> update_stmt update_asgn_list 
%type <intval> drop_stmt index_expr_list
%start stmt_list

%%

/* TOP */

stmt_list: stmt ';'
  | stmt_list stmt ';' ;

/* DELETE */

stmt: delete_stmt { emit("STMT"); }
    ;

delete_stmt: DELETE FROM NAME opt_where { emit("DELETE %s", $3); free($3); }
    ;

/* INSERT */

  stmt: insert_stmt { emit("STMT"); }
    ;

  insert_stmt: INSERT INTO NAME opt_col_names
     VALUES insert_vals_list { emit("INSERT %d %s", $6, $3); free($3); }
    ;

   opt_col_names: /* nil */
   | '(' column_list ')' { emit("INSERTCOLS %d", $2); }
    ;

   insert_vals_list: '(' insert_vals ')' { emit("VALUES %d", $2); $$ = 1; }
   | insert_vals_list ',' '(' insert_vals ')' { emit("VALUES %d", $4); $$ = $1 + 1; }

   insert_vals:
     expr { $$ = 1; }
   | insert_vals ',' expr { $$ = $1 + 1; }
    ;

/* UPDATE */

stmt: update_stmt { emit("STMT"); }
   ;

update_stmt: UPDATE table_references
    SET update_asgn_list
    opt_where { emit("UPDATE %d %d", $2, $4); }
;

update_asgn_list:
     NAME COMPARISON expr 
       { if ($2 != 4) yyerror("bad insert assignment to %s", $1);
   emit("ASSIGN %s", $1); free($1); $$ = 1; }
   | update_asgn_list ',' NAME COMPARISON expr
       { if ($4 != 4) yyerror("bad insert assignment to %s", $3);
   emit("ASSIGN %s.%s", $3); free($3); $$ = $1 + 1; }
   ;

/* CREATE */

stmt: create_table_stmt { emit("STMT"); }
   ;

create_table_stmt: CREATE TABLE NAME
   '(' create_col_list ')' { emit("CREATE %s", $3); free($3); }
   ;

stmt: create_index_stmt { emit("STMT"); }
    ;

create_index_stmt: CREATE KEY NAME ON NAME '(' index_expr_list ')'
  { emit("CREATEINDEX %s", $3); free($3); }

index_expr_list:
    expr { $$ = 1; }
    | index_expr_list ',' expr {$$ = $1 + 1; }
    | '*' { emit("INDEXALL"); $$ = 1; }
    ;

create_col_list: create_definition { $$ = 1; }
    | create_col_list ',' create_definition { $$ = $1 + 1; }
    ;

create_definition: { emit("STARTCOL"); } NAME data_type column_atts
                   { emit("COLUMNDEF %d %s", $3, $2); free($2); }

    | PRIMARY KEY '(' column_list ')'    { emit("PRIKEY %d", $4); }
    | KEY '(' column_list ')'            { emit("KEY %d", $3); }
    ;

column_atts: /* nil */ { $$ = 0; }
    | column_atts NOT NULLX             { emit("ATTR NOTNULL"); $$ = $1 + 1; }
    | column_atts NULLX
    | column_atts STRING        { emit("ATTR DEFAULT STRING %s", $2); free($2); $$ = $1 + 1; }
    | column_atts INTNUM        { emit("ATTR DEFAULT NUMBER %d", $2); $$ = $1 + 1; }
    | column_atts FLOAT         { emit("ATTR DEFAULT FLOAT %g", $2); $$ = $1 + 1; }
    | column_atts PRIMARY KEY   { emit("ATTR PRIKEY"); $$ = $1 + 1; }
    ;

opt_length: /* nil */ { $$ = 0; }
   | '(' INTNUM ')' { $$ = $2; }
   | '(' INTNUM ',' INTNUM ')' { $$ = $2 + 1000*$4; }
   ;

data_type:
   INTEGER opt_length { $$ = 50000 + $2; }
   | FLOAT opt_length { $$ = 90000 + $2; }
   | VARCHAR '(' INTNUM ')' { $$ = 130000 + $3; }
   ;

column_list: NAME { emit("COLUMN %s", $1); free($1); $$ = 1; }
  | column_list ',' NAME  { emit("COLUMN %s", $3); free($3); $$ = $1 + 1; }
  ;


/* SELECT */

stmt: 
    select_stmt { emit("STMT"); }
    ;
select_opts:
    { $$ = 0; }
    ;
select_stmt: 
    SELECT select_opts select_expr_list { emit("SELECTNODATA %d %d", $2, $3); } 
    | SELECT select_opts select_expr_list FROM table_references opt_where { emit("SELECT %d %d %d", $2, $3, $5); } 
    ;
opt_where:
    | WHERE expr { emit("WHERE"); }
    ;
select_expr_list:
    select_expr { $$ = 1; }
    | select_expr_list ',' select_expr {$$ = $1 + 1; }
    | '*' { emit("SELECTALL"); $$ = 1; }
    ;
select_expr: 
    expr
    ;
table_references:
    table_reference { $$ = 1; }
    | table_references ',' table_reference { $$ = $1 + 1; }
    ;

table_reference:
    NAME { emit("TABLE %s", $1); free($1); }
    ;

/* DROP */

stmt: drop_stmt { emit("STMT"); }
    ;

drop_stmt: DROP TABLE NAME { emit("DROP %s", $3); free $3; }
    | DROP KEY NAME ON NAME { emit("DROPINDEX %s %s", $3, $5);free $3; free $5; }
    ;

/* SHOW */

stmt: show_stmt { emit("STMT"); }
    ;

show_stmt: SHOW TABLES { emit("SHOWTABLE"); }
    | SHOW STATUS {emit("SHOWSTATUS"); }
    ;


/* EXIT */

stmt: exit_stmt { emit("STMT"); }
    ;

exit_stmt: EXIT { emit("EXIT"); }
    ;

/* EXPR */

expr: 
    NAME          { emit("NAME %s", $1); free($1); }
    | STRING        { emit("STRING %s", $1); free($1); }
    | INTNUM        { emit("NUMBER %d", $1); }
    | FLOAT     { emit("FLOAT %g", $1); }
    ;

expr: 
    expr ANDOP expr { emit("AND"); }
    | expr OR expr { emit("OR"); }
    | expr COMPARISON expr { emit("CMP %d", $2); }
    ;    

expr: 
    expr BETWEEN expr AND expr %prec BETWEEN { emit("BETWEEN"); }
    ;

%%

char sql_from_bison[200];

void emit(char *s, ...)
{
  extern int yylineno;
  va_list ap;
  va_start(ap, s);
  char sql[200]="";
  char temp[200]="";
  vsprintf(sql, s, ap);
  strcpy(temp, " ");
  strcat(temp, sql);
  strcat(temp, sql_from_bison);
  strcat(temp, " $");
  strcpy(sql_from_bison, temp);
}

void yyerror(char *s, ...)
{
  extern int yylineno;
  va_list ap;
  va_start(ap, s);
  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}