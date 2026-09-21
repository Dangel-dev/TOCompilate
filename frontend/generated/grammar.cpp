/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "../ast/ast.hpp"

extern int yylex();
extern int yylineno;
extern char* yytext;
void yyerror(const char* s);

extern to_lang::ProgramNodePtr g_program_root;
extern std::string g_parse_error;
extern int g_error_line;

#line 88 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"

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

#include "grammar.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT_LITERAL = 3,                /* INT_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 4,               /* CHAR_LITERAL  */
  YYSYMBOL_IDENTIFIER = 5,                 /* IDENTIFIER  */
  YYSYMBOL_STRING_LITERAL = 6,             /* STRING_LITERAL  */
  YYSYMBOL_KW_I32 = 7,                     /* KW_I32  */
  YYSYMBOL_KW_I64 = 8,                     /* KW_I64  */
  YYSYMBOL_KW_BOOL = 9,                    /* KW_BOOL  */
  YYSYMBOL_KW_CHAR = 10,                   /* KW_CHAR  */
  YYSYMBOL_KW_VOID = 11,                   /* KW_VOID  */
  YYSYMBOL_KW_PTR = 12,                    /* KW_PTR  */
  YYSYMBOL_KW_IF = 13,                     /* KW_IF  */
  YYSYMBOL_KW_ELIF = 14,                   /* KW_ELIF  */
  YYSYMBOL_KW_ELSE = 15,                   /* KW_ELSE  */
  YYSYMBOL_KW_WHILE = 16,                  /* KW_WHILE  */
  YYSYMBOL_KW_RETURN = 17,                 /* KW_RETURN  */
  YYSYMBOL_KW_TRUE = 18,                   /* KW_TRUE  */
  YYSYMBOL_KW_FALSE = 19,                  /* KW_FALSE  */
  YYSYMBOL_OP_ADD_ASSIGN = 20,             /* OP_ADD_ASSIGN  */
  YYSYMBOL_OP_SUB_ASSIGN = 21,             /* OP_SUB_ASSIGN  */
  YYSYMBOL_OP_MUL_ASSIGN = 22,             /* OP_MUL_ASSIGN  */
  YYSYMBOL_OP_DIV_ASSIGN = 23,             /* OP_DIV_ASSIGN  */
  YYSYMBOL_OP_MOD_ASSIGN = 24,             /* OP_MOD_ASSIGN  */
  YYSYMBOL_OP_EQ = 25,                     /* OP_EQ  */
  YYSYMBOL_OP_NEQ = 26,                    /* OP_NEQ  */
  YYSYMBOL_OP_LE = 27,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 28,                     /* OP_GE  */
  YYSYMBOL_OP_AND = 29,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 30,                     /* OP_OR  */
  YYSYMBOL_OP_SHL = 31,                    /* OP_SHL  */
  YYSYMBOL_OP_SHR = 32,                    /* OP_SHR  */
  YYSYMBOL_OP_BACKSLASH = 33,              /* OP_BACKSLASH  */
  YYSYMBOL_ERROR_TOKEN = 34,               /* ERROR_TOKEN  */
  YYSYMBOL_35_ = 35,                       /* '='  */
  YYSYMBOL_36_ = 36,                       /* '|'  */
  YYSYMBOL_37_ = 37,                       /* '^'  */
  YYSYMBOL_38_ = 38,                       /* '&'  */
  YYSYMBOL_39_ = 39,                       /* '<'  */
  YYSYMBOL_40_ = 40,                       /* '>'  */
  YYSYMBOL_41_ = 41,                       /* '+'  */
  YYSYMBOL_42_ = 42,                       /* '-'  */
  YYSYMBOL_43_ = 43,                       /* '*'  */
  YYSYMBOL_44_ = 44,                       /* '/'  */
  YYSYMBOL_45_ = 45,                       /* '%'  */
  YYSYMBOL_UNARY = 46,                     /* UNARY  */
  YYSYMBOL_47_ = 47,                       /* '['  */
  YYSYMBOL_48_ = 48,                       /* '('  */
  YYSYMBOL_49_ = 49,                       /* ')'  */
  YYSYMBOL_50_ = 50,                       /* ','  */
  YYSYMBOL_51_ = 51,                       /* ']'  */
  YYSYMBOL_52_ = 52,                       /* '{'  */
  YYSYMBOL_53_ = 53,                       /* '}'  */
  YYSYMBOL_54_ = 54,                       /* ';'  */
  YYSYMBOL_55_ = 55,                       /* '!'  */
  YYSYMBOL_56_ = 56,                       /* '~'  */
  YYSYMBOL_YYACCEPT = 57,                  /* $accept  */
  YYSYMBOL_program = 58,                   /* program  */
  YYSYMBOL_function_list = 59,             /* function_list  */
  YYSYMBOL_function_decl = 60,             /* function_decl  */
  YYSYMBOL_opt_param_list = 61,            /* opt_param_list  */
  YYSYMBOL_param_list = 62,                /* param_list  */
  YYSYMBOL_param = 63,                     /* param  */
  YYSYMBOL_type = 64,                      /* type  */
  YYSYMBOL_base_type = 65,                 /* base_type  */
  YYSYMBOL_block = 66,                     /* block  */
  YYSYMBOL_statement_list = 67,            /* statement_list  */
  YYSYMBOL_statement = 68,                 /* statement  */
  YYSYMBOL_var_decl = 69,                  /* var_decl  */
  YYSYMBOL_if_stmt = 70,                   /* if_stmt  */
  YYSYMBOL_elif_list = 71,                 /* elif_list  */
  YYSYMBOL_opt_else = 72,                  /* opt_else  */
  YYSYMBOL_while_stmt = 73,                /* while_stmt  */
  YYSYMBOL_return_stmt = 74,               /* return_stmt  */
  YYSYMBOL_expr = 75,                      /* expr  */
  YYSYMBOL_opt_arg_list = 76,              /* opt_arg_list  */
  YYSYMBOL_arg_list = 77,                  /* arg_list  */
  YYSYMBOL_primary_expr = 78               /* primary_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   496

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  81
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  147

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   290


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    55,     2,     2,     2,    45,    38,     2,
      48,    49,    43,    41,    50,    42,     2,    44,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    54,
      39,    35,    40,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    47,     2,    51,    37,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    52,    36,    53,    56,     2,     2,     2,
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
      46
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    83,    83,    90,    93,   100,   114,   117,   123,   128,
     136,   140,   147,   150,   153,   159,   160,   161,   162,   163,
     167,   174,   177,   184,   185,   186,   187,   188,   189,   190,
     194,   198,   205,   218,   221,   231,   234,   240,   246,   249,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   286,
     291,   297,   300,   306,   310,   317,   321,   324,   327,   331,
     334,   337
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INT_LITERAL",
  "CHAR_LITERAL", "IDENTIFIER", "STRING_LITERAL", "KW_I32", "KW_I64",
  "KW_BOOL", "KW_CHAR", "KW_VOID", "KW_PTR", "KW_IF", "KW_ELIF", "KW_ELSE",
  "KW_WHILE", "KW_RETURN", "KW_TRUE", "KW_FALSE", "OP_ADD_ASSIGN",
  "OP_SUB_ASSIGN", "OP_MUL_ASSIGN", "OP_DIV_ASSIGN", "OP_MOD_ASSIGN",
  "OP_EQ", "OP_NEQ", "OP_LE", "OP_GE", "OP_AND", "OP_OR", "OP_SHL",
  "OP_SHR", "OP_BACKSLASH", "ERROR_TOKEN", "'='", "'|'", "'^'", "'&'",
  "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'%'", "UNARY", "'['", "'('",
  "')'", "','", "']'", "'{'", "'}'", "';'", "'!'", "'~'", "$accept",
  "program", "function_list", "function_decl", "opt_param_list",
  "param_list", "param", "type", "base_type", "block", "statement_list",
  "statement", "var_decl", "if_stmt", "elif_list", "opt_else",
  "while_stmt", "return_stmt", "expr", "opt_arg_list", "arg_list",
  "primary_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-40)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -40,     3,    16,   -40,   -40,   -40,   -40,   -40,   -40,    16,
      13,   -40,    24,   -40,   -40,   -33,   -29,    16,    16,   -40,
     -18,    10,   -40,    -3,    41,    16,   -40,    29,   -40,   -40,
     -40,    89,    62,   -40,   -40,   -40,    47,   -40,    48,    53,
      79,   -40,   -40,    79,    79,    79,   -40,   -40,    79,    79,
      97,   -40,   -40,    52,   -40,   -40,    57,   116,   -40,    79,
      79,    79,   313,    60,    60,   193,    60,    60,    77,   -40,
     -40,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,   -40,   313,    70,    73,
     223,   253,   -40,    79,   313,   313,   313,   313,   313,   449,
     449,    45,    45,   359,   336,   -30,   -30,   313,   382,   405,
     428,    45,    45,    14,    14,    60,    60,    60,   151,   -40,
      79,    41,    41,   313,   -40,   313,   -40,   -40,    -7,    72,
      41,   -40,    79,   -40,   283,    41,   -40
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     2,     1,    15,    16,    17,    18,    19,     0,
       0,     4,     0,    12,    13,     0,     0,     0,     6,    14,
       0,     7,     8,     0,     0,     0,    10,     0,    21,     5,
       9,     0,     0,    11,    76,    77,    75,    78,     0,     0,
      38,    79,    80,     0,     0,     0,    20,    29,     0,     0,
       0,    28,    22,     0,    24,    25,     0,     0,    70,    71,
       0,     0,    39,    64,    67,     0,    65,    66,    30,    23,
      26,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    27,    73,     0,    72,
       0,     0,    81,     0,    41,    42,    43,    44,    45,    51,
      52,    54,    56,    57,    58,    62,    63,    40,    60,    61,
      59,    53,    55,    46,    47,    48,    49,    50,     0,    69,
       0,     0,     0,    31,    68,    74,    33,    37,    35,     0,
       0,    32,     0,    36,     0,     0,    34
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -40,   -40,   -40,   -40,   -40,   -40,    99,   195,   -40,   -32,
     -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -39,   -40,
     -40,   -40
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    11,    20,    21,    22,    23,    13,    29,
      32,    52,    53,    54,   138,   141,    55,    56,    57,    98,
      99,    58
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      51,    62,    26,     3,    63,    64,    65,   139,   140,    66,
      67,    90,    91,    92,    93,    94,    15,    95,    17,    18,
      97,   100,   101,     4,     5,     6,     7,     8,     9,    16,
      27,    24,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    92,    93,    94,
      25,    95,    31,    10,   133,    34,    35,    36,    37,     4,
       5,     6,     7,     8,     9,    38,    82,    83,    39,    40,
      41,    42,    34,    35,    36,    37,    90,    91,    92,    93,
      94,   135,    95,    28,    33,    59,    60,    41,    42,   136,
     137,    61,    68,   144,    43,    44,    69,    95,   143,    10,
      45,    70,   103,   146,    28,    46,    47,    48,    49,   129,
     142,    43,    44,   130,    30,     0,     0,    45,     0,     0,
       0,     0,     0,     0,    48,    49,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,     0,
       0,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,     0,    95,     0,     0,     0,     0,     0,     0,
      96,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,     0,     0,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    12,    95,     0,
       0,     0,   134,     0,    14,     0,     0,     0,     0,     0,
       0,     0,    19,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,     0,    50,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,     0,
      95,     0,   102,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,     0,     0,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,     0,
      95,     0,   131,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,     0,     0,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,     0,
      95,     0,   132,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,     0,     0,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,     0,
      95,     0,   145,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,     0,     0,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,     0,
      95,    76,    77,    78,    79,    80,     0,    82,    83,     0,
       0,     0,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,     0,    95,    76,    77,    78,    79,     0,     0,
      82,    83,     0,     0,     0,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,     0,    95,    76,    77,    78,
      79,     0,     0,    82,    83,     0,     0,     0,     0,    86,
      87,    88,    89,    90,    91,    92,    93,    94,     0,    95,
      76,    77,    78,    79,     0,     0,    82,    83,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,    92,    93,
      94,     0,    95,    76,    77,    78,    79,     0,     0,    82,
      83,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,    93,    94,     0,    95,    78,    79,     0,     0,
      82,    83,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,    93,    94,     0,    95
};

static const yytype_int16 yycheck[] =
{
      32,    40,     5,     0,    43,    44,    45,    14,    15,    48,
      49,    41,    42,    43,    44,    45,     3,    47,    51,    48,
      59,    60,    61,     7,     8,     9,    10,    11,    12,     5,
      33,    49,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    43,    44,    45,
      50,    47,    33,    47,   103,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    31,    32,    16,    17,
      18,    19,     3,     4,     5,     6,    41,    42,    43,    44,
      45,   130,    47,    52,     5,    48,    48,    18,    19,   131,
     132,    48,     5,   142,    42,    43,    54,    47,   140,    47,
      48,    54,    35,   145,    52,    53,    54,    55,    56,    49,
      48,    42,    43,    50,    25,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    56,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,     2,    47,    -1,
      -1,    -1,    51,    -1,     9,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    32,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    -1,    49,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    -1,    49,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    -1,    49,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    -1,    49,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    25,    26,    27,    28,    29,    -1,    31,    32,    -1,
      -1,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    25,    26,    27,    28,    -1,    -1,
      31,    32,    -1,    -1,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    25,    26,    27,
      28,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      25,    26,    27,    28,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    25,    26,    27,    28,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    27,    28,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    39,    40,
      41,    42,    43,    44,    45,    -1,    47
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    58,    59,     0,     7,     8,     9,    10,    11,    12,
      47,    60,    64,    65,    64,     3,     5,    51,    48,    64,
      61,    62,    63,    64,    49,    50,     5,    33,    52,    66,
      63,    33,    67,     5,     3,     4,     5,     6,    13,    16,
      17,    18,    19,    42,    43,    48,    53,    54,    55,    56,
      64,    66,    68,    69,    70,    73,    74,    75,    78,    48,
      48,    48,    75,    75,    75,    75,    75,    75,     5,    54,
      54,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    47,    54,    75,    76,    77,
      75,    75,    49,    35,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    49,
      50,    49,    49,    75,    51,    75,    66,    66,    71,    14,
      15,    72,    48,    66,    75,    49,    66
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    57,    58,    59,    59,    60,    61,    61,    62,    62,
      63,    63,    64,    64,    64,    65,    65,    65,    65,    65,
      66,    67,    67,    68,    68,    68,    68,    68,    68,    68,
      69,    69,    70,    71,    71,    72,    72,    73,    74,    74,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    76,    76,    77,    77,    78,    78,    78,    78,    78,
      78,    78
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     6,     0,     1,     1,     3,
       2,     4,     1,     2,     4,     1,     1,     1,     1,     1,
       3,     0,     2,     2,     1,     1,     2,     2,     1,     1,
       2,     4,     7,     0,     6,     0,     2,     5,     1,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     4,     4,
       1,     0,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

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
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  case 2: /* program: function_list  */
#line 83 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                  {
        g_program_root = std::make_shared<to_lang::ProgramNode>(*(yyvsp[0].func_list), yylineno, 1);
        delete (yyvsp[0].func_list);
    }
