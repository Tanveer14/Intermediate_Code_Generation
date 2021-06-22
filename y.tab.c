/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "1705014.y"

#include<bits/stdc++.h>
#include<iostream>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<string>
#include<sstream>
#include<fstream>
#include<vector>
#include "symboltable.cpp"

using namespace std;

int yyparse(void);
int yylex(void);
extern FILE *yyin;
extern int line_no;
extern int line_count_inc;
extern int error_count;
SymbolTable * table = new SymbolTable(30);
vector<SymbolInfo*> list_of_declared_vars; 
vector<SymbolInfo*> list_of_parameters; 
vector<SymbolInfo*> list_of_arguments;
ofstream logfile;
ofstream errorfile;
string currentFunctionName;

int labelCount=0;
int tempCount=0;

string newLabel()
{
	string lb="L";
	string t=to_string(labelCount);
	labelCount++;
	return lb;
}

string newTemp()
{
	string tmp="t"+to_string(labelCount);
	tempCount++;
	return tmp;
}

void println(string id)
{

}


void yyerror(string text)
{
	error_count++;
	errorfile<<"Error at line "<<line_no<<": "<<text<<endl<<endl;
	logfile<<"Error at line "<<line_no<<": "<<text<<endl<<endl;
}

void printError(string text)
{
	error_count++;
	errorfile<<"Error at line "<<line_no<<": "<<text<<endl<<endl;
	logfile<<"Error at line "<<line_no<<": "<<text<<endl<<endl;
}

void printLogRule(string rule)
{
	logfile<<"Line "<<line_no<<": "<<rule<<endl<<endl;
}


void printLogSymbol(SymbolInfo * symbol)
{
	logfile<<symbol->getName()<<endl<<endl;

}


void endParse()
{
	table->printAllScope(logfile);
	logfile<<endl<<endl<<"Total lines: "<<line_no<<endl<<endl;
	logfile<<"Total errors: "<<error_count<<endl;
	
}

string toUpper(string str)
{
		transform(str.begin(), str.end(), str.begin(), ::toupper);
		return str;
}


void InsertInDeclarationList(SymbolInfo * s)
{
	SymbolInfo * s1=new SymbolInfo();
	s1->makeCopy(s);
	if(table->lookUpCurrentScope(s->getName()) != nullptr)
	{
	printError("Multiple declaration of "+s->getName());
	return;
	}
	list_of_declared_vars.push_back(s1);
}


void InsertDeclaredVarsSymbolTable(string type_specifier)//for var declaration
{
	
	
	for(int i=0;i<list_of_declared_vars.size();i++)
	{
		SymbolInfo * s=new SymbolInfo();
		s->makeCopy(list_of_declared_vars[i]);
		s->setDeclaredType(toUpper(type_specifier));  
		if(type_specifier!="VOID")
		{
			if(!table->Insert(s)){
	    		printError("Multiple declaration of "+s->getName());
	    		}
		}
		else 
		{
		printError("Variable type cannot be void ");
		break;
		}
	    	
	}
	
	 list_of_declared_vars.clear();
	 
}

void InsertInParameterList(SymbolInfo * s)
{
	SymbolInfo * s1=new SymbolInfo();
	s1->makeCopy(s);
	for(int i=0;i<list_of_parameters.size();i++)
	{
		if(s1->getName()!="$" && list_of_parameters[i]->getName()==s1->getName()) 
		{
			printError("Multiple declaration of "+s1->getName()+" in parameter");
			return;
		}
	}
	list_of_parameters.push_back(s1);
}

void InsertParametersSymbolTable()//for function declaration
{
	for(int i=0;i< list_of_parameters.size();i++)
	 {
		SymbolInfo * s=new SymbolInfo();
		s->makeCopy(list_of_parameters[i]);
	   	if(s->getName()!="$") table->Insert(s); 
	    	
	 }
	 //list_of_parameters.clear();
}

void checkParameterError()//for function declaration
{
	for(int i=0;i< list_of_parameters.size();i++)
	 {
		
	   	if(list_of_parameters[i]->getName()=="$")
	   	{
	   		printError(to_string(i+1)+"th parameter's name not given in function definition of "+currentFunctionName);
	   	}
	 
	    	
	 }
	
}

void InsertInArgumentList(SymbolInfo * s)
{
	SymbolInfo * s1=new SymbolInfo();
	s1->makeCopy(s);
	if(s->getDeclaredType()=="VOID") 
	{
	printError("Invalid use of void expression");
	}
	list_of_arguments.push_back(s1);
}




#line 261 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    IF = 258,
    FOR = 259,
    DO = 260,
    INT = 261,
    FLOAT = 262,
    VOID = 263,
    SWITCH = 264,
    DEFAULT = 265,
    ELSE = 266,
    WHILE = 267,
    BREAK = 268,
    CHAR = 269,
    DOUBLE = 270,
    RETURN = 271,
    CASE = 272,
    CONTINUE = 273,
    CONST_INT = 274,
    CONST_FLOAT = 275,
    CONST_CHAR = 276,
    INCOP = 277,
    ADDOP = 278,
    MULOP = 279,
    RELOP = 280,
    ASSIGNOP = 281,
    LOGICOP = 282,
    NOT = 283,
    LPAREN = 284,
    RPAREN = 285,
    LCURL = 286,
    RCURL = 287,
    LTHIRD = 288,
    RTHIRD = 289,
    COMMA = 290,
    SEMICOLON = 291,
    ID = 292,
    STRING = 293,
    COMMENT = 294,
    PRINTLN = 295,
    DECOP = 296,
    WRONG_FLOAT = 297,
    LOWER_THAN_ELSE = 298
  };
#endif
/* Tokens.  */
#define IF 258
#define FOR 259
#define DO 260
#define INT 261
#define FLOAT 262
#define VOID 263
#define SWITCH 264
#define DEFAULT 265
#define ELSE 266
#define WHILE 267
#define BREAK 268
#define CHAR 269
#define DOUBLE 270
#define RETURN 271
#define CASE 272
#define CONTINUE 273
#define CONST_INT 274
#define CONST_FLOAT 275
#define CONST_CHAR 276
#define INCOP 277
#define ADDOP 278
#define MULOP 279
#define RELOP 280
#define ASSIGNOP 281
#define LOGICOP 282
#define NOT 283
#define LPAREN 284
#define RPAREN 285
#define LCURL 286
#define RCURL 287
#define LTHIRD 288
#define RTHIRD 289
#define COMMA 290
#define SEMICOLON 291
#define ID 292
#define STRING 293
#define COMMENT 294
#define PRINTLN 295
#define DECOP 296
#define WRONG_FLOAT 297
#define LOWER_THAN_ELSE 298

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 192 "1705014.y"
SymbolInfo* symbolinfo;

#line 402 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

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
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
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
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   216

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  80
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  134

#define YYUNDEFTOK  2
#define YYMAXUTOK   298


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   209,   209,   217,   223,   232,   240,   245,   252,   252,
     278,   278,   299,   300,   305,   305,   381,   381,   436,   445,
     455,   463,   474,   487,   502,   502,   512,   512,   525,   553,
     558,   563,   571,   578,   586,   594,   602,   612,   618,   623,
     630,   637,   644,   653,   663,   669,   675,   681,   686,   691,
     696,   706,   712,   717,   728,   763,   807,   813,   840,   846,
     866,   872,   892,   898,   927,   933,   989,  1000,  1013,  1022,
    1028,  1090,  1096,  1103,  1110,  1118,  1126,  1138,  1145,  1155,
    1164
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IF", "FOR", "DO", "INT", "FLOAT",
  "VOID", "SWITCH", "DEFAULT", "ELSE", "WHILE", "BREAK", "CHAR", "DOUBLE",
  "RETURN", "CASE", "CONTINUE", "CONST_INT", "CONST_FLOAT", "CONST_CHAR",
  "INCOP", "ADDOP", "MULOP", "RELOP", "ASSIGNOP", "LOGICOP", "NOT",
  "LPAREN", "RPAREN", "LCURL", "RCURL", "LTHIRD", "RTHIRD", "COMMA",
  "SEMICOLON", "ID", "STRING", "COMMENT", "PRINTLN", "DECOP",
  "WRONG_FLOAT", "LOWER_THAN_ELSE", "$accept", "start", "program", "unit",
  "func_declaration", "$@1", "$@2", "func_definition", "$@3", "$@4",
  "parameter_list", "compound_statement", "$@5", "$@6", "var_declaration",
  "type_specifier", "declaration_list", "statements", "statement",
  "expression_statement", "variable", "expression", "logic_expression",
  "rel_expression", "simple_expression", "term", "unary_expression",
  "factor", "argument_list", "arguments", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298
};
# endif

