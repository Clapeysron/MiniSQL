/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "sql.y" /* yacc.c:339  */

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

int yylex();
void yyerror(char *s, ...);
void emit(char *s, ...);

#line 77 "sql.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "sql.tab.h".  */
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
#line 12 "sql.y" /* yacc.c:355  */

	int intval;
	double floatval;
	char *strval;
	int subtok;

#line 164 "sql.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SQL_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 181 "sql.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  32
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   156

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  74
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  151

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      41,    42,    44,     2,    43,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    40,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    76,    76,    77,    81,    84,    89,    92,    96,    97,
     100,   101,   104,   105,   110,   113,   119,   122,   129,   132,
     136,   139,   143,   144,   145,   148,   149,   152,   152,   155,
     156,   159,   160,   161,   162,   163,   164,   165,   168,   169,
     170,   174,   175,   176,   179,   180,   187,   190,   193,   194,
     196,   197,   200,   201,   202,   205,   208,   209,   213,   218,
     221,   222,   227,   230,   231,   237,   240,   246,   247,   248,
     249,   253,   254,   255,   259
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NAME", "ERROR_STR", "STRING", "INTNUM",
  "FLOAT", "OR", "ANDOP", "BETWEEN", "COMPARISON", "AND", "CREATE",
  "DISTINCT", "DELETE", "DROP", "INDEX", "INSERT", "INTO", "INT",
  "INTEGER", "KEY", "KEYS", "FROM", "NULLX", "NOT", "ON", "PRIMARY",
  "SELECT", "STATUS", "SET", "SHOW", "TABLE", "TABLES", "UPDATE", "VALUES",
  "VARCHAR", "WHERE", "EXIT", "';'", "'('", "')'", "','", "'*'", "$accept",
  "stmt_list", "stmt", "delete_stmt", "insert_stmt", "opt_col_names",
  "insert_vals_list", "insert_vals", "update_stmt", "update_asgn_list",
  "create_table_stmt", "create_index_stmt", "index_expr_list",
  "create_col_list", "create_definition", "$@1", "column_atts",
  "opt_length", "data_type", "column_list", "select_opts", "select_stmt",
  "opt_where", "select_expr_list", "select_expr", "table_references",
  "table_reference", "drop_stmt", "show_stmt", "exit_stmt", "expr", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
      59,    40,    41,    44,    42
};
# endif