#line 1326 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 3: /* function_list: %empty  */
#line 90 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                {
        (yyval.func_list) = new std::vector<to_lang::FunctionDeclPtr>();
    }
#line 1334 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 4: /* function_list: function_list function_decl  */
#line 93 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                                  {
        (yyval.func_list) = (yyvsp[-1].func_list);
        if ((yyvsp[0].func_val)) (yyval.func_list)->push_back(to_lang::FunctionDeclPtr((yyvsp[0].func_val)));
    }
#line 1343 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 5: /* function_decl: type IDENTIFIER '(' opt_param_list ')' block  */
#line 100 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                                                 {
        (yyval.func_val) = new to_lang::FunctionDecl(
            to_lang::TypeNodePtr((yyvsp[-5].type_val)),
            (yyvsp[-4].str_val),
            *(yyvsp[-2].param_list),
            to_lang::BlockStmtPtr((yyvsp[0].block_val)),
            yylineno, 1
        );
        free((yyvsp[-4].str_val));
        delete (yyvsp[-2].param_list);
    }
#line 1359 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 6: /* opt_param_list: %empty  */
#line 114 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                {
        (yyval.param_list) = new std::vector<to_lang::ParamDef>();
    }
#line 1367 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 7: /* opt_param_list: param_list  */
#line 117 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                 {
        (yyval.param_list) = (yyvsp[0].param_list);
    }