#define YYPACT_NINF (-75)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-27)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      86,   -75,   -75,   -75,    14,    86,   -75,   -75,   -75,   -75,
     -14,   -75,   -75,    50,     4,     2,    17,     0,    26,   -75,
      18,   -17,    15,    39,    33,    49,   -75,    53,    55,    25,
      86,   -75,   -75,   -75,   -75,    71,   -75,    63,   -75,   -75,
      62,    55,    16,    72,   108,    70,   -75,   -75,   -75,   -75,
     -75,   -75,    84,    88,    89,   174,   -75,   -75,   174,   174,
     174,   -75,    52,    90,   -75,   -75,   -75,   -75,   -75,    68,
     -75,   -75,    36,    85,   -75,    80,    78,    98,   -75,   -75,
     -75,   174,   163,   174,    87,    37,   -75,   -75,    95,   174,
     174,    92,   -75,   -75,   -75,   -75,   174,   -75,   -75,   174,
     174,   174,   174,    96,   163,   100,   -75,   -75,   -75,   102,
      99,   101,   103,   -75,   -75,    98,   115,   -75,   148,   174,
     148,   -75,   174,   -75,   104,   130,   112,   -75,   -75,   -75,
     148,   148,   -75,   -75
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    29,    30,    31,     0,     2,     4,     6,     7,     5,
       0,     1,     3,    34,     0,     0,     0,     0,     0,    28,
       0,     0,     0,     0,     0,    32,    13,     0,     0,     0,
       0,    22,    20,    35,    36,     0,    11,    24,    17,    12,
       0,     0,     0,     0,     0,     0,     9,    15,    23,    18,
      33,    40,     0,     0,     0,     0,    72,    73,     0,     0,
       0,    52,    54,     0,    76,    42,    43,    45,    41,     0,
      37,    44,    69,     0,    56,    58,    60,    62,    64,    68,
      27,     0,     0,     0,     0,    69,    66,    67,     0,    78,
       0,     0,    39,    25,    38,    74,     0,    75,    53,     0,
       0,     0,     0,     0,     0,     0,    51,    71,    80,     0,
      77,     0,     0,    57,    59,    63,    61,    65,     0,     0,
       0,    70,     0,    55,     0,    47,     0,    49,    79,    50,
       0,     0,    48,    46
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -75,   -75,   -75,   138,    20,   -75,   -75,    24,   -75,   -75,
     -75,    -7,   -75,   -75,    60,    -3,   -75,   -75,   -63,   -71,
     -58,   -53,   -74,    47,    46,    57,   -55,   -75,   -75,   -75
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,    65,    40,    27,    66,    41,    28,
      21,    67,    44,    45,    68,    10,    14,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,   109,   110
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      85,    85,    84,    86,    87,    17,    94,    88,     1,     2,
       3,   104,    22,    29,    11,   108,    31,    48,    30,    26,
       7,    38,   113,    13,     8,     7,    39,    42,   103,     8,
     105,    85,    20,   119,    47,    24,    23,   111,    85,    18,
      19,    85,    85,    85,    85,   -21,   -19,   117,   128,   -16,
     -21,   -19,    32,    49,   -10,   125,   -14,   127,    95,    95,
       9,    -8,    96,    25,    85,     9,   126,   132,   133,    92,
      34,    52,    53,    33,     1,     2,     3,    97,    97,    15,
      54,    89,    35,    16,    55,    90,    37,    56,    57,    36,
      43,    58,     1,     2,     3,   -26,    59,    60,    46,    37,
      93,   100,    80,   101,    61,    62,    50,    99,    63,    51,
      64,    52,    53,    81,     1,     2,     3,    82,    83,    91,
      54,    98,   102,   106,    55,   107,   118,    56,    57,   112,
     120,    58,   121,   124,   122,   123,    59,    60,   100,    37,
     129,   130,   131,    12,    61,    62,   114,   116,    63,     0,
      64,    52,    53,     0,     1,     2,     3,   115,     0,     0,
      54,     0,     0,     0,    55,     0,     0,    56,    57,     0,
       0,    58,     0,     0,     0,     0,    59,    60,     0,    37,
       0,     0,    56,    57,    61,    62,    58,     0,    63,     0,
      64,    59,    60,    56,    57,     0,     0,    58,     0,    61,
      62,     0,    59,    60,     0,    64,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,    64
};