#define YYPACT_NINF -72

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-72)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      41,     4,    22,    31,    53,   -72,    28,    64,   -72,    12,
      66,   -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,
     104,   105,   106,   107,   108,   109,    -1,   -72,   -72,   -72,
      -7,   -72,   -72,    74,   -72,    86,    75,    77,    90,   -72,
      79,   -72,   -72,   -72,   -72,   -72,   -23,   -72,    78,   115,
      64,   -72,   116,    10,    16,   -72,   118,   119,    87,    64,
      16,    16,    16,    16,    16,   113,    17,   -72,    84,    85,
     110,    -3,   -72,   124,    78,   -72,   -72,    48,    88,    23,
     -72,    68,    82,    73,   -72,    16,   125,   -72,     8,   119,
      89,   -72,    10,    -4,   -72,   128,    16,    91,   -72,    16,
      78,   122,   -72,    52,    78,    54,   119,   -72,    94,    94,
      95,   -72,   -72,    56,    78,    96,   127,    16,   -72,    16,
     -72,    58,   133,   -72,   -72,   134,    43,   -72,    16,    16,
      78,    78,   -72,    60,    99,   -72,   -72,   -72,   -72,   117,
     121,    78,    62,   -72,   138,   -72,   -72,   -72,   -72,   103,
     -72
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,    47,     0,     0,    66,     0,
       0,     4,     6,    14,    18,    20,    46,    59,    62,    65,
       0,     0,     0,     0,     0,     0,     0,    64,    63,    58,
       0,    56,     1,     0,     2,     0,     0,    50,     0,    60,
       8,    67,    68,    69,    70,    54,    48,    52,    55,     0,
       0,     3,     0,    27,     0,     5,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    57,     0,     0,
       0,     0,    25,     0,    51,    61,    44,     0,     0,    50,
      53,    72,    71,     0,    73,     0,     0,    15,     0,     0,
       0,    19,    27,     0,     9,     0,     0,     7,    49,     0,
      16,     0,    24,     0,    22,     0,     0,    26,    38,    38,
       0,    31,    45,     0,    12,     0,    74,     0,    21,     0,
      30,     0,     0,    42,    41,     0,    28,    10,     0,     0,
      17,    23,    29,     0,     0,    34,    35,    36,    33,     0,
       0,    13,     0,    39,     0,    43,    32,    37,    11,     0,
      40
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -72,   -72,   137,   -72,   -72,   -72,   -72,    18,   -72,   -72,
     -72,   -72,   -72,   -72,    57,   -72,   -72,    39,   -72,   -71,
     -72,   -72,   -50,   -72,    92,    97,   100,   -72,   -72,   -72,
     -54
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     9,    10,    11,    12,    58,    97,   113,    13,    66,
      14,    15,   103,    71,    72,    73,   126,   123,   111,    77,
      26,    16,    55,    46,    47,    30,    31,    17,    18,    19,
      48
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      74,    59,    41,   108,    42,    43,    44,    81,    82,    83,
      84,    41,    32,    42,    43,    44,    87,   109,   105,    41,
      60,    42,    43,    44,    49,     1,    20,     2,     3,    98,
       4,   100,    69,   110,   104,   121,    50,    21,    70,    91,
      92,     5,   114,    45,     6,   116,    22,     7,   135,   136,
     137,     8,   102,    23,     1,    54,     2,     3,    27,     4,
      86,    54,    28,   130,    24,   131,    50,    29,   138,   139,
       5,   140,    25,     6,   141,   114,     7,    62,    63,    64,
       8,    61,    62,    63,    64,    99,    61,    62,    63,    64,
      94,    95,    63,    64,   118,   119,   120,    95,   127,   128,
     132,    95,   143,   144,   148,   128,    34,    35,    36,    37,
      38,    39,    40,    52,    51,    54,    53,    56,    65,    68,
      57,    75,    76,    78,    85,    88,    89,    93,   101,    96,
     106,   112,    90,   117,   115,   122,   125,   129,    64,   133,
     134,   145,   146,   147,   149,   150,    33,   142,   124,   107,
      67,     0,    80,     0,     0,     0,    79
};