#line 1375 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 8: /* param_list: param  */
#line 123 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
          {
        (yyval.param_list) = new std::vector<to_lang::ParamDef>();
        (yyval.param_list)->push_back(*(yyvsp[0].param_val));
        delete (yyvsp[0].param_val);
    }
#line 1385 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 9: /* param_list: param_list ',' param  */
#line 128 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                           {
        (yyval.param_list) = (yyvsp[-2].param_list);
        (yyval.param_list)->push_back(*(yyvsp[0].param_val));
        delete (yyvsp[0].param_val);
    }
#line 1395 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 10: /* param: type IDENTIFIER  */
#line 136 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                    {
        (yyval.param_val) = new to_lang::ParamDef{ to_lang::TypeNodePtr((yyvsp[-1].type_val)), (yyvsp[0].str_val) };
        free((yyvsp[0].str_val));
    }
#line 1404 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 11: /* param: type OP_BACKSLASH OP_BACKSLASH IDENTIFIER  */
#line 140 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                                                {
        (yyval.param_val) = new to_lang::ParamDef{ to_lang::TypeNodePtr((yyvsp[-3].type_val)), (yyvsp[0].str_val) };
        free((yyvsp[0].str_val));
    }
#line 1413 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 12: /* type: base_type  */
#line 147 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
              {
        (yyval.type_val) = (yyvsp[0].type_val);
    }
