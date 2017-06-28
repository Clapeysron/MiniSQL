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

%left ANDOP
%left BETWEEN
%left <subtok> COMPARISON

%token AND
%token BETWEEN
%token CREATE
%token DISTINCT
%token DELETE
%token DROP
%token EXEC
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
%token ON
%token PRIMARY
%token SELECT
%token STATUS
%token SET
%token SHOW
%token TABLE
%token TABLES
%token UPDATE
%token UNIQUE
%token VALUES
%token VARCHAR
%token WHERE
%token EXIT

%type <intval> select_expr_list data_type opt_length
%type <intval> column_list create_col_list column_atts
%type <intval> insert_stmt insert_vals_list insert_vals
%type <intval> update_stmt update_asgn_list
%type <intval> drop_stmt index_expr_list exec_stmt type_expr
%start stmt_list

%%

/* TOP */

stmt_list: stmt ';'
  | stmt_list stmt ';' ;

/* DELETE */

stmt: delete_stmt { emit("STMT"); }
    ;

delete_stmt: DELETE FROM NAME opt_where { emit("DELETE TABLE %s", $3); free($3); }
    ;

/* INSERT */

  stmt: insert_stmt { emit("STMT"); }
    ;

  insert_stmt: INSERT INTO NAME VALUES insert_vals_list { emit("INSERT %s", $3); free($3); }
    ;

   insert_vals_list: '(' insert_vals ')' { emit("VALUES %d", $2); $$ = 1; }

   insert_vals:
     type_expr { $$ = 1; }
   | insert_vals ',' type_expr { $$ = $1 + 1; }
    ;

/* UPDATE */

stmt: update_stmt { emit("STMT"); }
    ;

update_stmt: UPDATE NAME SET update_asgn_list opt_where { emit("UPDATE TABLE %s", $2); }
    ;

update_asgn_list:
    NAME COMPARISON type_expr { emit("ASSIGN %s", $1); free($1); $$ = 1; }
    ;

/* CREATE */

stmt: create_table_stmt { emit("STMT"); }
   ;

create_table_stmt: CREATE TABLE NAME
   '(' create_col_list ')' { emit("CREATE %s %d", $3, $5); free($3); }
   ;

stmt: create_index_stmt { emit("STMT"); }
    ;

create_index_stmt: CREATE KEY NAME ON NAME '(' index_expr_list ')'
  { emit("CREATEINDEX %s %s %d", $3, $5, $7); free($3); free($5); }

index_expr_list:
    column_list {$$ = $1; }
    | '*' { $$ = -1; }
    ;

create_col_list: create_definition { $$ = 1; }
    | create_col_list ',' create_definition { $$ = $1 + 1; }
    ;

create_definition: { emit("ENDCOL"); } NAME data_type column_atts
                   { emit("%d %s", $3, $2); free($2); }
    ;

column_atts: /* nil */ { $$ = 0; }
    | column_atts NOT NULLX             { emit("ATTR_NOTNULL"); $$ = $1 + 1; }
    | column_atts PRIMARY KEY   { emit("ATTR_PRIKEY"); $$ = $1 + 1; }
    | column_atts UNIQUE        { emit("ATTR_UNIQUE"); $$ = $1 + 1; }
    ;

opt_length: /* nil */ { $$ = 0; }
   | '(' INTNUM ')' { $$ = $2; }
   ;

data_type:
   INTEGER opt_length { $$ = 3000 + $2; }
   | FLOAT opt_length { $$ = 4000 + $2; }
   | VARCHAR '(' INTNUM ')' { $$ = 2000 + $3; }
   ;

column_list: NAME { emit(" %s", $1); free($1); $$ = 1; }
  | column_list ',' NAME  { emit(" %s", $3); free($3); $$ = $1 + 1; }
  ;


/* SELECT */

stmt: 
    select_stmt { emit("STMT"); }
    ;
select_stmt: 
    SELECT select_expr_list FROM NAME opt_where
    { emit("SELECT %d TABLE %s", $2, $4); } 
    ;
select_expr_list:
    select_expr { $$ = 1; }
    | select_expr_list ',' select_expr {$$ = $1 + 1; }
    | '*' { emit("81 *"); $$ = 1; }
    ;
select_expr: 
    NAME
    ;
opt_where:
    | WHERE expr { emit("WHERE"); }
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

/* EXEC */
stmt: exec_stmt { emit("STMT"); }
    ;

exec_stmt: EXEC NAME {emit("EXEC %s",$2); free($2); }
    | EXEC NAME '.' NAME {emit("EXEC %s.%s",$2,$4); free($2);free($4);}
    ;

/* EXIT */

stmt: exit_stmt { emit("STMT"); }
    ;

exit_stmt: EXIT { emit("EXIT"); }
    ;

/* EXPR */

expr: 
    type_expr BETWEEN type_expr AND type_expr %prec BETWEEN { emit("BETWEEN"); }
    ;

type_expr: 
    NAME          { emit("81 %s", $1); free($1); }
    | STRING        { emit("82 %s", $1); free($1); }
    | INTNUM        { emit("83 %d", $1); }
    | FLOAT     { emit("84 %g", $1); }
    ;

expr: 
    expr ANDOP expr { emit("AND"); }
    | type_expr COMPARISON type_expr { emit("CMP %d", $2); }
    ;

%%

char sql_from_bison[1000];
int first_flag=0;
void emit(char *s, ...)
{
  extern int yylineno;
  va_list ap;
  va_start(ap, s);
  char sql[1000]="";
  char temp[1000]="";
  vsprintf(sql, s, ap);
  strcpy(temp, " ");
  strcat(temp, sql);
  strcat(temp, sql_from_bison);
  if( first_flag==0 )
  {
      strcat(temp, " $");
      first_flag=1;
  }
  strcpy(sql_from_bison, temp);
}

void yyerror(char *s, ...)
{
  extern int yylineno;
  va_list ap;
  va_start(ap, s);
  fprintf(stderr, "Error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n\n");
}