static const yytype_int16 yycheck[] =
{
      58,    59,    55,    58,    59,     1,    69,    60,     6,     7,
       8,    82,    15,    30,     0,    89,     1,     1,    35,     1,
       0,    28,    96,    37,     0,     5,     1,    30,    81,     5,
      83,    89,    30,   104,    41,    35,    19,    90,    96,    35,
      36,    99,   100,   101,   102,    30,    30,   102,   122,    31,
      35,    35,    37,    37,    36,   118,    31,   120,    22,    22,
       0,    36,    26,    37,   122,     5,   119,   130,   131,     1,
      37,     3,     4,    34,     6,     7,     8,    41,    41,    29,
      12,    29,    33,    33,    16,    33,    31,    19,    20,    36,
      19,    23,     6,     7,     8,    32,    28,    29,    36,    31,
      32,    23,    32,    25,    36,    37,    34,    27,    40,     1,
      42,     3,     4,    29,     6,     7,     8,    29,    29,    29,
      12,    36,    24,    36,    16,    30,    30,    19,    20,    37,
      30,    23,    30,    30,    35,    34,    28,    29,    23,    31,
      36,    11,    30,     5,    36,    37,    99,   101,    40,    -1,
      42,     3,     4,    -1,     6,     7,     8,   100,    -1,    -1,
      12,    -1,    -1,    -1,    16,    -1,    -1,    19,    20,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    28,    29,    -1,    31,
      -1,    -1,    19,    20,    36,    37,    23,    -1,    40,    -1,
      42,    28,    29,    19,    20,    -1,    -1,    23,    -1,    36,
      37,    -1,    28,    29,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    -1,    42
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,     7,     8,    45,    46,    47,    48,    51,    58,
      59,     0,    47,    37,    60,    29,    33,     1,    35,    36,
      30,    54,    59,    19,    35,    37,     1,    50,    53,    30,
      35,     1,    37,    34,    37,    33,    36,    31,    55,     1,
      49,    52,    59,    19,    56,    57,    36,    55,     1,    37,
      34,     1,     3,     4,    12,    16,    19,    20,    23,    28,
      29,    36,    37,    40,    42,    48,    51,    55,    58,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      32,    29,    29,    29,    65,    64,    70,    70,    65,    29,
      33,    29,     1,    32,    62,    22,    26,    41,    36,    27,
      23,    25,    24,    65,    63,    65,    36,    30,    66,    72,
      73,    65,    37,    66,    67,    69,    68,    70,    30,    63,
      30,    30,    35,    34,    30,    62,    65,    62,    66,    36,
      11,    30,    62,    62
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    44,    45,    46,    46,    47,    47,    47,    49,    48,
      50,    48,    48,    48,    52,    51,    53,    51,    54,    54,
      54,    54,    54,    54,    56,    55,    57,    55,    58,    59,
      59,    59,    60,    60,    60,    60,    60,    61,    61,    61,
      61,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    63,    63,    64,    64,    65,    65,    66,    66,
      67,    67,    68,    68,    69,    69,    70,    70,    70,    71,
      71,    71,    71,    71,    71,    71,    71,    72,    72,    73,
      73
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     0,     7,
       0,     6,     6,     5,     0,     7,     0,     6,     4,     3,
       2,     1,     2,     4,     0,     4,     0,     3,     3,     1,
       1,     1,     3,     6,     1,     4,     4,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     7,     5,     7,     5,
       5,     3,     1,     2,     1,     4,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     2,     2,     1,     1,
       4,     3,     1,     1,     2,     2,     1,     1,     0,     3,
       1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
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
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
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
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
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
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

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
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
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
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
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
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
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
          ++yyp;
          ++yyformat;
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
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

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
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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
  case 2:
#line 210 "1705014.y"
        {	
		
		printLogRule("start : program");
		endParse();
	}
#line 1696 "y.tab.c"
    break;

  case 3:
#line 217 "1705014.y"
                       {
			(yyval.symbolinfo)=new SymbolInfo((yyvsp[-1].symbolinfo)->getName()+"\n"+(yyvsp[0].symbolinfo)->getName(),"program");
			printLogRule("program : program unit");
			printLogSymbol((yyval.symbolinfo));
			}
#line 1706 "y.tab.c"
    break;

  case 4:
#line 223 "1705014.y"
               {
		printLogRule("program : unit");
		printLogSymbol((yyvsp[0].symbolinfo));
		(yyval.symbolinfo)= new SymbolInfo();
		(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
		}
#line 1717 "y.tab.c"
    break;

  case 5:
#line 232 "1705014.y"
                       {
			
			printLogRule("unit : var_declaration");
			printLogSymbol((yyvsp[0].symbolinfo));
			(yyval.symbolinfo)= new SymbolInfo();
		      	(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
			}
#line 1729 "y.tab.c"
    break;

  case 6:
#line 240 "1705014.y"
                        {printLogRule("unit : func_declaration");
     			printLogSymbol((yyvsp[0].symbolinfo));
     			(yyval.symbolinfo)= new SymbolInfo();
		      	(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));}
#line 1738 "y.tab.c"
    break;

  case 7:
#line 245 "1705014.y"
                       {printLogRule("unit : func_definition");
     			printLogSymbol((yyvsp[0].symbolinfo));
     			(yyval.symbolinfo)= new SymbolInfo();
		      	(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));}
#line 1747 "y.tab.c"
    break;

  case 8:
#line 252 "1705014.y"
                                                                  {
		SymbolInfo * func=table->lookUp((yyvsp[-3].symbolinfo)->getName());
		if(func!=nullptr)
			{
				printError("Multiple declaration of "+func->getName());
			}
			
		else
			{
				(yyvsp[-3].symbolinfo)->setFunction();
				(yyvsp[-3].symbolinfo)->setDeclaredType(toUpper((yyvsp[-4].symbolinfo)->getName())); 
				SymbolInfo * s= new SymbolInfo();
				s->makeCopy((yyvsp[-3].symbolinfo));
				for(auto var : list_of_parameters)//inserting parameters in function ID
				 {
				   s->insertParameter(var); 
				 }
				table->Insert(s);
			}
		}
#line 1772 "y.tab.c"
    break;

  case 9:
#line 271 "1705014.y"
                            {
		
		list_of_parameters.clear();
		 (yyval.symbolinfo)=new SymbolInfo((yyvsp[-6].symbolinfo)->getName()+" "+(yyvsp[-5].symbolinfo)->getName()+"("+(yyvsp[-3].symbolinfo)->getName()+");","func_declaration");
		 printLogRule("func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
		 printLogSymbol((yyval.symbolinfo));}
#line 1783 "y.tab.c"
    break;

  case 10:
#line 278 "1705014.y"
                                                  {
		SymbolInfo * func=table->lookUp((yyvsp[-2].symbolinfo)->getName());
		if(func!=nullptr)
			{
				printError("Multiple declaration of "+func->getName());
			}
			
		else
			{
				(yyvsp[-2].symbolinfo)->setFunction();
				(yyvsp[-2].symbolinfo)->setDeclaredType(toUpper((yyvsp[-3].symbolinfo)->getName())); 
				SymbolInfo * s= new SymbolInfo();
				s->makeCopy((yyvsp[-2].symbolinfo));
				table->Insert(s);
			}
		}
#line 1804 "y.tab.c"
    break;

  case 11:
#line 293 "1705014.y"
                            {
		
		(yyval.symbolinfo)=new SymbolInfo((yyvsp[-5].symbolinfo)->getName()+" "+(yyvsp[-4].symbolinfo)->getName()+"();","func_declaration");
		printLogRule("func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
		printLogSymbol((yyval.symbolinfo));}
#line 1814 "y.tab.c"
    break;

  case 12:
#line 299 "1705014.y"
                                                                     {}
#line 1820 "y.tab.c"
    break;

  case 13:
#line 300 "1705014.y"
                                                      {}
#line 1826 "y.tab.c"
    break;

  case 14:
#line 305 "1705014.y"
                                                                 {

		SymbolInfo * func=table->lookUp((yyvsp[-3].symbolinfo)->getName());
		currentFunctionName=(yyvsp[-3].symbolinfo)->getName();
		checkParameterError();
		
		if(func!=nullptr)
		{
			if(func->getIsFunction())
			{
				if(func->getIsDefined())
				{
					printError("Multiple definition of function "+func->getName());
				}
				else
				{
					vector<SymbolInfo *> declaredParameterList=func->getParameters();
					if(toUpper((yyvsp[-4].symbolinfo)->getName())!=func->getDeclaredType())
					{
						printError("Return type mismatch with function declaration in function "+func->getName());
					}
					if(list_of_parameters.size()!=declaredParameterList.size())
					{
						printError("Total number of arguments mismatch with declaration in function "+func->getName());
					}
					else
					{
						for(int i=0;i<declaredParameterList.size();i++)
						{	
							//cout<<declaredParameterList[i]->getDeclaredType()<<endl;
							if(declaredParameterList[i]->getDeclaredType()!=list_of_parameters[i]->getDeclaredType())
							{
								printError((i+1)+"th parameter mismatch in function "+func->getName());
								break;
							}
						}
					}
				
				}

			}
			else
			{
				printError("Multiple declaration of "+func->getName());
			}
		}
		
		else{
		(yyvsp[-3].symbolinfo)->setFunction();
		(yyvsp[-3].symbolinfo)->setDeclaredType(toUpper((yyvsp[-4].symbolinfo)->getName()));
		SymbolInfo * s= new SymbolInfo();
		s->makeCopy((yyvsp[-3].symbolinfo));
		for(auto var : list_of_parameters)//inserting parameters in function ID
		 {
		   s->insertParameter(var); 
		 
		 }
		 
		table->Insert(s);		
		}


		
		}
#line 1895 "y.tab.c"
    break;

  case 15:
#line 368 "1705014.y"
                                     {
		//inside compound statement, list of parameters inserted in scope
		list_of_parameters.clear();
		(yyval.symbolinfo)=new SymbolInfo((yyvsp[-6].symbolinfo)->getName()+" "+(yyvsp[-5].symbolinfo)->getName()+"("+(yyvsp[-3].symbolinfo)->getName()+")"+(yyvsp[0].symbolinfo)->getName(),"func_definition");
		printLogRule("func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement");
		SymbolInfo * s= table->lookUp((yyvsp[-5].symbolinfo)->getName());
		if(s!=nullptr && s->getIsFunction())
		{
			s->setIsDefined();
		}
		printLogSymbol((yyval.symbolinfo));
		}
#line 1912 "y.tab.c"
    break;

  case 16:
#line 381 "1705014.y"
                                                  {
		
		SymbolInfo * func=table->lookUp((yyvsp[-2].symbolinfo)->getName());
		currentFunctionName=(yyvsp[-2].symbolinfo)->getName();
		
		if(func!=nullptr)
		{
			if(func->getIsFunction())
			{
				if(func->getIsDefined())
				{
					printError("Multiple definition of function "+func->getName());
				}
				else
				{
					vector<SymbolInfo *> declaredParameterList=func->getParameters();
					if(toUpper((yyvsp[-3].symbolinfo)->getName())!=func->getDeclaredType())
					{
						printError("Function return type didn't match with declaration");
					}
					if(list_of_parameters.size()!=declaredParameterList.size())
					{
						printError("Total number of arguments mismatch with declaration in function "+func->getName());
					}				
				}

			}
			else
			{
				printError("Multiple declaration of "+func->getName());
			}
		}
		
		else{
		(yyvsp[-2].symbolinfo)->setFunction();
		(yyvsp[-2].symbolinfo)->setDeclaredType(toUpper((yyvsp[-3].symbolinfo)->getName())); 
		SymbolInfo * s= new SymbolInfo();
		s->makeCopy((yyvsp[-2].symbolinfo));
		table->Insert(s);
		}
		
		}
#line 1959 "y.tab.c"
    break;

  case 17:
#line 422 "1705014.y"
                                     {		    
		(yyval.symbolinfo)=new SymbolInfo((yyvsp[-5].symbolinfo)->getName()+" "+(yyvsp[-4].symbolinfo)->getName()+"()"+(yyvsp[0].symbolinfo)->getName(),"func_definition");
		printLogRule("func_definition : type_specifier ID LPAREN RPAREN compound_statement");
		printLogSymbol((yyval.symbolinfo));
		SymbolInfo * s= table->lookUp((yyvsp[-4].symbolinfo)->getName());
		if(s!=nullptr && s->getIsFunction())
		{
			s->setIsDefined();
		}
		}
#line 1974 "y.tab.c"
    break;

  case 18:
#line 436 "1705014.y"
                                                         {
		
		(yyvsp[0].symbolinfo)->setDeclaredType(toUpper((yyvsp[-1].symbolinfo)->getName()));
		//list_of_parameters.push_back($4);
		InsertInParameterList((yyvsp[0].symbolinfo));
		(yyval.symbolinfo)=new SymbolInfo((yyvsp[-3].symbolinfo)->getName()+","+(yyvsp[-1].symbolinfo)->getName()+" "+(yyvsp[0].symbolinfo)->getName(),"parameter_list");
		printLogRule("parameter_list  : parameter_list COMMA type_specifier ID");
		printLogSymbol((yyval.symbolinfo));}
#line 1987 "y.tab.c"
    break;

  case 19:
#line 445 "1705014.y"
                                                      {//need to insert in parameter_list
		
		(yyval.symbolinfo)=new SymbolInfo((yyvsp[-2].symbolinfo)->getName()+","+(yyvsp[0].symbolinfo)->getName(),"parameter_list");
		printLogRule("parameter_list  : parameter_list COMMA type_specifier");
		SymbolInfo * s= new SymbolInfo("$","type_specifier");
		s->setDeclaredType(toUpper((yyvsp[0].symbolinfo)->getName())); 
		InsertInParameterList(s);
		printLogSymbol((yyval.symbolinfo));
		}
#line 2001 "y.tab.c"
    break;

  case 20:
#line 455 "1705014.y"
                                    {
		(yyvsp[0].symbolinfo)->setDeclaredType(toUpper((yyvsp[-1].symbolinfo)->getName()));//correction needed
		//list_of_parameters.push_back($2);
		InsertInParameterList((yyvsp[0].symbolinfo));
 		(yyval.symbolinfo)=new SymbolInfo((yyvsp[-1].symbolinfo)->getName()+" "+(yyvsp[0].symbolinfo)->getName(),"parameter_list");
 		printLogRule("parameter_list  : type_specifier ID");
 		printLogSymbol((yyval.symbolinfo));}
#line 2013 "y.tab.c"
    break;

  case 21:
#line 463 "1705014.y"
                                 {//need to insert in parameter_list
		
		printLogRule("parameter_list  : type_specifier");
		SymbolInfo * s= new SymbolInfo("$","type_specifier");
		s->setDeclaredType(toUpper((yyvsp[0].symbolinfo)->getName())); 
		InsertInParameterList(s);
		(yyval.symbolinfo)=new SymbolInfo();
		(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
		printLogSymbol((yyvsp[0].symbolinfo));
		}
#line 2028 "y.tab.c"
    break;

  case 22:
#line 474 "1705014.y"
                                       {
		
		printLogRule("parameter_list  : type_specifier");
		SymbolInfo * s= new SymbolInfo("$","type_specifier");
		s->setDeclaredType(toUpper((yyvsp[-1].symbolinfo)->getName())); 
		InsertInParameterList(s);
		(yyval.symbolinfo)=new SymbolInfo();
		(yyval.symbolinfo)->makeCopy((yyvsp[-1].symbolinfo));
		printLogSymbol((yyvsp[-1].symbolinfo));

		yyclearin;
		}
#line 2045 "y.tab.c"
    break;

  case 23:
#line 487 "1705014.y"
                                                           {
		
		(yyval.symbolinfo)=new SymbolInfo((yyvsp[-3].symbolinfo)->getName()+","+(yyvsp[-1].symbolinfo)->getName(),"parameter_list");
		printLogRule("parameter_list  : parameter_list COMMA type_specifier");
		SymbolInfo * s= new SymbolInfo("$","type_specifier");
		s->setDeclaredType(toUpper((yyvsp[-1].symbolinfo)->getName())); 
		InsertInParameterList(s);
		printLogSymbol((yyval.symbolinfo));
	
		yyclearin;
		}
#line 2061 "y.tab.c"
    break;

  case 24:
#line 502 "1705014.y"
                           {table->EnterScope(logfile);InsertParametersSymbolTable();}
#line 2067 "y.tab.c"
    break;

  case 25:
#line 502 "1705014.y"
                                                                                                        {
		    
 		    (yyval.symbolinfo)=new SymbolInfo("{\n"+(yyvsp[-1].symbolinfo)->getName()+"\n}\n","compound_statement");
 		    printLogRule("compound_statement : LCURL statements RCURL");
 		    printLogSymbol((yyval.symbolinfo));
 		    table->printAllScope(logfile);
	            table->ExitScope(logfile);
	            
 		    }
#line 2081 "y.tab.c"
    break;

  case 26:
#line 512 "1705014.y"
                            {table->EnterScope(logfile);InsertParametersSymbolTable();}
#line 2087 "y.tab.c"
    break;

  case 27:
#line 512 "1705014.y"
                                                                                              {
 		    
 		    (yyval.symbolinfo)=new SymbolInfo("{\n}\n","compound_statement");
 		    printLogRule("compound_statement : LCURL RCURL");
 		    printLogSymbol((yyval.symbolinfo));
 		    table->printAllScope(logfile);
	            table->ExitScope(logfile);

				
	            
 		    }
#line 2103 "y.tab.c"
    break;

  case 28:
#line 525 "1705014.y"
                                                            {
								
								//string str=$1->getName();
								//transform(str.begin(), str.end(), str.begin(), ::toupper);//this may cause problem
								
								(yyval.symbolinfo)=new SymbolInfo((yyvsp[-2].symbolinfo)->getName()+" "+(yyvsp[-1].symbolinfo)->getName()+";","var_declaration");
								printLogRule("var_declaration : type_specifier declaration_list SEMICOLON");
								InsertDeclaredVarsSymbolTable(toUpper((yyvsp[-2].symbolinfo)->getName()));
		
							printLogSymbol((yyval.symbolinfo));
								}
#line 2119 "y.tab.c"
    break;

  case 29:
#line 553 "1705014.y"
                      {
			(yyval.symbolinfo)=new SymbolInfo("int","type_specifier");
			printLogRule("type_specifier : INT");
			printLogSymbol((yyval.symbolinfo));}
#line 2128 "y.tab.c"
    break;

  case 30:
#line 558 "1705014.y"
                        {
			(yyval.symbolinfo)=new SymbolInfo("float","type_specifier");
			printLogRule("type_specifier : FLOAT");
			printLogSymbol((yyval.symbolinfo));}
#line 2137 "y.tab.c"
    break;

  case 31:
#line 563 "1705014.y"
                       {
			(yyval.symbolinfo)=new SymbolInfo("void","type_specifier");
			printLogRule("type_specifier : VOID");
			printLogSymbol((yyval.symbolinfo));}
#line 2146 "y.tab.c"
    break;

  case 32:
#line 571 "1705014.y"
                                             {
						//list_of_declared_vars.push_back($3);
						InsertInDeclarationList((yyvsp[0].symbolinfo));
						(yyval.symbolinfo)=new SymbolInfo((yyvsp[-2].symbolinfo)->getName()+","+(yyvsp[0].symbolinfo)->getName(),"declaration_list");
						printLogRule("declaration_list : declaration_list COMMA ID");
						printLogSymbol((yyval.symbolinfo));}
#line 2157 "y.tab.c"
    break;

  case 33:
#line 578 "1705014.y"
                                                                      {
 		  							(yyvsp[-3].symbolinfo)->setArray();
 		  							//list_of_declared_vars.push_back($3);//array handling
 		  							InsertInDeclarationList((yyvsp[-3].symbolinfo));
 		  							(yyval.symbolinfo)=new SymbolInfo((yyvsp[-5].symbolinfo)->getName()+","+(yyvsp[-3].symbolinfo)->getName()+"["+(yyvsp[-1].symbolinfo)->getName()+"]","declaration_list");
									printLogRule("declaration_list : declaration_list COMMA ID LTHIRD CONST_INT RTHIRD");
									printLogSymbol((yyval.symbolinfo));}
#line 2169 "y.tab.c"
    break;

  case 34:
#line 586 "1705014.y"
                       {
 		  	//list_of_declared_vars.push_back($1);
 		  	InsertInDeclarationList((yyvsp[0].symbolinfo));
 		  	printLogRule(" declaration_list : ID");
 		  	(yyval.symbolinfo)= new SymbolInfo();
		      	(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
		      	printLogSymbol((yyvsp[0].symbolinfo));}
#line 2181 "y.tab.c"
    break;

  case 35:
#line 594 "1705014.y"
                                               {
 		  				(yyvsp[-3].symbolinfo)->setArray();
 		  				//list_of_declared_vars.push_back($1);//array handling
 		  				InsertInDeclarationList((yyvsp[-3].symbolinfo));
 		  				(yyval.symbolinfo)=new SymbolInfo((yyvsp[-3].symbolinfo)->getName()+"["+(yyvsp[-1].symbolinfo)->getName()+"]","declaration_list");
						printLogRule("declaration_list : ID LTHIRD CONST_INT RTHIRD");
						printLogSymbol((yyval.symbolinfo));}
#line 2193 "y.tab.c"
    break;

  case 36:
#line 602 "1705014.y"
                                                   {
						
						
						InsertInDeclarationList((yyvsp[0].symbolinfo));
						(yyval.symbolinfo)=new SymbolInfo((yyvsp[-3].symbolinfo)->getName()+","+(yyvsp[0].symbolinfo)->getName(),"declaration_list");
						printLogRule("declaration_list : declaration_list COMMA ID");
						printLogSymbol((yyval.symbolinfo));
						}
#line 2206 "y.tab.c"
    break;

  case 37:
#line 612 "1705014.y"
                       {
			printLogRule("statements : statement");
			(yyval.symbolinfo)= new SymbolInfo();
		      	(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
		      	printLogSymbol((yyvsp[0].symbolinfo));}
#line 2216 "y.tab.c"
    break;

  case 38:
#line 618 "1705014.y"
                                  {
	   (yyval.symbolinfo)=new SymbolInfo((yyvsp[-1].symbolinfo)->getName()+"\n"+(yyvsp[0].symbolinfo)->getName(),"declaration_list");
	   printLogRule("statements : statements statement");
	   printLogSymbol((yyval.symbolinfo));}
#line 2225 "y.tab.c"
    break;

  case 39:
#line 623 "1705014.y"
                              {
		
		
	
		
	   }
#line 2236 "y.tab.c"
    break;

  case 40:
#line 630 "1705014.y"
                   {
	   	
	   }
#line 2244 "y.tab.c"
    break;

  case 41:
#line 637 "1705014.y"
                            {
				//$$->setName($1->getName());
				printLogRule("statement : var_declaration");
				(yyval.symbolinfo)= new SymbolInfo();
		      		(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
		      		printLogSymbol((yyvsp[0].symbolinfo));
			     }
#line 2256 "y.tab.c"
    break;

  case 42:
#line 644 "1705014.y"
                             {
	  
	  			printLogRule("statement : func_declaration");
				(yyval.symbolinfo)= new SymbolInfo();
		      		(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
		      		printLogSymbol((yyvsp[0].symbolinfo));
		      		printError("A function declared inside a function");
	  
	  			}
#line 2270 "y.tab.c"
    break;

  case 43:
#line 653 "1705014.y"
                            {
	  
	  			printLogRule("statement : func_definition");
				(yyval.symbolinfo)= new SymbolInfo();
		      		(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
		      		printLogSymbol((yyvsp[0].symbolinfo));
		      		printError("A function defined inside a function");
	  
	  			}
#line 2284 "y.tab.c"
    break;

  case 44:
#line 663 "1705014.y"
                                 {
	  			printLogRule("statement : expression_statement");
	  			(yyval.symbolinfo)= new SymbolInfo();
		      		(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
		      		printLogSymbol((yyvsp[0].symbolinfo));}
#line 2294 "y.tab.c"
    break;

  case 45:
#line 669 "1705014.y"
                               {
	  			printLogRule("statement : compound_statement");
	  			(yyval.symbolinfo)= new SymbolInfo();
		      		(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
		      		printLogSymbol((yyvsp[0].symbolinfo));}
#line 2304 "y.tab.c"
    break;

  case 46:
#line 675 "1705014.y"
                                                                                             {
	  (yyval.symbolinfo)=new SymbolInfo("for("+(yyvsp[-4].symbolinfo)->getName()+(yyvsp[-3].symbolinfo)->getName()+(yyvsp[-2].symbolinfo)->getName()+")"+(yyvsp[0].symbolinfo)->getName(),"declaration_list");
	  printLogRule("statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
	  printLogSymbol((yyval.symbolinfo));
	  }
#line 2314 "y.tab.c"
    break;

  case 47:
#line 681 "1705014.y"
                                                                        {
	  (yyval.symbolinfo)=new SymbolInfo("if("+(yyvsp[-2].symbolinfo)->getName()+")"+(yyvsp[0].symbolinfo)->getName(),"declaration_list");
	  printLogRule("statement : IF LPAREN expression RPAREN statement");
	  printLogSymbol((yyval.symbolinfo));}
#line 2323 "y.tab.c"
    break;

  case 48:
#line 686 "1705014.y"
                                                                 {
	  (yyval.symbolinfo)=new SymbolInfo("if("+(yyvsp[-4].symbolinfo)->getName()+")"+(yyvsp[-2].symbolinfo)->getName()+"else\n"+(yyvsp[0].symbolinfo)->getName(),"declaration_list");
	  printLogRule("statement : IF LPAREN expression RPAREN statement ELSE statement");
	  printLogSymbol((yyval.symbolinfo));}
#line 2332 "y.tab.c"
    break;

  case 49:
#line 691 "1705014.y"
                                                     {
	  (yyval.symbolinfo)=new SymbolInfo("while("+(yyvsp[-2].symbolinfo)->getName()+")"+(yyvsp[0].symbolinfo)->getName(),"declaration_list");
	  printLogRule("statement : WHILE LPAREN expression RPAREN statement");
	  printLogSymbol((yyval.symbolinfo));}
#line 2341 "y.tab.c"
    break;

  case 50:
#line 696 "1705014.y"
                                               {
	  (yyval.symbolinfo)=new SymbolInfo("printf("+(yyvsp[-2].symbolinfo)->getName()+");","declaration_list");
	  printLogRule("statement : PRINTLN LPAREN ID RPAREN SEMICOLON");
	  
	  
	  SymbolInfo * s=table->lookUp((yyvsp[-2].symbolinfo)->getName());
	  if(s==nullptr) printError("Undeclared variable "+(yyvsp[-2].symbolinfo)->getName());
	  printLogSymbol((yyval.symbolinfo));
	  }
#line 2355 "y.tab.c"
    break;

  case 51:
#line 706 "1705014.y"
                                        {
	  (yyval.symbolinfo)=new SymbolInfo("return "+(yyvsp[-1].symbolinfo)->getName()+";","declaration_list");
	  printLogRule("statement : RETURN expression SEMICOLON");
	  printLogSymbol((yyval.symbolinfo));}
#line 2364 "y.tab.c"
    break;

  case 52:
#line 712 "1705014.y"
                                    {
			(yyval.symbolinfo)=new SymbolInfo(";","expression_statement");
			printLogRule("expression_statement : SEMICOLON");
			printLogSymbol((yyval.symbolinfo));}
#line 2373 "y.tab.c"
    break;

  case 53:
#line 717 "1705014.y"
                                               {
			(yyval.symbolinfo)=new SymbolInfo((yyvsp[-1].symbolinfo)->getName()+";","expression_statement");
			printLogRule("expression_statement : expression SEMICOLON");
			printLogSymbol((yyval.symbolinfo));}
#line 2382 "y.tab.c"
    break;

  case 54:
#line 728 "1705014.y"
              {
		printLogRule("variable : ID");
		
		SymbolInfo * s=table->lookUp((yyvsp[0].symbolinfo)->getName());
		
		if(s != nullptr)
		{
			(yyval.symbolinfo)= new SymbolInfo();
			(yyval.symbolinfo)->makeCopy(s);
			if(s->getIsFunction())
			{
				printError("Type mismatch, "+s->getName()+" is a function");//if I use a function name in place of a variable
				 (yyval.symbolinfo)->setDeclaredType("UNDECLARED"); 
			}
			
			else if(s->getIsArray())
			{
				printError("Type mismatch, "+s->getName()+" is an array");//if I use an array name in place of a variable
				 (yyval.symbolinfo)->setDeclaredType("UNDECLARED"); 
			}
			
			
		}
		else
		{
			printError("Undeclared variable "+(yyvsp[0].symbolinfo)->getName());
			(yyval.symbolinfo)= new SymbolInfo();
		      	(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
			(yyval.symbolinfo)->setDeclaredType("UNDECLARED");//problem may arise
		}
		
		printLogSymbol((yyvsp[0].symbolinfo));
		
		}
#line 2421 "y.tab.c"
    break;

  case 55:
#line 763 "1705014.y"
                                       {
	 
	 (yyval.symbolinfo)=new SymbolInfo((yyvsp[-3].symbolinfo)->getName()+"["+(yyvsp[-1].symbolinfo)->getName()+"]","variable");
	 printLogRule("variable : ID LTHIRD expression RTHIRD");
	 
	 SymbolInfo * s=table->lookUp((yyvsp[-3].symbolinfo)->getName());
 	
	if(s != nullptr)
	{
		
		if(s->getIsFunction())//if I use function name in place of an array
		{
			printError("Type mismatch, "+s->getName()+" is a function");
		}
			
		else if(s->getIsArray())//correct case
		{
			
			//$$->setDeclaredType(s->getDeclaredType());
			if((yyvsp[-1].symbolinfo)->getDeclaredType()!="INT") {printError("Expression inside third brackets not an integer");}
			(yyval.symbolinfo)->setDeclaredType(s->getDeclaredType());
			
		}
		
		else //if I use a non-array variable in place of an array
		{
			printError(s->getName()+" not an array");//this
		}
	}
	else
	{
		printError("Undeclared variable "+(yyvsp[-3].symbolinfo)->getName());
		(yyval.symbolinfo)= new SymbolInfo();
		(yyval.symbolinfo)->makeCopy((yyvsp[-3].symbolinfo));
		(yyval.symbolinfo)->setDeclaredType("UNDECLARED");//problem may arise
	}
		
	
	printLogSymbol((yyval.symbolinfo));	
	 
	 }
#line 2467 "y.tab.c"
    break;

  case 56:
#line 807 "1705014.y"
                              {
	    printLogRule("expression : logic_expression");
	    (yyval.symbolinfo)= new SymbolInfo();
	    (yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
	    printLogSymbol((yyval.symbolinfo));}
#line 2477 "y.tab.c"
    break;

  case 57:
#line 813 "1705014.y"
                                                {
	   (yyval.symbolinfo)=new SymbolInfo((yyvsp[-2].symbolinfo)->getName()+"="+(yyvsp[0].symbolinfo)->getName(),"expression");
	   printLogRule("expression : variable ASSIGNOP logic_expression");
	   //check type mismatch
	   
	   if((yyvsp[-2].symbolinfo)->getDeclaredType()=="VOID" || (yyvsp[0].symbolinfo)->getDeclaredType()=="VOID")
	  {
	  	printError("Void used in expression ");
	  }
	else if((yyvsp[-2].symbolinfo)->getDeclaredType()=="INT" && (yyvsp[0].symbolinfo)->getDeclaredType()=="FLOAT")
	   {
	   	printError("Type Mismatch");
	   }
	   
	   
	   (yyval.symbolinfo)->setDeclaredType((yyvsp[-2].symbolinfo)->getDeclaredType());
	   
	   printLogSymbol((yyval.symbolinfo));
	   
	   
	   }
#line 2503 "y.tab.c"
    break;

  case 58:
#line 840 "1705014.y"
                                  {
		  printLogRule("logic_expression : rel_expression");
		  (yyval.symbolinfo)= new SymbolInfo();
		  (yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
		  printLogSymbol((yyval.symbolinfo));}
#line 2513 "y.tab.c"
    break;

  case 59:
#line 846 "1705014.y"
                                                         {
		 (yyval.symbolinfo)=new SymbolInfo((yyvsp[-2].symbolinfo)->getName()+(yyvsp[-1].symbolinfo)->getName()+(yyvsp[0].symbolinfo)->getName(),"logic_expression");
		 printLogRule("logic_expression : rel_expression LOGICOP rel_expression");
		 (yyval.symbolinfo)->setDeclaredType("INT");
		 if((yyvsp[-2].symbolinfo)->getDeclaredType()=="VOID" ) 
		  {
		  
		  printError("Void used in expression ");
		  }
		  else if((yyvsp[0].symbolinfo)->getDeclaredType()=="VOID")
		  {
	
		  printError("Void used in expression ");
		  }
		  
		  printLogSymbol((yyval.symbolinfo));
		 }
#line 2535 "y.tab.c"
    break;

  case 60:
#line 866 "1705014.y"
                                   {
		printLogRule("rel_expression : simple_expression");
		(yyval.symbolinfo)= new SymbolInfo();
		(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
		printLogSymbol((yyval.symbolinfo));}
#line 2545 "y.tab.c"
    break;

  case 61:
#line 872 "1705014.y"
                                                                {
		(yyval.symbolinfo)=new SymbolInfo((yyvsp[-2].symbolinfo)->getName()+(yyvsp[-1].symbolinfo)->getName()+(yyvsp[0].symbolinfo)->getName(),"rel_expression");
		printLogRule("rel_expression : simple_expression RELOP simple_expression");
		(yyval.symbolinfo)->setDeclaredType("INT");
		 if((yyvsp[-2].symbolinfo)->getDeclaredType()=="VOID" ) 
		  {
		  
		  printError("Void used in expression ");
		  }
		  else if((yyvsp[0].symbolinfo)->getDeclaredType()=="VOID")
		  {
	
		  printError("Void used in expression ");
		  }
		  
		  printLogSymbol((yyval.symbolinfo));
		}
#line 2567 "y.tab.c"
    break;

  case 62:
#line 892 "1705014.y"
                         {
		   (yyval.symbolinfo)= new SymbolInfo();
		   (yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
		   printLogRule("simple_expression : term");
		   printLogSymbol((yyval.symbolinfo));}
#line 2577 "y.tab.c"
    break;

  case 63:
#line 898 "1705014.y"
                                                 {
		  (yyval.symbolinfo)=new SymbolInfo((yyvsp[-2].symbolinfo)->getName()+(yyvsp[-1].symbolinfo)->getName()+(yyvsp[0].symbolinfo)->getName(),"simple_expression");
		  printLogRule("simple_expression : simple_expression ADDOP term");
		  
		  if((yyvsp[-2].symbolinfo)->getDeclaredType()=="INT" && (yyvsp[0].symbolinfo)->getDeclaredType()=="INT") (yyval.symbolinfo)->setDeclaredType("INT");
		  else if((yyvsp[-2].symbolinfo)->getDeclaredType()=="VOID" ) 
		  {
		  (yyval.symbolinfo)->setDeclaredType((yyvsp[0].symbolinfo)->getDeclaredType());
		  printError("Void used in expression ");
		  }
		  else if((yyvsp[0].symbolinfo)->getDeclaredType()=="VOID")
		  {
		  (yyval.symbolinfo)->setDeclaredType((yyvsp[-2].symbolinfo)->getDeclaredType());
		  printError("Void used in expression ");
		  }
		  else if((yyvsp[-2].symbolinfo)->getDeclaredType()=="UNDECLARED")
		  {
		  	(yyval.symbolinfo)->setDeclaredType((yyvsp[0].symbolinfo)->getDeclaredType());
		  }
		  else if((yyvsp[0].symbolinfo)->getDeclaredType()=="UNDECLARED")
		  {
		  	(yyval.symbolinfo)->setDeclaredType((yyvsp[-2].symbolinfo)->getDeclaredType());
		  }
     		  else if((yyvsp[-2].symbolinfo)->getDeclaredType()=="FLOAT" || (yyvsp[0].symbolinfo)->getDeclaredType()=="FLOAT") (yyval.symbolinfo)->setDeclaredType("FLOAT");
     		  
     		  printLogSymbol((yyval.symbolinfo));
		  }
#line 2609 "y.tab.c"
    break;

  case 64:
#line 927 "1705014.y"
                         {
      printLogRule("term : unary_expression");
      (yyval.symbolinfo)= new SymbolInfo();
      (yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
      printLogSymbol((yyval.symbolinfo));}
#line 2619 "y.tab.c"
    break;

  case 65:
#line 933 "1705014.y"
                                    {
     (yyval.symbolinfo)=new SymbolInfo((yyvsp[-2].symbolinfo)->getName()+(yyvsp[-1].symbolinfo)->getName()+(yyvsp[0].symbolinfo)->getName(),"term");
     printLogRule("term : term MULOP unary_expression");
     
     if((yyvsp[-1].symbolinfo)->getName()=="%")
     {
     	    if((yyvsp[0].symbolinfo)->getName()=="0") printError("Modulus by Zero ");
	    if((yyvsp[-2].symbolinfo)->getDeclaredType()=="VOID" ) 
	     {
		 (yyval.symbolinfo)->setDeclaredType((yyvsp[0].symbolinfo)->getDeclaredType());
		 printError("Void used in expression ");
	     }
	    else if((yyvsp[0].symbolinfo)->getDeclaredType()=="VOID")
	     {
		 (yyval.symbolinfo)->setDeclaredType((yyvsp[-2].symbolinfo)->getDeclaredType());
		 printError("Void used in expression ");
	     }
	  else if((yyvsp[-2].symbolinfo)->getDeclaredType()=="UNDECLARED")
	  {
	  	(yyval.symbolinfo)->setDeclaredType((yyvsp[0].symbolinfo)->getDeclaredType());
	  }
	  else if((yyvsp[0].symbolinfo)->getDeclaredType()=="UNDECLARED")
	  {
	  	(yyval.symbolinfo)->setDeclaredType((yyvsp[-2].symbolinfo)->getDeclaredType());
	  }
	    else if((yyvsp[-2].symbolinfo)->getDeclaredType()!="INT" || (yyvsp[0].symbolinfo)->getDeclaredType()!="INT")
	     {
	     	printError("Non-Integer operand on modulus operator");
	     }
	     (yyval.symbolinfo)->setDeclaredType("INT");
     	
     }
     else
     {
     	if((yyvsp[-1].symbolinfo)->getName()=="/" && (yyvsp[0].symbolinfo)->getName()=="0") printError("Divide by Zero ");
     	if((yyvsp[-2].symbolinfo)->getDeclaredType()=="INT" && (yyvsp[0].symbolinfo)->getDeclaredType()=="INT") (yyval.symbolinfo)->setDeclaredType("INT");
     	else if((yyvsp[-2].symbolinfo)->getDeclaredType()=="VOID" ) 
	  {
	  (yyval.symbolinfo)->setDeclaredType((yyvsp[0].symbolinfo)->getDeclaredType());
	  printError("Void used in expression ");
	  }
	else if((yyvsp[0].symbolinfo)->getDeclaredType()=="VOID")
	  {
	  (yyval.symbolinfo)->setDeclaredType((yyvsp[-2].symbolinfo)->getDeclaredType());
	  printError("Void used in expression ");
	  }
     	else if((yyvsp[-2].symbolinfo)->getDeclaredType()=="FLOAT" || (yyvsp[0].symbolinfo)->getDeclaredType()=="FLOAT") (yyval.symbolinfo)->setDeclaredType("FLOAT");
     }
     
     printLogSymbol((yyval.symbolinfo));
     
     
     }
#line 2677 "y.tab.c"
    break;

  case 66:
#line 989 "1705014.y"
                                           {
		  (yyval.symbolinfo)=new SymbolInfo((yyvsp[-1].symbolinfo)->getName()+(yyvsp[0].symbolinfo)->getName(),"unary_expression");
		  printLogRule("unary_expression : ADDOP unary_expression");
		  (yyval.symbolinfo)->setDeclaredType((yyvsp[0].symbolinfo)->getDeclaredType());
		  if((yyvsp[0].symbolinfo)->getDeclaredType()=="VOID")
		  {
		 	printError("Invalid operation on void type.");
		  }
		  printLogSymbol((yyval.symbolinfo));
		  }
#line 2692 "y.tab.c"
    break;

  case 67:
#line 1000 "1705014.y"
                                        {
		 (yyval.symbolinfo)=new SymbolInfo("!"+(yyvsp[0].symbolinfo)->getName(),"unary_expression");
		 printLogRule("unary_expression : NOT unary_expression");
		 (yyval.symbolinfo)->setDeclaredType("INT");
		 
		 if((yyvsp[0].symbolinfo)->getDeclaredType()=="VOID")
		 {
		 	printError("Invalid operation on void type.");
		 }
		 
		 printLogSymbol((yyval.symbolinfo));
		 }
#line 2709 "y.tab.c"
    break;

  case 68:
#line 1013 "1705014.y"
                          {
		 printLogRule("unary_expression : factor");
		 (yyval.symbolinfo)= new SymbolInfo();
		 (yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
		 
		 printLogSymbol((yyval.symbolinfo));}
#line 2720 "y.tab.c"
    break;

  case 69:
#line 1022 "1705014.y"
                   {
	printLogRule("factor: variable");
	(yyval.symbolinfo)= new SymbolInfo();
	(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
	printLogSymbol((yyval.symbolinfo));}
#line 2730 "y.tab.c"
    break;

  case 70:
#line 1028 "1705014.y"
                                         {//function call
	//get the function variable from symbolTable
	
	(yyval.symbolinfo)=new SymbolInfo((yyvsp[-3].symbolinfo)->getName()+"("+(yyvsp[-1].symbolinfo)->getName()+")","factor");
	printLogRule("factor : ID LPAREN argument_list RPAREN");
	
	SymbolInfo * s= table->lookUp((yyvsp[-3].symbolinfo)->getName());
	if(s!=nullptr)
	{
		if(s->getIsFunction())
		{
			(yyval.symbolinfo)->setFunction();
			(yyval.symbolinfo)->setDeclaredType(s->getDeclaredType());
			vector<SymbolInfo*> parameters= s->getParameters();
			
			if(!s->getIsDefined())
			{
				printError("Undefined reference to '"+s->getName()+"'");
			}
			
			if(parameters.size()==list_of_arguments.size())
			{
				for(int i=0;i<list_of_arguments.size();i++)
				{
					if(list_of_arguments[i]->getDeclaredType()=="FLOAT" && parameters[i]->getDeclaredType()=="INT")
					{
						
						printError(to_string(i+1)+"th argument mismatch in function "+s->getName());
						break;
						//cout<<list_of_arguments[i]->getDeclaredType()<<" "<<parameters[i]->getDeclaredType()<<endl;
						
					}
				}
			}
			else
			{
				printError("Total number of arguments mismatch in function "+s->getName());
				
				
				
			}
			(yyval.symbolinfo)->setDeclaredType(s->getDeclaredType());
		}
		else
		{
			printError(s->getName()+" not a function");
			(yyval.symbolinfo)->setDeclaredType("UNDECLARED");
		}
		
		//$$->setDeclaredType(s->getDeclaredType());
	}
	else 
	{
		printError("Undeclared function "+(yyvsp[-3].symbolinfo)->getName());
		(yyval.symbolinfo)->setDeclaredType("UNDECLARED");
	}
	
	list_of_arguments.clear();
	printLogSymbol((yyval.symbolinfo));
	;}
#line 2795 "y.tab.c"
    break;

  case 71:
#line 1090 "1705014.y"
                                   {
	(yyval.symbolinfo)=new SymbolInfo("("+(yyvsp[-1].symbolinfo)->getName()+")","factor");
	printLogRule("factor : LPAREN expression RPAREN");
	(yyval.symbolinfo)->setDeclaredType((yyvsp[-1].symbolinfo)->getDeclaredType());
	printLogSymbol((yyval.symbolinfo));}
#line 2805 "y.tab.c"
    break;

  case 72:
#line 1096 "1705014.y"
                    {
	(yyvsp[0].symbolinfo)->setDeclaredType("INT");
	printLogRule("factor : CONST_INT");
	(yyval.symbolinfo)= new SymbolInfo();
	(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
	printLogSymbol((yyval.symbolinfo));}
#line 2816 "y.tab.c"
    break;

  case 73:
#line 1103 "1705014.y"
                      {
	(yyvsp[0].symbolinfo)->setDeclaredType("FLOAT");
	printLogRule("factor : CONST_FLOAT");
	(yyval.symbolinfo)= new SymbolInfo();
	(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
	printLogSymbol((yyval.symbolinfo));}
#line 2827 "y.tab.c"
    break;

  case 74:
#line 1110 "1705014.y"
                         {
	(yyval.symbolinfo)=new SymbolInfo((yyvsp[-1].symbolinfo)->getName()+"++","factor");
	printLogRule("factor : variable INCOP");
	
	(yyval.symbolinfo)->setDeclaredType((yyvsp[-1].symbolinfo)->getDeclaredType());
	printLogSymbol((yyval.symbolinfo));
	}
#line 2839 "y.tab.c"
    break;

  case 75:
#line 1118 "1705014.y"
                         {
	(yyval.symbolinfo)=new SymbolInfo((yyvsp[-1].symbolinfo)->getName()+"--","factor");
	printLogRule("factor : variable DECOP");
	
	(yyval.symbolinfo)->setDeclaredType((yyvsp[-1].symbolinfo)->getDeclaredType());
	printLogSymbol((yyval.symbolinfo));}
#line 2850 "y.tab.c"
    break;

  case 76:
#line 1126 "1705014.y"
                      {
	(yyvsp[0].symbolinfo)->setDeclaredType("WRONG_FLOAT");
	printLogRule("factor : WRONG_FLOAT");
	(yyval.symbolinfo)= new SymbolInfo();
	(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
	printLogSymbol((yyval.symbolinfo));
	
	}
#line 2863 "y.tab.c"
    break;

  case 77:
#line 1138 "1705014.y"
                         {
			  printLogRule("argument_list : arguments");
		      	(yyval.symbolinfo)= new SymbolInfo();
		      	(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));			  
			printLogSymbol((yyval.symbolinfo)); 
			  }
#line 2874 "y.tab.c"
    break;

  case 78:
#line 1145 "1705014.y"
                           {
			  SymbolInfo * s=new SymbolInfo("","argument_list");
			  printLogRule("argument_list : ");
			  
			  (yyval.symbolinfo)=s;
			  printLogSymbol((yyval.symbolinfo));
			  }
#line 2886 "y.tab.c"
    break;

  case 79:
#line 1155 "1705014.y"
                                             {
		(yyval.symbolinfo)=new SymbolInfo((yyvsp[-2].symbolinfo)->getName()+","+(yyvsp[0].symbolinfo)->getName(),"arguments");
		printLogRule("arguments : arguments COMMA logic_expression");
		//list_of_arguments.push_back($3);
		InsertInArgumentList((yyvsp[0].symbolinfo));
		printLogSymbol((yyval.symbolinfo));
		
		}
#line 2899 "y.tab.c"
    break;

  case 80:
#line 1164 "1705014.y"
                                 {
	      	printLogRule("arguments : logic_expression");
	      	(yyval.symbolinfo)= new SymbolInfo();
	      	(yyval.symbolinfo)->makeCopy((yyvsp[0].symbolinfo));
	      	InsertInArgumentList((yyvsp[0].symbolinfo));
	      	printLogSymbol((yyval.symbolinfo));
	      	//list_of_arguments.push_back();
	      	}
#line 2912 "y.tab.c"
    break;


#line 2916 "y.tab.c"

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
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

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
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
                  yystos[+*yyssp], yyvsp);
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
#line 1176 "1705014.y"

int main(int argc,char *argv[])
{

	if(argc<2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}
	
	
	
	
	string logfilename="log.txt";
	string errorfilename="error.txt";
	
	
	logfile.open(logfilename);
	errorfile.open(errorfilename);
	yyin=fin;
	
	yyparse();
	
	logfile.close();
	errorfile.close();
	
	fclose(fin);
	
	

	return 0;
}