#line 1421 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 13: /* type: KW_PTR type  */
#line 150 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                  {
        (yyval.type_val) = new to_lang::TypeNode(to_lang::TypeNode::Kind::POINTER, to_lang::TypeNodePtr((yyvsp[0].type_val)));
    }
#line 1429 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 14: /* type: '[' INT_LITERAL ']' type  */
#line 153 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                               {
        (yyval.type_val) = new to_lang::TypeNode(to_lang::TypeNode::Kind::ARRAY, to_lang::TypeNodePtr((yyvsp[0].type_val)), (yyvsp[-2].int_val));
    }
#line 1437 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 15: /* base_type: KW_I32  */
#line 159 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
            { (yyval.type_val) = new to_lang::TypeNode(to_lang::BaseTypeKind::I32); }
#line 1443 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 16: /* base_type: KW_I64  */
#line 160 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
              { (yyval.type_val) = new to_lang::TypeNode(to_lang::BaseTypeKind::I64); }
#line 1449 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 17: /* base_type: KW_BOOL  */
#line 161 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
              { (yyval.type_val) = new to_lang::TypeNode(to_lang::BaseTypeKind::BOOL); }
#line 1455 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 18: /* base_type: KW_CHAR  */
#line 162 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
              { (yyval.type_val) = new to_lang::TypeNode(to_lang::BaseTypeKind::CHAR); }