static const yytype_int16 yycheck[] =
{
      54,    24,     3,     7,     5,     6,     7,    61,    62,    63,
      64,     3,     0,     5,     6,     7,    66,    21,    89,     3,
      43,     5,     6,     7,    31,    13,    22,    15,    16,    79,
      18,    85,    22,    37,    88,   106,    43,    33,    28,    42,
      43,    29,    96,    44,    32,    99,    24,    35,     5,     6,
       7,    39,    44,    22,    13,    38,    15,    16,    30,    18,
      43,    38,    34,   117,    33,   119,    43,     3,    25,    26,
      29,    28,    19,    32,   128,   129,    35,     9,    10,    11,
      39,     8,     9,    10,    11,    12,     8,     9,    10,    11,
      42,    43,    10,    11,    42,    43,    42,    43,    42,    43,
      42,    43,    42,    43,    42,    43,    40,     3,     3,     3,
       3,     3,     3,    27,    40,    38,    41,    27,     3,     3,
      41,     3,     3,    36,    11,    41,    41,     3,     3,    41,
      41,     3,    22,    11,    43,    41,    41,    41,    11,     6,
       6,    42,    25,    22,     6,    42,     9,   129,   109,    92,
      50,    -1,    60,    -1,    -1,    -1,    59
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    13,    15,    16,    18,    29,    32,    35,    39,    46,
      47,    48,    49,    53,    55,    56,    66,    72,    73,    74,
      22,    33,    24,    22,    33,    19,    65,    30,    34,     3,
      70,    71,     0,    47,    40,     3,     3,     3,     3,     3,
       3,     3,     5,     6,     7,    44,    68,    69,    75,    31,
      43,    40,    27,    41,    38,    67,    27,    41,    50,    24,
      43,     8,     9,    10,    11,     3,    54,    71,     3,    22,
      28,    58,    59,    60,    75,     3,     3,    64,    36,    70,
      69,    75,    75,    75,    75,    11,    43,    67,    41,    41,
      22,    42,    43,     3,    42,    43,    41,    51,    67,    12,
      75,     3,    44,    57,    75,    64,    41,    59,     7,    21,
      37,    63,     3,    52,    75,    43,    75,    11,    42,    43,
      42,    64,    41,    62,    62,    41,    61,    42,    43,    41,
      75,    75,    42,     6,     6,     5,     6,     7,    25,    26,
      28,    75,    52,    42,    43,    42,    25,    22,    42,     6,
      42
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    45,    46,    46,    47,    48,    47,    49,    50,    50,
      51,    51,    52,    52,    47,    53,    54,    54,    47,    55,
      47,    56,    57,    57,    57,    58,    58,    60,    59,    59,
      59,    61,    61,    61,    61,    61,    61,    61,    62,    62,
      62,    63,    63,    63,    64,    64,    47,    65,    66,    66,
      67,    67,    68,    68,    68,    69,    70,    70,    71,    47,
      72,    72,    47,    73,    73,    47,    74,    75,    75,    75,
      75,    75,    75,    75,    75
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     3,     1,     4,     1,     6,     0,     3,
       3,     5,     1,     3,     1,     5,     3,     5,     1,     6,
       1,     8,     1,     3,     1,     1,     3,     0,     4,     5,
       4,     0,     3,     2,     2,     2,     2,     3,     0,     3,
       5,     2,     2,     4,     1,     3,     1,     0,     3,     6,
       0,     2,     1,     3,     1,     1,     1,     3,     1,     1,
       3,     5,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     5
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 81 "sql.y" /* yacc.c:1661  */
    { emit("STMT"); }
#line 1367 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 5:
#line 84 "sql.y" /* yacc.c:1661  */
    { emit("DELETE %s", (yyvsp[-1].strval)); free((yyvsp[-1].strval)); }
#line 1373 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 6:
#line 89 "sql.y" /* yacc.c:1661  */
    { emit("STMT"); }
#line 1379 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 7:
#line 93 "sql.y" /* yacc.c:1661  */
    { emit("INSERT %d %s", (yyvsp[0].intval), (yyvsp[-3].strval)); free((yyvsp[-3].strval)); }
#line 1385 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 9:
#line 97 "sql.y" /* yacc.c:1661  */
    { emit("INSERTCOLS %d", (yyvsp[-1].intval)); }
#line 1391 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 10:
#line 100 "sql.y" /* yacc.c:1661  */
    { emit("VALUES %d", (yyvsp[-1].intval)); (yyval.intval) = 1; }
#line 1397 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 11:
#line 101 "sql.y" /* yacc.c:1661  */
    { emit("VALUES %d", (yyvsp[-1].intval)); (yyval.intval) = (yyvsp[-4].intval) + 1; }
#line 1403 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 12:
#line 104 "sql.y" /* yacc.c:1661  */
    { (yyval.intval) = 1; }
#line 1409 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 13:
#line 105 "sql.y" /* yacc.c:1661  */
    { (yyval.intval) = (yyvsp[-2].intval) + 1; }
#line 1415 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 14:
#line 110 "sql.y" /* yacc.c:1661  */
    { emit("STMT"); }
#line 1421 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 15:
#line 115 "sql.y" /* yacc.c:1661  */
    { emit("UPDATE %d %d", (yyvsp[-3].intval), (yyvsp[-1].intval)); }
#line 1427 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 16:
#line 120 "sql.y" /* yacc.c:1661  */
    { if ((yyvsp[-1].subtok) != 4) yyerror("bad insert assignment to %s", (yyvsp[-2].strval));
   emit("ASSIGN %s", (yyvsp[-2].strval)); free((yyvsp[-2].strval)); (yyval.intval) = 1; }
#line 1434 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 17:
#line 123 "sql.y" /* yacc.c:1661  */
    { if ((yyvsp[-1].subtok) != 4) yyerror("bad insert assignment to %s", (yyvsp[-2].strval));
   emit("ASSIGN %s.%s", (yyvsp[-2].strval)); free((yyvsp[-2].strval)); (yyval.intval) = (yyvsp[-4].intval) + 1; }
#line 1441 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 18:
#line 129 "sql.y" /* yacc.c:1661  */
    { emit("STMT"); }
#line 1447 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 19:
#line 133 "sql.y" /* yacc.c:1661  */
    { emit("CREATE %s", (yyvsp[-3].strval)); free((yyvsp[-3].strval)); }
#line 1453 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 20:
#line 136 "sql.y" /* yacc.c:1661  */
    { emit("STMT"); }
#line 1459 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 21:
#line 140 "sql.y" /* yacc.c:1661  */
    { emit("CREATEINDEX %s", (yyvsp[-5].strval)); free((yyvsp[-5].strval)); }
#line 1465 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 22:
#line 143 "sql.y" /* yacc.c:1661  */
    { (yyval.intval) = 1; }
#line 1471 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 23:
#line 144 "sql.y" /* yacc.c:1661  */
    {(yyval.intval) = (yyvsp[-2].intval) + 1; }
#line 1477 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 24:
#line 145 "sql.y" /* yacc.c:1661  */
    { emit("INDEXALL"); (yyval.intval) = 1; }
#line 1483 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 25:
#line 148 "sql.y" /* yacc.c:1661  */
    { (yyval.intval) = 1; }
#line 1489 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 26:
#line 149 "sql.y" /* yacc.c:1661  */
    { (yyval.intval) = (yyvsp[-2].intval) + 1; }
#line 1495 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 27:
#line 152 "sql.y" /* yacc.c:1661  */
    { emit("STARTCOL"); }
#line 1501 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 28:
#line 153 "sql.y" /* yacc.c:1661  */
    { emit("COLUMNDEF %d %s", (yyvsp[-1].intval), (yyvsp[-2].strval)); free((yyvsp[-2].strval)); }
#line 1507 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 29:
#line 155 "sql.y" /* yacc.c:1661  */
    { emit("PRIKEY %d", (yyvsp[-1].intval)); }
#line 1513 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 30:
#line 156 "sql.y" /* yacc.c:1661  */
    { emit("KEY %d", (yyvsp[-1].intval)); }
#line 1519 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 31:
#line 159 "sql.y" /* yacc.c:1661  */
    { (yyval.intval) = 0; }
#line 1525 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 32:
#line 160 "sql.y" /* yacc.c:1661  */
    { emit("ATTR NOTNULL"); (yyval.intval) = (yyvsp[-2].intval) + 1; }
#line 1531 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 34:
#line 162 "sql.y" /* yacc.c:1661  */
    { emit("ATTR DEFAULT STRING %s", (yyvsp[0].strval)); free((yyvsp[0].strval)); (yyval.intval) = (yyvsp[-1].intval) + 1; }
#line 1537 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 35:
#line 163 "sql.y" /* yacc.c:1661  */
    { emit("ATTR DEFAULT NUMBER %d", (yyvsp[0].intval)); (yyval.intval) = (yyvsp[-1].intval) + 1; }
#line 1543 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 36:
#line 164 "sql.y" /* yacc.c:1661  */
    { emit("ATTR DEFAULT FLOAT %g", (yyvsp[0].floatval)); (yyval.intval) = (yyvsp[-1].intval) + 1; }
#line 1549 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 37:
#line 165 "sql.y" /* yacc.c:1661  */
    { emit("ATTR PRIKEY"); (yyval.intval) = (yyvsp[-2].intval) + 1; }
#line 1555 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 38:
#line 168 "sql.y" /* yacc.c:1661  */
    { (yyval.intval) = 0; }
#line 1561 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 39:
#line 169 "sql.y" /* yacc.c:1661  */
    { (yyval.intval) = (yyvsp[-1].intval); }
#line 1567 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 40:
#line 170 "sql.y" /* yacc.c:1661  */
    { (yyval.intval) = (yyvsp[-3].intval) + 1000*(yyvsp[-1].intval); }
#line 1573 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 41:
#line 174 "sql.y" /* yacc.c:1661  */
    { (yyval.intval) = 50000 + (yyvsp[0].intval); }
#line 1579 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 42:
#line 175 "sql.y" /* yacc.c:1661  */
    { (yyval.intval) = 90000 + (yyvsp[0].intval); }
#line 1585 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 43:
#line 176 "sql.y" /* yacc.c:1661  */
    { (yyval.intval) = 130000 + (yyvsp[-1].intval); }
#line 1591 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 44:
#line 179 "sql.y" /* yacc.c:1661  */
    { emit("COLUMN %s", (yyvsp[0].strval)); free((yyvsp[0].strval)); (yyval.intval) = 1; }
#line 1597 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 45:
#line 180 "sql.y" /* yacc.c:1661  */
    { emit("COLUMN %s", (yyvsp[0].strval)); free((yyvsp[0].strval)); (yyval.intval) = (yyvsp[-2].intval) + 1; }
#line 1603 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 46:
#line 187 "sql.y" /* yacc.c:1661  */
    { emit("STMT"); }
#line 1609 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 47:
#line 190 "sql.y" /* yacc.c:1661  */
    { (yyval.intval) = 0; }
#line 1615 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 48:
#line 193 "sql.y" /* yacc.c:1661  */
    { emit("SELECTNODATA %d %d", (yyvsp[-1].intval), (yyvsp[0].intval)); }
#line 1621 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 49:
#line 194 "sql.y" /* yacc.c:1661  */
    { emit("SELECT %d %d %d", (yyvsp[-4].intval), (yyvsp[-3].intval), (yyvsp[-1].intval)); }
#line 1627 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 51:
#line 197 "sql.y" /* yacc.c:1661  */
    { emit("WHERE"); }
#line 1633 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 52:
#line 200 "sql.y" /* yacc.c:1661  */
    { (yyval.intval) = 1; }
#line 1639 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 53:
#line 201 "sql.y" /* yacc.c:1661  */
    {(yyval.intval) = (yyvsp[-2].intval) + 1; }
#line 1645 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 54:
#line 202 "sql.y" /* yacc.c:1661  */
    { emit("SELECTALL"); (yyval.intval) = 1; }
#line 1651 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 56:
#line 208 "sql.y" /* yacc.c:1661  */
    { (yyval.intval) = 1; }
#line 1657 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 57:
#line 209 "sql.y" /* yacc.c:1661  */
    { (yyval.intval) = (yyvsp[-2].intval) + 1; }
#line 1663 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 58:
#line 213 "sql.y" /* yacc.c:1661  */
    { emit("TABLE %s", (yyvsp[0].strval)); free((yyvsp[0].strval)); }
#line 1669 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 59:
#line 218 "sql.y" /* yacc.c:1661  */
    { emit("STMT"); }
#line 1675 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 60:
#line 221 "sql.y" /* yacc.c:1661  */
    { emit("DROP %s", (yyvsp[0].strval)); free (yyvsp[0].strval); }
#line 1681 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 61:
#line 222 "sql.y" /* yacc.c:1661  */
    { emit("DROPINDEX %s %s", (yyvsp[-2].strval), (yyvsp[0].strval));free (yyvsp[-2].strval); free (yyvsp[0].strval); }
#line 1687 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 62:
#line 227 "sql.y" /* yacc.c:1661  */
    { emit("STMT"); }
#line 1693 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 63:
#line 230 "sql.y" /* yacc.c:1661  */
    { emit("SHOWTABLE"); }
#line 1699 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 64:
#line 231 "sql.y" /* yacc.c:1661  */
    {emit("SHOWSTATUS"); }
#line 1705 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 65:
#line 237 "sql.y" /* yacc.c:1661  */
    { emit("STMT"); }
#line 1711 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 66:
#line 240 "sql.y" /* yacc.c:1661  */
    { emit("EXIT"); }
#line 1717 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 67:
#line 246 "sql.y" /* yacc.c:1661  */
    { emit("NAME %s", (yyvsp[0].strval)); free((yyvsp[0].strval)); }
#line 1723 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 68:
#line 247 "sql.y" /* yacc.c:1661  */
    { emit("STRING %s", (yyvsp[0].strval)); free((yyvsp[0].strval)); }
#line 1729 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 69:
#line 248 "sql.y" /* yacc.c:1661  */
    { emit("NUMBER %d", (yyvsp[0].intval)); }
#line 1735 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 70:
#line 249 "sql.y" /* yacc.c:1661  */
    { emit("FLOAT %g", (yyvsp[0].floatval)); }
#line 1741 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 71:
#line 253 "sql.y" /* yacc.c:1661  */
    { emit("AND"); }
#line 1747 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 72:
#line 254 "sql.y" /* yacc.c:1661  */
    { emit("OR"); }
#line 1753 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 73:
#line 255 "sql.y" /* yacc.c:1661  */
    { emit("CMP %d", (yyvsp[-1].subtok)); }
#line 1759 "sql.tab.c" /* yacc.c:1661  */
    break;

  case 74:
#line 259 "sql.y" /* yacc.c:1661  */
    { emit("BETWEEN"); }
#line 1765 "sql.tab.c" /* yacc.c:1661  */
    break;


#line 1769 "sql.tab.c" /* yacc.c:1661  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 262 "sql.y" /* yacc.c:1906  */


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