#line 1461 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 19: /* base_type: KW_VOID  */
#line 163 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
              { (yyval.type_val) = new to_lang::TypeNode(to_lang::BaseTypeKind::VOID); }
#line 1467 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 20: /* block: '{' statement_list '}'  */
#line 167 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                           {
        (yyval.block_val) = new to_lang::BlockStmt(*(yyvsp[-1].stmt_list), yylineno, 1);
        delete (yyvsp[-1].stmt_list);
    }
#line 1476 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 21: /* statement_list: %empty  */
#line 174 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                {
        (yyval.stmt_list) = new std::vector<to_lang::StmtNodePtr>();
    }
#line 1484 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 22: /* statement_list: statement_list statement  */
#line 177 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                               {
        (yyval.stmt_list) = (yyvsp[-1].stmt_list);
        if ((yyvsp[0].stmt_val)) (yyval.stmt_list)->push_back(to_lang::StmtNodePtr((yyvsp[0].stmt_val)));
    }
#line 1493 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 23: /* statement: var_decl ';'  */
#line 184 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                       { (yyval.stmt_val) = (yyvsp[-1].vardecl_val); }
#line 1499 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 24: /* statement: if_stmt  */
#line 185 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                        { (yyval.stmt_val) = (yyvsp[0].if_val); }
#line 1505 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 25: /* statement: while_stmt  */
#line 186 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                        { (yyval.stmt_val) = (yyvsp[0].while_val); }
#line 1511 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 26: /* statement: return_stmt ';'  */
#line 187 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                        { (yyval.stmt_val) = (yyvsp[-1].return_val); }
#line 1517 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 27: /* statement: expr ';'  */
#line 188 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                        { (yyval.stmt_val) = new to_lang::ExprStmt(to_lang::ExprNodePtr((yyvsp[-1].expr_val)), yylineno, 1); }
#line 1523 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 28: /* statement: block  */
#line 189 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                        { (yyval.stmt_val) = (yyvsp[0].block_val); }
#line 1529 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 29: /* statement: ';'  */
#line 190 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                        { (yyval.stmt_val) = nullptr; }
#line 1535 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 30: /* var_decl: type IDENTIFIER  */
#line 194 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                    {
        (yyval.vardecl_val) = new to_lang::VarDeclStmt(to_lang::TypeNodePtr((yyvsp[-1].type_val)), (yyvsp[0].str_val), nullptr, yylineno, 1);
        free((yyvsp[0].str_val));
    }
#line 1544 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 31: /* var_decl: type IDENTIFIER '=' expr  */
#line 198 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                               {
        (yyval.vardecl_val) = new to_lang::VarDeclStmt(to_lang::TypeNodePtr((yyvsp[-3].type_val)), (yyvsp[-2].str_val), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1);
        free((yyvsp[-2].str_val));
    }
#line 1553 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 32: /* if_stmt: KW_IF '(' expr ')' block elif_list opt_else  */
#line 205 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                                                {
        (yyval.if_val) = new to_lang::IfStmt(
            to_lang::ExprNodePtr((yyvsp[-4].expr_val)),
            to_lang::BlockStmtPtr((yyvsp[-2].block_val)),
            *(yyvsp[-1].elif_list),
            to_lang::BlockStmtPtr((yyvsp[0].block_val)),
            yylineno, 1
        );
        delete (yyvsp[-1].elif_list);
    }
#line 1568 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 33: /* elif_list: %empty  */
#line 218 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                {
        (yyval.elif_list) = new std::vector<to_lang::ElifBranch>();
    }
#line 1576 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 34: /* elif_list: elif_list KW_ELIF '(' expr ')' block  */
#line 221 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                                           {
        (yyval.elif_list) = (yyvsp[-5].elif_list);
        (yyval.elif_list)->push_back(to_lang::ElifBranch{
            to_lang::ExprNodePtr((yyvsp[-2].expr_val)),
            to_lang::BlockStmtPtr((yyvsp[0].block_val))
        });
    }
#line 1588 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 35: /* opt_else: %empty  */
#line 231 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                {
        (yyval.block_val) = nullptr;
    }
#line 1596 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 36: /* opt_else: KW_ELSE block  */
#line 234 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                    {
        (yyval.block_val) = (yyvsp[0].block_val);
    }
#line 1604 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 37: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 240 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                                {
        (yyval.while_val) = new to_lang::WhileStmt(to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::BlockStmtPtr((yyvsp[0].block_val)), yylineno, 1);
    }
#line 1612 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 38: /* return_stmt: KW_RETURN  */
#line 246 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
              {
        (yyval.return_val) = new to_lang::ReturnStmt(nullptr, yylineno, 1);
    }
#line 1620 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 39: /* return_stmt: KW_RETURN expr  */
#line 249 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                     {
        (yyval.return_val) = new to_lang::ReturnStmt(to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1);
    }
#line 1628 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 40: /* expr: expr '=' expr  */
#line 255 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                    { (yyval.expr_val) = new to_lang::AssignStmt("=", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1634 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 41: /* expr: expr OP_ADD_ASSIGN expr  */
#line 256 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                              { (yyval.expr_val) = new to_lang::AssignStmt("+=", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1640 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 42: /* expr: expr OP_SUB_ASSIGN expr  */
#line 257 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                              { (yyval.expr_val) = new to_lang::AssignStmt("-=", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1646 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 43: /* expr: expr OP_MUL_ASSIGN expr  */
#line 258 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                              { (yyval.expr_val) = new to_lang::AssignStmt("*=", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1652 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 44: /* expr: expr OP_DIV_ASSIGN expr  */
#line 259 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                              { (yyval.expr_val) = new to_lang::AssignStmt("/=", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1658 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 45: /* expr: expr OP_MOD_ASSIGN expr  */
#line 260 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                              { (yyval.expr_val) = new to_lang::AssignStmt("%=", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1664 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 46: /* expr: expr '+' expr  */
#line 261 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                      { (yyval.expr_val) = new to_lang::BinaryExpr("+", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1670 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 47: /* expr: expr '-' expr  */
#line 262 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                    { (yyval.expr_val) = new to_lang::BinaryExpr("-", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1676 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 48: /* expr: expr '*' expr  */
#line 263 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                    { (yyval.expr_val) = new to_lang::BinaryExpr("*", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1682 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 49: /* expr: expr '/' expr  */
#line 264 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                    { (yyval.expr_val) = new to_lang::BinaryExpr("/", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1688 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 50: /* expr: expr '%' expr  */
#line 265 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                    { (yyval.expr_val) = new to_lang::BinaryExpr("%", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1694 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 51: /* expr: expr OP_EQ expr  */
#line 266 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                      { (yyval.expr_val) = new to_lang::BinaryExpr("==", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1700 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 52: /* expr: expr OP_NEQ expr  */
#line 267 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                       { (yyval.expr_val) = new to_lang::BinaryExpr("!=", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1706 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 53: /* expr: expr '<' expr  */
#line 268 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                    { (yyval.expr_val) = new to_lang::BinaryExpr("<", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1712 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 54: /* expr: expr OP_LE expr  */
#line 269 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                      { (yyval.expr_val) = new to_lang::BinaryExpr("<=", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1718 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 55: /* expr: expr '>' expr  */
#line 270 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                    { (yyval.expr_val) = new to_lang::BinaryExpr(">", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1724 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 56: /* expr: expr OP_GE expr  */
#line 271 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                      { (yyval.expr_val) = new to_lang::BinaryExpr(">=", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1730 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 57: /* expr: expr OP_AND expr  */
#line 272 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                       { (yyval.expr_val) = new to_lang::BinaryExpr("&&", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1736 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 58: /* expr: expr OP_OR expr  */
#line 273 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                      { (yyval.expr_val) = new to_lang::BinaryExpr("||", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1742 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 59: /* expr: expr '&' expr  */
#line 274 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                    { (yyval.expr_val) = new to_lang::BinaryExpr("&", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1748 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 60: /* expr: expr '|' expr  */
#line 275 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                    { (yyval.expr_val) = new to_lang::BinaryExpr("|", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1754 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 61: /* expr: expr '^' expr  */
#line 276 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                    { (yyval.expr_val) = new to_lang::BinaryExpr("^", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1760 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 62: /* expr: expr OP_SHL expr  */
#line 277 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                       { (yyval.expr_val) = new to_lang::BinaryExpr("<<", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1766 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 63: /* expr: expr OP_SHR expr  */
#line 278 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                       { (yyval.expr_val) = new to_lang::BinaryExpr(">>", to_lang::ExprNodePtr((yyvsp[-2].expr_val)), to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1772 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 64: /* expr: '-' expr  */
#line 279 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                           { (yyval.expr_val) = new to_lang::UnaryExpr("-", to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1778 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 65: /* expr: '!' expr  */
#line 280 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                           { (yyval.expr_val) = new to_lang::UnaryExpr("!", to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1784 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 66: /* expr: '~' expr  */
#line 281 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                           { (yyval.expr_val) = new to_lang::UnaryExpr("~", to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1790 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 67: /* expr: '*' expr  */
#line 282 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                           { (yyval.expr_val) = new to_lang::UnaryExpr("*", to_lang::ExprNodePtr((yyvsp[0].expr_val)), yylineno, 1); }
#line 1796 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 68: /* expr: expr '[' expr ']'  */
#line 283 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                        {
        (yyval.expr_val) = new to_lang::IndexExpr(to_lang::ExprNodePtr((yyvsp[-3].expr_val)), to_lang::ExprNodePtr((yyvsp[-1].expr_val)), yylineno, 1);
    }
#line 1804 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 69: /* expr: IDENTIFIER '(' opt_arg_list ')'  */
#line 286 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                                      {
        (yyval.expr_val) = new to_lang::CallExpr((yyvsp[-3].str_val), *(yyvsp[-1].expr_list), yylineno, 1);
        free((yyvsp[-3].str_val));
        delete (yyvsp[-1].expr_list);
    }
#line 1814 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 70: /* expr: primary_expr  */
#line 291 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                   {
        (yyval.expr_val) = (yyvsp[0].expr_val);
    }
#line 1822 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 71: /* opt_arg_list: %empty  */
#line 297 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                {
        (yyval.expr_list) = new std::vector<to_lang::ExprNodePtr>();
    }
#line 1830 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 72: /* opt_arg_list: arg_list  */
#line 300 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
               {
        (yyval.expr_list) = (yyvsp[0].expr_list);
    }
#line 1838 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 73: /* arg_list: expr  */
#line 306 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
         {
        (yyval.expr_list) = new std::vector<to_lang::ExprNodePtr>();
        (yyval.expr_list)->push_back(to_lang::ExprNodePtr((yyvsp[0].expr_val)));
    }
#line 1847 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 74: /* arg_list: arg_list ',' expr  */
#line 310 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                        {
        (yyval.expr_list) = (yyvsp[-2].expr_list);
        (yyval.expr_list)->push_back(to_lang::ExprNodePtr((yyvsp[0].expr_val)));
    }
#line 1856 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 75: /* primary_expr: IDENTIFIER  */
#line 317 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
               {
        (yyval.expr_val) = new to_lang::VariableExpr((yyvsp[0].str_val), yylineno, 1);
        free((yyvsp[0].str_val));
    }
#line 1865 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 76: /* primary_expr: INT_LITERAL  */
#line 321 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                  {
        (yyval.expr_val) = new to_lang::LiteralExpr((yyvsp[0].int_val), yylineno, 1);
    }
#line 1873 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 77: /* primary_expr: CHAR_LITERAL  */
#line 324 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                   {
        (yyval.expr_val) = new to_lang::LiteralExpr((yyvsp[0].char_val), yylineno, 1);
    }
#line 1881 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 78: /* primary_expr: STRING_LITERAL  */
#line 327 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                     {
        (yyval.expr_val) = new to_lang::LiteralExpr(std::string((yyvsp[0].str_val)), yylineno, 1);
        free((yyvsp[0].str_val));
    }
#line 1890 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 79: /* primary_expr: KW_TRUE  */
#line 331 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
              {
        (yyval.expr_val) = new to_lang::LiteralExpr(true, yylineno, 1);
    }
#line 1898 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 80: /* primary_expr: KW_FALSE  */
#line 334 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
               {
        (yyval.expr_val) = new to_lang::LiteralExpr(false, yylineno, 1);
    }
#line 1906 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;

  case 81: /* primary_expr: '(' expr ')'  */
#line 337 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"
                   {
        (yyval.expr_val) = (yyvsp[-1].expr_val);
    }
#line 1914 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"
    break;


#line 1918 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/generated/grammar.cpp"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 342 "C:/Users/krasn/Documents/CO/TOCompilate/frontend/parser/grammar.y"


void yyerror(const char* s) {
    g_parse_error = s;
    g_error_line = yylineno;
}
