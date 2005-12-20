/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INCLUDE = 258,
     IF = 259,
     DEFINED = 260,
     MACRO = 261,
     MACRO_STRING = 262,
     ORG = 263,
     ERROR = 264,
     ECHO = 265,
     INCBIN = 266,
     INCLEN = 267,
     INCWORD = 268,
     RES = 269,
     WORD = 270,
     BYTE = 271,
     LDA = 272,
     LDX = 273,
     LDY = 274,
     STA = 275,
     STX = 276,
     STY = 277,
     AND = 278,
     ORA = 279,
     EOR = 280,
     ADC = 281,
     SBC = 282,
     CMP = 283,
     CPX = 284,
     CPY = 285,
     TSX = 286,
     TXS = 287,
     PHA = 288,
     PLA = 289,
     PHP = 290,
     PLP = 291,
     SEI = 292,
     CLI = 293,
     NOP = 294,
     TYA = 295,
     TAY = 296,
     TXA = 297,
     TAX = 298,
     CLC = 299,
     SEC = 300,
     RTS = 301,
     JSR = 302,
     JMP = 303,
     BEQ = 304,
     BNE = 305,
     BCC = 306,
     BCS = 307,
     BPL = 308,
     BMI = 309,
     BVC = 310,
     BVS = 311,
     INX = 312,
     DEX = 313,
     INY = 314,
     DEY = 315,
     INC = 316,
     DEC = 317,
     LSR = 318,
     ASL = 319,
     ROR = 320,
     ROL = 321,
     SYMBOL = 322,
     STRING = 323,
     LAND = 324,
     LOR = 325,
     LNOT = 326,
     LPAREN = 327,
     RPAREN = 328,
     COMMA = 329,
     COLON = 330,
     X = 331,
     Y = 332,
     HASH = 333,
     PLUS = 334,
     MINUS = 335,
     MULT = 336,
     DIV = 337,
     MOD = 338,
     LT = 339,
     GT = 340,
     EQ = 341,
     NEQ = 342,
     ASSIGN = 343,
     NUMBER = 344,
     vNEG = 345
   };
#endif
/* Tokens.  */
#define INCLUDE 258
#define IF 259
#define DEFINED 260
#define MACRO 261
#define MACRO_STRING 262
#define ORG 263
#define ERROR 264
#define ECHO 265
#define INCBIN 266
#define INCLEN 267
#define INCWORD 268
#define RES 269
#define WORD 270
#define BYTE 271
#define LDA 272
#define LDX 273
#define LDY 274
#define STA 275
#define STX 276
#define STY 277
#define AND 278
#define ORA 279
#define EOR 280
#define ADC 281
#define SBC 282
#define CMP 283
#define CPX 284
#define CPY 285
#define TSX 286
#define TXS 287
#define PHA 288
#define PLA 289
#define PHP 290
#define PLP 291
#define SEI 292
#define CLI 293
#define NOP 294
#define TYA 295
#define TAY 296
#define TXA 297
#define TAX 298
#define CLC 299
#define SEC 300
#define RTS 301
#define JSR 302
#define JMP 303
#define BEQ 304
#define BNE 305
#define BCC 306
#define BCS 307
#define BPL 308
#define BMI 309
#define BVC 310
#define BVS 311
#define INX 312
#define DEX 313
#define INY 314
#define DEY 315
#define INC 316
#define DEC 317
#define LSR 318
#define ASL 319
#define ROR 320
#define ROL 321
#define SYMBOL 322
#define STRING 323
#define LAND 324
#define LOR 325
#define LNOT 326
#define LPAREN 327
#define RPAREN 328
#define COMMA 329
#define COLON 330
#define X 331
#define Y 332
#define HASH 333
#define PLUS 334
#define MINUS 335
#define MULT 336
#define DIV 337
#define MOD 338
#define LT 339
#define GT 340
#define EQ 341
#define NEQ 342
#define ASSIGN 343
#define NUMBER 344
#define vNEG 345




/* Copy the first part of user declarations.  */
#line 28 "asm.y"

#include "int.h"
#include "parse.h"
#include "vec.h"
#include "membuf.h"
#include "log.h"
#include <stdio.h>
#define YYERROR_VERBOSE

static struct vec asm_atoms[1];


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 138 "asm.y"
typedef union YYSTYPE {
    i32 num;
    char *str;
    struct atom *atom;
    struct expr *expr;
} YYSTYPE;
/* Line 196 of yacc.c.  */
#line 284 "asm.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 296 "asm.tab.c"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  213
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   596

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  91
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  17
/* YYNRULES -- Number of rules. */
#define YYNRULES  194
/* YYNRULES -- Number of states. */
#define YYNSTATES  311

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   345

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     6,     8,    11,    15,    20,    25,    30,
      35,    40,    45,    47,    49,    51,    58,    63,    68,    73,
      80,    89,    93,    95,    98,   101,   104,   107,   110,   113,
     116,   119,   122,   125,   128,   131,   134,   137,   140,   143,
     146,   149,   152,   155,   158,   161,   164,   167,   170,   173,
     176,   179,   182,   185,   188,   191,   194,   197,   200,   203,
     206,   209,   212,   215,   218,   221,   224,   227,   230,   233,
     236,   239,   242,   245,   248,   251,   254,   257,   260,   263,
     266,   269,   272,   275,   278,   281,   284,   287,   290,   293,
     296,   299,   302,   305,   308,   311,   314,   317,   320,   323,
     326,   329,   332,   335,   338,   341,   344,   347,   350,   352,
     354,   356,   358,   360,   362,   364,   366,   368,   370,   372,
     374,   376,   378,   380,   382,   385,   388,   391,   394,   397,
     400,   403,   406,   409,   412,   414,   416,   418,   420,   423,
     426,   429,   432,   435,   438,   441,   444,   446,   449,   452,
     455,   458,   460,   463,   466,   469,   472,   474,   477,   480,
     483,   486,   488,   491,   494,   497,   500,   503,   505,   509,
     513,   516,   521,   526,   532,   538,   542,   546,   550,   554,
     558,   561,   565,   570,   577,   579,   581,   585,   589,   592,
     596,   600,   604,   608,   612
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      92,     0,    -1,    92,    93,    -1,    93,    -1,    67,    75,
      -1,    67,    88,   106,    -1,     4,    72,   107,    73,    -1,
       8,    72,   106,    73,    -1,     9,    72,    68,    73,    -1,
      10,    72,    68,    73,    -1,     3,    72,    68,    73,    -1,
       6,    72,    68,    73,    -1,    94,    -1,     7,    -1,    96,
      -1,    14,    72,   106,    74,   106,    73,    -1,    15,    72,
      95,    73,    -1,    16,    72,    95,    73,    -1,    11,    72,
      68,    73,    -1,    11,    72,    68,    74,   106,    73,    -1,
      11,    72,    68,    74,   106,    74,   106,    73,    -1,    95,
      74,   106,    -1,   106,    -1,    17,    97,    -1,    17,   101,
      -1,    17,   102,    -1,    17,    98,    -1,    17,    99,    -1,
      17,   100,    -1,    17,   104,    -1,    17,   105,    -1,    18,
      97,    -1,    18,   101,    -1,    18,   103,    -1,    18,    98,
      -1,    18,   100,    -1,    19,    97,    -1,    19,   101,    -1,
      19,   102,    -1,    19,    98,    -1,    19,   100,    -1,    20,
     101,    -1,    20,   102,    -1,    20,    98,    -1,    20,    99,
      -1,    20,   100,    -1,    20,   104,    -1,    20,   105,    -1,
      21,   101,    -1,    21,   103,    -1,    21,    98,    -1,    22,
     101,    -1,    22,   102,    -1,    22,    98,    -1,    23,    97,
      -1,    23,   101,    -1,    23,   102,    -1,    23,    98,    -1,
      23,    99,    -1,    23,   100,    -1,    23,   104,    -1,    23,
     105,    -1,    24,    97,    -1,    24,   101,    -1,    24,   102,
      -1,    24,    98,    -1,    24,    99,    -1,    24,   100,    -1,
      24,   104,    -1,    24,   105,    -1,    25,    97,    -1,    25,
     101,    -1,    25,   102,    -1,    25,    98,    -1,    25,    99,
      -1,    25,   100,    -1,    25,   104,    -1,    25,   105,    -1,
      26,    97,    -1,    26,   101,    -1,    26,   102,    -1,    26,
      98,    -1,    26,    99,    -1,    26,   100,    -1,    26,   104,
      -1,    26,   105,    -1,    27,    97,    -1,    27,   101,    -1,
      27,   102,    -1,    27,    98,    -1,    27,    99,    -1,    27,
     100,    -1,    27,   104,    -1,    27,   105,    -1,    28,    97,
      -1,    28,   101,    -1,    28,   102,    -1,    28,    98,    -1,
      28,    99,    -1,    28,   100,    -1,    28,   104,    -1,    28,
     105,    -1,    29,    97,    -1,    29,   101,    -1,    29,    98,
      -1,    30,    97,    -1,    30,   101,    -1,    30,    98,    -1,
      32,    -1,    31,    -1,    33,    -1,    34,    -1,    35,    -1,
      36,    -1,    37,    -1,    38,    -1,    39,    -1,    40,    -1,
      41,    -1,    42,    -1,    43,    -1,    44,    -1,    45,    -1,
      46,    -1,    47,    98,    -1,    48,    98,    -1,    49,    98,
      -1,    50,    98,    -1,    51,    98,    -1,    52,    98,    -1,
      53,    98,    -1,    54,    98,    -1,    55,    98,    -1,    56,
      98,    -1,    57,    -1,    58,    -1,    59,    -1,    60,    -1,
      61,   101,    -1,    61,   102,    -1,    61,    98,    -1,    61,
      99,    -1,    62,   101,    -1,    62,   102,    -1,    62,    98,
      -1,    62,    99,    -1,    63,    -1,    63,   101,    -1,    63,
     102,    -1,    63,    98,    -1,    63,    99,    -1,    64,    -1,
      64,   101,    -1,    64,   102,    -1,    64,    98,    -1,    64,
      99,    -1,    65,    -1,    65,   101,    -1,    65,   102,    -1,
      65,    98,    -1,    65,    99,    -1,    66,    -1,    66,   101,
      -1,    66,   102,    -1,    66,    98,    -1,    66,    99,    -1,
      78,   106,    -1,   106,    -1,   106,    74,    76,    -1,   106,
      74,    77,    -1,    84,   106,    -1,    84,   106,    74,    76,
      -1,    84,   106,    74,    77,    -1,    72,   106,    74,    76,
      73,    -1,    72,   106,    73,    74,    77,    -1,   106,    79,
     106,    -1,   106,    80,   106,    -1,   106,    81,   106,    -1,
     106,    82,   106,    -1,   106,    83,   106,    -1,    80,   106,
      -1,    72,   106,    73,    -1,    12,    72,    68,    73,    -1,
      13,    72,    68,    74,   106,    73,    -1,    89,    -1,    67,
      -1,   107,    70,   107,    -1,   107,    69,   107,    -1,    71,
     107,    -1,    72,   107,    73,    -1,   106,    84,   106,    -1,
     106,    85,   106,    -1,   106,    86,   106,    -1,   106,    87,
     106,    -1,     5,    72,    67,    73,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   172,   172,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   184,   185,   186,   187,   188,   190,
     192,   195,   196,   198,   199,   200,   201,   202,   203,   204,
     205,   207,   208,   209,   210,   211,   213,   214,   215,   216,
     217,   219,   220,   221,   222,   223,   224,   225,   227,   228,
     229,   231,   232,   233,   235,   236,   237,   238,   239,   240,
     241,   242,   244,   245,   246,   247,   248,   249,   250,   251,
     253,   254,   255,   256,   257,   258,   259,   260,   262,   263,
     264,   265,   266,   267,   268,   269,   271,   272,   273,   274,
     275,   276,   277,   278,   280,   281,   282,   283,   284,   285,
     286,   287,   289,   290,   291,   292,   293,   294,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   324,   325,   326,   327,   329,   330,
     331,   332,   334,   335,   336,   337,   339,   340,   341,   342,
     343,   345,   346,   347,   348,   349,   351,   352,   353,   354,
     355,   357,   358,   359,   360,   361,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   383,   384,   386,   387,   388,   389,
     390,   391,   392,   393,   395
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INCLUDE", "IF", "DEFINED", "MACRO",
  "MACRO_STRING", "ORG", "ERROR", "ECHO", "INCBIN", "INCLEN", "INCWORD",
  "RES", "WORD", "BYTE", "LDA", "LDX", "LDY", "STA", "STX", "STY", "AND",
  "ORA", "EOR", "ADC", "SBC", "CMP", "CPX", "CPY", "TSX", "TXS", "PHA",
  "PLA", "PHP", "PLP", "SEI", "CLI", "NOP", "TYA", "TAY", "TXA", "TAX",
  "CLC", "SEC", "RTS", "JSR", "JMP", "BEQ", "BNE", "BCC", "BCS", "BPL",
  "BMI", "BVC", "BVS", "INX", "DEX", "INY", "DEY", "INC", "DEC", "LSR",
  "ASL", "ROR", "ROL", "SYMBOL", "STRING", "LAND", "LOR", "LNOT", "LPAREN",
  "RPAREN", "COMMA", "COLON", "X", "Y", "HASH", "PLUS", "MINUS", "MULT",
  "DIV", "MOD", "LT", "GT", "EQ", "NEQ", "ASSIGN", "NUMBER", "vNEG",
  "$accept", "stmts", "stmt", "atom", "exprs", "op", "am_im", "am_a",
  "am_ax", "am_ay", "am_zp", "am_zpx", "am_zpy", "am_ix", "am_iy", "expr",
  "lexpr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    91,    92,    92,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    94,    94,    94,    94,    94,    94,
      94,    95,    95,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   107,   107,   107,   107,
     107,   107,   107,   107,   107
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     2,     1,     2,     3,     4,     4,     4,     4,
       4,     4,     1,     1,     1,     6,     4,     4,     4,     6,
       8,     3,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     1,     1,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     2,     2,
       2,     1,     2,     2,     2,     2,     1,     2,     2,     2,
       2,     1,     2,     2,     2,     2,     2,     1,     3,     3,
       2,     4,     4,     5,     5,     3,     3,     3,     3,     3,
       2,     3,     4,     6,     1,     1,     3,     3,     2,     3,
       3,     3,     3,     3,     4
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,     0,     0,    13,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   109,   108,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   134,   135,   136,   137,     0,     0,   146,   151,
     156,   161,     0,     0,     3,    12,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   185,
       0,     0,     0,     0,   184,    23,    26,    27,    28,    24,
      25,    29,    30,   167,     0,     0,    31,    34,    35,    32,
      33,   167,    36,    39,    40,    37,    38,    43,    44,    45,
      41,    42,    46,    47,    50,    48,    49,   167,    53,    51,
      52,    54,    57,    58,    59,    55,    56,    60,    61,    62,
      65,    66,    67,    63,    64,    68,    69,    70,    73,    74,
      75,    71,    72,    76,    77,    78,    81,    82,    83,    79,
      80,    84,    85,    86,    89,    90,    91,    87,    88,    92,
      93,    94,    97,    98,    99,    95,    96,   100,   101,     0,
     102,   104,   103,   105,   107,   106,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   140,   141,   138,   139,
     167,   144,   145,   142,   143,   149,   150,   147,   148,   154,
     155,   152,   153,   159,   160,   157,   158,   164,   165,   162,
     163,     4,     0,     1,     2,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,   166,   180,   170,     0,     0,     0,     0,
       0,     0,     0,   170,     0,   170,     0,     5,    10,     0,
     188,     0,     0,     0,     0,     0,     0,     0,     0,     6,
      11,     7,     8,     9,    18,     0,     0,    16,     0,    17,
       0,     0,   181,     0,     0,   168,   169,   175,   176,   177,
     178,   179,   181,     0,     0,   189,   190,   191,   192,   193,
     187,   186,     0,     0,    21,   182,     0,     0,     0,   171,
     172,   194,    19,     0,    15,     0,   174,   173,     0,   183,
      20
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,    63,    64,    65,   227,    66,    85,    86,    87,    88,
      89,    90,   100,    91,    92,   117,   220
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -186
static const short int yypact[] =
{
     344,   -67,   -59,   -56,  -186,   -20,   -17,   -15,   -13,     3,
       9,    12,    13,    23,    38,    48,    64,    87,    13,    13,
      13,    13,    13,    13,    41,    41,  -186,  -186,  -186,  -186,
    -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,
    -186,  -186,   121,   121,   121,   121,   121,   121,   121,   121,
     121,   121,  -186,  -186,  -186,  -186,    87,    87,    87,    87,
      87,    87,   -71,   279,  -186,  -186,  -186,    18,     7,    26,
     121,    30,    36,    43,   121,   121,   121,    34,    37,  -186,
     121,   121,   121,   121,  -186,  -186,  -186,  -186,  -186,  -186,
    -186,  -186,  -186,   -40,   121,   121,  -186,  -186,  -186,  -186,
    -186,   455,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,
    -186,  -186,  -186,  -186,  -186,  -186,  -186,   132,  -186,  -186,
    -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,
    -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,
    -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,
    -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,
    -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,   121,
    -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,
    -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,
     465,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,
    -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,  -186,
    -186,  -186,   121,  -186,  -186,    44,    42,     7,     7,   500,
     -55,    53,   413,    56,    74,   -50,   475,   -36,   132,    50,
      81,    83,   -52,   132,  -186,   485,    69,   121,   121,   121,
     121,   121,   418,   495,    73,   132,    76,   132,  -186,    91,
    -186,   178,    19,   121,   121,   121,   121,     7,     7,  -186,
    -186,  -186,  -186,  -186,  -186,   121,   121,  -186,   121,  -186,
      93,    86,    94,    96,    97,  -186,  -186,   -16,   -16,  -186,
    -186,  -186,  -186,    92,   111,  -186,   132,   132,   132,   132,
    -186,   101,    82,   429,   132,  -186,   121,   108,   113,  -186,
    -186,  -186,  -186,   121,  -186,   434,  -186,  -186,   445,  -186,
    -186
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -186,  -186,   124,  -186,   114,  -186,   253,   399,   176,   461,
     412,   160,   173,   120,   573,   -12,  -185
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned short int yytable[] =
{
      93,   101,   101,    93,   211,    67,    93,    93,    93,    93,
      93,    93,   216,    68,   257,   258,    69,   212,   259,    77,
      78,   272,   273,   264,   265,    77,    78,   237,   238,   239,
     240,   241,   250,   252,   236,    77,    78,   267,   268,   237,
     238,   239,   240,   241,   190,   190,   190,   190,   190,   190,
      77,    78,    70,    77,    78,    71,   219,    72,   222,    73,
      77,    78,   226,   228,   228,   239,   240,   241,   232,   233,
     234,   235,   290,   291,    79,    74,    77,    78,   217,   218,
      79,    75,   242,   243,    76,    80,   215,    82,   257,   258,
      79,    81,   285,    82,   221,    94,    84,    83,   223,    77,
      78,    81,    84,    82,   224,    79,   230,    95,    79,   231,
      94,   225,    84,    94,   249,    79,    81,   248,    82,    81,
      80,    82,    83,   269,   268,   169,   260,    84,    82,   262,
      84,    79,    83,    77,    78,   112,    94,    84,   127,   135,
     143,   151,   159,   167,    82,   275,   276,   263,    95,   270,
     276,   271,   275,    84,    79,   302,   303,   245,   284,    94,
     296,   237,   238,   239,   240,   241,   295,    82,   297,   300,
     257,    83,   298,   299,   106,   111,    84,   120,   126,   134,
     142,   150,   158,   166,   301,   306,   307,   214,    79,   116,
     229,   108,     0,    94,   123,   131,   139,   147,   155,   163,
     247,    82,     0,     0,     0,   219,   251,     0,     0,     0,
      84,   237,   238,   239,   240,   241,   189,   194,   198,   202,
     206,   210,     0,     0,     0,   277,   278,   279,   280,   281,
       0,     0,   187,   192,   196,   200,   204,   208,     0,     0,
       0,   286,   287,   288,   289,   219,   219,     0,     0,     0,
       0,   282,     0,   292,   293,     0,   294,   237,   238,   239,
     240,   241,   253,   254,   255,   256,    96,   102,     0,     0,
       0,   121,   129,   137,   145,   153,   161,   170,   173,   213,
       0,     0,     1,     2,   305,     3,     4,     5,     6,     7,
       8,   308,     0,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,     1,     2,     0,
       3,     4,     5,     6,     7,     8,     0,     0,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    97,   103,   107,   114,   118,   122,   130,   138,
     146,   154,   162,   171,   174,    99,   105,   110,   115,   119,
     125,   133,   141,   149,   157,   165,   172,   175,     0,     0,
       0,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,     0,     0,     0,     0,   186,   191,   195,   199,   203,
     207,     0,     0,     0,     0,     0,     0,     0,   188,   193,
     197,   201,   205,   209,    98,   104,   109,     0,     0,   124,
     132,   140,   148,   156,   164,     0,   261,     0,     0,     0,
       0,   282,   237,   238,   239,   240,   241,   237,   238,   239,
     240,   241,   304,     0,     0,     0,     0,   309,   237,   238,
     239,   240,   241,   237,   238,   239,   240,   241,   310,     0,
       0,     0,     0,     0,   237,   238,   239,   240,   241,   244,
       0,     0,     0,     0,   237,   238,   239,   240,   241,   246,
       0,     0,     0,     0,   237,   238,   239,   240,   241,   266,
       0,     0,     0,     0,   237,   238,   239,   240,   241,   274,
       0,     0,     0,     0,   237,   238,   239,   240,   241,   283,
       0,     0,     0,     0,   237,   238,   239,   240,   241,   237,
     238,   239,   240,   241,   253,   254,   255,   256,   113,     0,
       0,   128,   136,   144,   152,   160,   168
};

static const short int yycheck[] =
{
      12,    13,    14,    15,    75,    72,    18,    19,    20,    21,
      22,    23,     5,    72,    69,    70,    72,    88,    73,    12,
      13,    73,    74,    73,    74,    12,    13,    79,    80,    81,
      82,    83,   217,   218,    74,    12,    13,    73,    74,    79,
      80,    81,    82,    83,    56,    57,    58,    59,    60,    61,
      12,    13,    72,    12,    13,    72,    68,    72,    70,    72,
      12,    13,    74,    75,    76,    81,    82,    83,    80,    81,
      82,    83,   257,   258,    67,    72,    12,    13,    71,    72,
      67,    72,    94,    95,    72,    72,    68,    80,    69,    70,
      67,    78,    73,    80,    68,    72,    89,    84,    68,    12,
      13,    78,    89,    80,    68,    67,    72,    84,    67,    72,
      72,    68,    89,    72,    72,    67,    78,    73,    80,    78,
      72,    80,    84,    73,    74,    84,    73,    89,    80,    73,
      89,    67,    84,    12,    13,    15,    72,    89,    18,    19,
      20,    21,    22,    23,    80,    76,    77,    73,    84,    68,
      77,    68,    76,    89,    67,    73,    74,   169,    67,    72,
      74,    79,    80,    81,    82,    83,    73,    80,    74,    77,
      69,    84,    76,    76,    14,    15,    89,    17,    18,    19,
      20,    21,    22,    23,    73,    77,    73,    63,    67,    16,
      76,    15,    -1,    72,    18,    19,    20,    21,    22,    23,
     212,    80,    -1,    -1,    -1,   217,   218,    -1,    -1,    -1,
      89,    79,    80,    81,    82,    83,    56,    57,    58,    59,
      60,    61,    -1,    -1,    -1,   237,   238,   239,   240,   241,
      -1,    -1,    56,    57,    58,    59,    60,    61,    -1,    -1,
      -1,   253,   254,   255,   256,   257,   258,    -1,    -1,    -1,
      -1,    73,    -1,   265,   266,    -1,   268,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    13,    14,    -1,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,     0,
      -1,    -1,     3,     4,   296,     6,     7,     8,     9,    10,
      11,   303,    -1,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,     3,     4,    -1,
       6,     7,     8,     9,    10,    11,    -1,    -1,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    -1,    -1,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    -1,    -1,    -1,    -1,    56,    57,    58,    59,    60,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,
      58,    59,    60,    61,    13,    14,    15,    -1,    -1,    18,
      19,    20,    21,    22,    23,    -1,    73,    -1,    -1,    -1,
      -1,    73,    79,    80,    81,    82,    83,    79,    80,    81,
      82,    83,    73,    -1,    -1,    -1,    -1,    73,    79,    80,
      81,    82,    83,    79,    80,    81,    82,    83,    73,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    74,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    74,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    74,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    74,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    74,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,     4,     6,     7,     8,     9,    10,    11,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    92,    93,    94,    96,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    12,    13,    67,
      72,    78,    80,    84,    89,    97,    98,    99,   100,   101,
     102,   104,   105,   106,    72,    84,    97,    98,   100,   101,
     103,   106,    97,    98,   100,   101,   102,    98,    99,   100,
     101,   102,   104,   105,    98,   101,   103,   106,    98,   101,
     102,    97,    98,    99,   100,   101,   102,   104,   105,    97,
      98,    99,   100,   101,   102,   104,   105,    97,    98,    99,
     100,   101,   102,   104,   105,    97,    98,    99,   100,   101,
     102,   104,   105,    97,    98,    99,   100,   101,   102,   104,
     105,    97,    98,    99,   100,   101,   102,   104,   105,    84,
      97,    98,   101,    97,    98,   101,    98,    98,    98,    98,
      98,    98,    98,    98,    98,    98,    98,    99,   101,   102,
     106,    98,    99,   101,   102,    98,    99,   101,   102,    98,
      99,   101,   102,    98,    99,   101,   102,    98,    99,   101,
     102,    75,    88,     0,    93,    68,     5,    71,    72,   106,
     107,    68,   106,    68,    68,    68,   106,    95,   106,    95,
      72,    72,   106,   106,   106,   106,    74,    79,    80,    81,
      82,    83,   106,   106,    74,   106,    74,   106,    73,    72,
     107,   106,   107,    84,    85,    86,    87,    69,    70,    73,
      73,    73,    73,    73,    73,    74,    74,    73,    74,    73,
      68,    68,    73,    74,    74,    76,    77,   106,   106,   106,
     106,   106,    73,    74,    67,    73,   106,   106,   106,   106,
     107,   107,   106,   106,   106,    73,    74,    74,    76,    76,
      77,    73,    73,    74,    73,   106,    77,    73,   106,    73,
      73
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname[yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
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
#ifndef	YYINITDEPTH
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
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
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
      size_t yyn = 0;
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

#endif /* YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()
    ;
#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


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
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
     `$$ = $1'.

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
#line 173 "asm.y"
    { new_label((yyvsp[-1].str)); ;}
    break;

  case 5:
#line 174 "asm.y"
    { new_symbol_expr((yyvsp[-2].str), (yyvsp[0].expr)); ;}
    break;

  case 6:
#line 175 "asm.y"
    { push_if_state((yyvsp[-1].expr)); ;}
    break;

  case 7:
#line 176 "asm.y"
    { set_org((yyvsp[-1].expr)); ;}
    break;

  case 8:
#line 177 "asm.y"
    { asm_error((yyvsp[-1].str)); ;}
    break;

  case 9:
#line 178 "asm.y"
    { asm_echo((yyvsp[-1].str)); ;}
    break;

  case 10:
#line 179 "asm.y"
    { asm_include((yyvsp[-1].str)); ;}
    break;

  case 11:
#line 180 "asm.y"
    { push_macro_state((yyvsp[-1].str)); ;}
    break;

  case 12:
#line 181 "asm.y"
    { vec_push(asm_atoms, &(yyvsp[0].atom)); ;}
    break;

  case 13:
#line 182 "asm.y"
    { macro_append((yyvsp[0].str)) ;}
    break;

  case 14:
#line 184 "asm.y"
    { (yyval.atom) = (yyvsp[0].atom);}
    break;

  case 15:
#line 185 "asm.y"
    { (yyval.atom) = new_res((yyvsp[-3].expr), (yyvsp[-1].expr)); ;}
    break;

  case 16:
#line 186 "asm.y"
    { (yyval.atom) = exprs_to_word_exprs((yyvsp[-1].atom)); ;}
    break;

  case 17:
#line 187 "asm.y"
    { (yyval.atom) = exprs_to_byte_exprs((yyvsp[-1].atom)); ;}
    break;

  case 18:
#line 188 "asm.y"
    {
            (yyval.atom) = new_incbin((yyvsp[-1].str), NULL, NULL); ;}
    break;

  case 19:
#line 190 "asm.y"
    {
            (yyval.atom) = new_incbin((yyvsp[-3].str), (yyvsp[-1].expr), NULL); ;}
    break;

  case 20:
#line 192 "asm.y"
    {
            (yyval.atom) = new_incbin((yyvsp[-5].str), (yyvsp[-3].expr), (yyvsp[-1].expr)); ;}
    break;

  case 21:
#line 195 "asm.y"
    { (yyval.atom) = exprs_add((yyvsp[-2].atom), (yyvsp[0].expr)); ;}
    break;

  case 22:
#line 196 "asm.y"
    { (yyval.atom) = new_exprs((yyvsp[0].expr)); ;}
    break;

  case 23:
#line 198 "asm.y"
    { (yyval.atom) = new_op(0xA9, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); ;}
    break;

  case 24:
#line 199 "asm.y"
    { (yyval.atom) = new_op(0xA5, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 25:
#line 200 "asm.y"
    { (yyval.atom) = new_op(0xB5, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 26:
#line 201 "asm.y"
    { (yyval.atom) = new_op(0xAD, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 27:
#line 202 "asm.y"
    { (yyval.atom) = new_op(0xBD, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 28:
#line 203 "asm.y"
    { (yyval.atom) = new_op(0xB9, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 29:
#line 204 "asm.y"
    { (yyval.atom) = new_op(0xA1, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 30:
#line 205 "asm.y"
    { (yyval.atom) = new_op(0xB1, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 31:
#line 207 "asm.y"
    { (yyval.atom) = new_op(0xA2, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); ;}
    break;

  case 32:
#line 208 "asm.y"
    { (yyval.atom) = new_op(0xA6, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 33:
#line 209 "asm.y"
    { (yyval.atom) = new_op(0xB6, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 34:
#line 210 "asm.y"
    { (yyval.atom) = new_op(0xAE, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 35:
#line 211 "asm.y"
    { (yyval.atom) = new_op(0xBE, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 36:
#line 213 "asm.y"
    { (yyval.atom) = new_op(0xA0, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); ;}
    break;

  case 37:
#line 214 "asm.y"
    { (yyval.atom) = new_op(0xA4, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 38:
#line 215 "asm.y"
    { (yyval.atom) = new_op(0xB4, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 39:
#line 216 "asm.y"
    { (yyval.atom) = new_op(0xAC, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 40:
#line 217 "asm.y"
    { (yyval.atom) = new_op(0xBC, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 41:
#line 219 "asm.y"
    { (yyval.atom) = new_op(0x85, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 42:
#line 220 "asm.y"
    { (yyval.atom) = new_op(0x95, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 43:
#line 221 "asm.y"
    { (yyval.atom) = new_op(0x8D, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 44:
#line 222 "asm.y"
    { (yyval.atom) = new_op(0x9D, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 45:
#line 223 "asm.y"
    { (yyval.atom) = new_op(0x99, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 46:
#line 224 "asm.y"
    { (yyval.atom) = new_op(0x81, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 47:
#line 225 "asm.y"
    { (yyval.atom) = new_op(0x91, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 48:
#line 227 "asm.y"
    { (yyval.atom) = new_op(0x86, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 49:
#line 228 "asm.y"
    { (yyval.atom) = new_op(0x96, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 50:
#line 229 "asm.y"
    { (yyval.atom) = new_op(0x8e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 51:
#line 231 "asm.y"
    { (yyval.atom) = new_op(0x84, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 52:
#line 232 "asm.y"
    { (yyval.atom) = new_op(0x94, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 53:
#line 233 "asm.y"
    { (yyval.atom) = new_op(0x8c, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 54:
#line 235 "asm.y"
    { (yyval.atom) = new_op(0x29, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); ;}
    break;

  case 55:
#line 236 "asm.y"
    { (yyval.atom) = new_op(0x25, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 56:
#line 237 "asm.y"
    { (yyval.atom) = new_op(0x35, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 57:
#line 238 "asm.y"
    { (yyval.atom) = new_op(0x2d, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 58:
#line 239 "asm.y"
    { (yyval.atom) = new_op(0x3d, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 59:
#line 240 "asm.y"
    { (yyval.atom) = new_op(0x39, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 60:
#line 241 "asm.y"
    { (yyval.atom) = new_op(0x21, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 61:
#line 242 "asm.y"
    { (yyval.atom) = new_op(0x31, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 62:
#line 244 "asm.y"
    { (yyval.atom) = new_op(0x09, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); ;}
    break;

  case 63:
#line 245 "asm.y"
    { (yyval.atom) = new_op(0x05, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 64:
#line 246 "asm.y"
    { (yyval.atom) = new_op(0x15, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 65:
#line 247 "asm.y"
    { (yyval.atom) = new_op(0x0d, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 66:
#line 248 "asm.y"
    { (yyval.atom) = new_op(0x1d, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 67:
#line 249 "asm.y"
    { (yyval.atom) = new_op(0x19, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 68:
#line 250 "asm.y"
    { (yyval.atom) = new_op(0x01, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 69:
#line 251 "asm.y"
    { (yyval.atom) = new_op(0x11, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 70:
#line 253 "asm.y"
    { (yyval.atom) = new_op(0x49, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); ;}
    break;

  case 71:
#line 254 "asm.y"
    { (yyval.atom) = new_op(0x45, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 72:
#line 255 "asm.y"
    { (yyval.atom) = new_op(0x55, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 73:
#line 256 "asm.y"
    { (yyval.atom) = new_op(0x4d, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 74:
#line 257 "asm.y"
    { (yyval.atom) = new_op(0x5d, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 75:
#line 258 "asm.y"
    { (yyval.atom) = new_op(0x59, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 76:
#line 259 "asm.y"
    { (yyval.atom) = new_op(0x41, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 77:
#line 260 "asm.y"
    { (yyval.atom) = new_op(0x51, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 78:
#line 262 "asm.y"
    { (yyval.atom) = new_op(0x69, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); ;}
    break;

  case 79:
#line 263 "asm.y"
    { (yyval.atom) = new_op(0x65, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 80:
#line 264 "asm.y"
    { (yyval.atom) = new_op(0x75, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 81:
#line 265 "asm.y"
    { (yyval.atom) = new_op(0x6D, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 82:
#line 266 "asm.y"
    { (yyval.atom) = new_op(0x7D, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 83:
#line 267 "asm.y"
    { (yyval.atom) = new_op(0x79, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 84:
#line 268 "asm.y"
    { (yyval.atom) = new_op(0x61, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 85:
#line 269 "asm.y"
    { (yyval.atom) = new_op(0x71, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 86:
#line 271 "asm.y"
    { (yyval.atom) = new_op(0xe9, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); ;}
    break;

  case 87:
#line 272 "asm.y"
    { (yyval.atom) = new_op(0xe5, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 88:
#line 273 "asm.y"
    { (yyval.atom) = new_op(0xf5, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 89:
#line 274 "asm.y"
    { (yyval.atom) = new_op(0xeD, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 90:
#line 275 "asm.y"
    { (yyval.atom) = new_op(0xfD, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 91:
#line 276 "asm.y"
    { (yyval.atom) = new_op(0xf9, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 92:
#line 277 "asm.y"
    { (yyval.atom) = new_op(0xe1, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 93:
#line 278 "asm.y"
    { (yyval.atom) = new_op(0xf1, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 94:
#line 280 "asm.y"
    { (yyval.atom) = new_op(0xc9, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); ;}
    break;

  case 95:
#line 281 "asm.y"
    { (yyval.atom) = new_op(0xc5, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); ;}
    break;

  case 96:
#line 282 "asm.y"
    { (yyval.atom) = new_op(0xd5, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 97:
#line 283 "asm.y"
    { (yyval.atom) = new_op(0xcD, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 98:
#line 284 "asm.y"
    { (yyval.atom) = new_op(0xdD, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 99:
#line 285 "asm.y"
    { (yyval.atom) = new_op(0xd9, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 100:
#line 286 "asm.y"
    { (yyval.atom) = new_op(0xc1, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 101:
#line 287 "asm.y"
    { (yyval.atom) = new_op(0xd1, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 102:
#line 289 "asm.y"
    { (yyval.atom) = new_op(0xe0, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 103:
#line 290 "asm.y"
    { (yyval.atom) = new_op(0xe4, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 104:
#line 291 "asm.y"
    { (yyval.atom) = new_op(0xec, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 105:
#line 292 "asm.y"
    { (yyval.atom) = new_op(0xc0, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 106:
#line 293 "asm.y"
    { (yyval.atom) = new_op(0xc4, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 107:
#line 294 "asm.y"
    { (yyval.atom) = new_op(0xcc, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 108:
#line 296 "asm.y"
    { (yyval.atom) = new_op0(0x9A); ;}
    break;

  case 109:
#line 297 "asm.y"
    { (yyval.atom) = new_op0(0xBA); ;}
    break;

  case 110:
#line 298 "asm.y"
    { (yyval.atom) = new_op0(0x48); ;}
    break;

  case 111:
#line 299 "asm.y"
    { (yyval.atom) = new_op0(0x68); ;}
    break;

  case 112:
#line 300 "asm.y"
    { (yyval.atom) = new_op0(0x08); ;}
    break;

  case 113:
#line 301 "asm.y"
    { (yyval.atom) = new_op0(0x28); ;}
    break;

  case 114:
#line 302 "asm.y"
    { (yyval.atom) = new_op0(0x78); ;}
    break;

  case 115:
#line 303 "asm.y"
    { (yyval.atom) = new_op0(0x58); ;}
    break;

  case 116:
#line 304 "asm.y"
    { (yyval.atom) = new_op0(0xea); ;}
    break;

  case 117:
#line 305 "asm.y"
    { (yyval.atom) = new_op0(0x98); ;}
    break;

  case 118:
#line 306 "asm.y"
    { (yyval.atom) = new_op0(0xa8); ;}
    break;

  case 119:
#line 307 "asm.y"
    { (yyval.atom) = new_op0(0x8a); ;}
    break;

  case 120:
#line 308 "asm.y"
    { (yyval.atom) = new_op0(0xaa); ;}
    break;

  case 121:
#line 309 "asm.y"
    { (yyval.atom) = new_op0(0x18); ;}
    break;

  case 122:
#line 310 "asm.y"
    { (yyval.atom) = new_op0(0x38); ;}
    break;

  case 123:
#line 311 "asm.y"
    { (yyval.atom) = new_op0(0x60); ;}
    break;

  case 124:
#line 313 "asm.y"
    { (yyval.atom) = new_op(0x20, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 125:
#line 314 "asm.y"
    { (yyval.atom) = new_op(0x4c, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 126:
#line 315 "asm.y"
    { (yyval.atom) = new_op(0xf0, ATOM_TYPE_OP_ARG_I8,  (yyvsp[0].expr)); ;}
    break;

  case 127:
#line 316 "asm.y"
    { (yyval.atom) = new_op(0xd0, ATOM_TYPE_OP_ARG_I8,  (yyvsp[0].expr)); ;}
    break;

  case 128:
#line 317 "asm.y"
    { (yyval.atom) = new_op(0x90, ATOM_TYPE_OP_ARG_I8,  (yyvsp[0].expr)); ;}
    break;

  case 129:
#line 318 "asm.y"
    { (yyval.atom) = new_op(0xb0, ATOM_TYPE_OP_ARG_I8,  (yyvsp[0].expr)); ;}
    break;

  case 130:
#line 319 "asm.y"
    { (yyval.atom) = new_op(0x10, ATOM_TYPE_OP_ARG_I8,  (yyvsp[0].expr)); ;}
    break;

  case 131:
#line 320 "asm.y"
    { (yyval.atom) = new_op(0x30, ATOM_TYPE_OP_ARG_I8,  (yyvsp[0].expr)); ;}
    break;

  case 132:
#line 321 "asm.y"
    { (yyval.atom) = new_op(0x50, ATOM_TYPE_OP_ARG_I8,  (yyvsp[0].expr)); ;}
    break;

  case 133:
#line 322 "asm.y"
    { (yyval.atom) = new_op(0x70, ATOM_TYPE_OP_ARG_I8,  (yyvsp[0].expr)); ;}
    break;

  case 134:
#line 324 "asm.y"
    { (yyval.atom) = new_op0(0xe8); ;}
    break;

  case 135:
#line 325 "asm.y"
    { (yyval.atom) = new_op0(0xca); ;}
    break;

  case 136:
#line 326 "asm.y"
    { (yyval.atom) = new_op0(0xc8); ;}
    break;

  case 137:
#line 327 "asm.y"
    { (yyval.atom) = new_op0(0x88); ;}
    break;

  case 138:
#line 329 "asm.y"
    { (yyval.atom) = new_op(0xe6, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 139:
#line 330 "asm.y"
    { (yyval.atom) = new_op(0xf6, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 140:
#line 331 "asm.y"
    { (yyval.atom) = new_op(0xee, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 141:
#line 332 "asm.y"
    { (yyval.atom) = new_op(0xfe, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 142:
#line 334 "asm.y"
    { (yyval.atom) = new_op(0xc6, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 143:
#line 335 "asm.y"
    { (yyval.atom) = new_op(0xd6, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 144:
#line 336 "asm.y"
    { (yyval.atom) = new_op(0xce, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 145:
#line 337 "asm.y"
    { (yyval.atom) = new_op(0xde, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 146:
#line 339 "asm.y"
    { (yyval.atom) = new_op0(0x4a); ;}
    break;

  case 147:
#line 340 "asm.y"
    { (yyval.atom) = new_op(0x46, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 148:
#line 341 "asm.y"
    { (yyval.atom) = new_op(0x56, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 149:
#line 342 "asm.y"
    { (yyval.atom) = new_op(0x4e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 150:
#line 343 "asm.y"
    { (yyval.atom) = new_op(0x5e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 151:
#line 345 "asm.y"
    { (yyval.atom) = new_op0(0x0a); ;}
    break;

  case 152:
#line 346 "asm.y"
    { (yyval.atom) = new_op(0x06, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 153:
#line 347 "asm.y"
    { (yyval.atom) = new_op(0x16, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 154:
#line 348 "asm.y"
    { (yyval.atom) = new_op(0x0e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 155:
#line 349 "asm.y"
    { (yyval.atom) = new_op(0x1e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 156:
#line 351 "asm.y"
    { (yyval.atom) = new_op0(0x6a); ;}
    break;

  case 157:
#line 352 "asm.y"
    { (yyval.atom) = new_op(0x66, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 158:
#line 353 "asm.y"
    { (yyval.atom) = new_op(0x76, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 159:
#line 354 "asm.y"
    { (yyval.atom) = new_op(0x6e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 160:
#line 355 "asm.y"
    { (yyval.atom) = new_op(0x7e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 161:
#line 357 "asm.y"
    { (yyval.atom) = new_op0(0x2a); ;}
    break;

  case 162:
#line 358 "asm.y"
    { (yyval.atom) = new_op(0x26, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 163:
#line 359 "asm.y"
    { (yyval.atom) = new_op(0x36, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); ;}
    break;

  case 164:
#line 360 "asm.y"
    { (yyval.atom) = new_op(0x2e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 165:
#line 361 "asm.y"
    { (yyval.atom) = new_op(0x3e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); ;}
    break;

  case 166:
#line 363 "asm.y"
    { (yyval.expr) = (yyvsp[0].expr); ;}
    break;

  case 167:
#line 364 "asm.y"
    { (yyval.expr) = (yyvsp[0].expr); ;}
    break;

  case 168:
#line 365 "asm.y"
    { (yyval.expr) = (yyvsp[-2].expr); ;}
    break;

  case 169:
#line 366 "asm.y"
    { (yyval.expr) = (yyvsp[-2].expr); ;}
    break;

  case 170:
#line 367 "asm.y"
    { (yyval.expr) = (yyvsp[0].expr); ;}
    break;

  case 171:
#line 368 "asm.y"
    { (yyval.expr) = (yyvsp[-2].expr); ;}
    break;

  case 172:
#line 369 "asm.y"
    { (yyval.expr) = (yyvsp[-2].expr); ;}
    break;

  case 173:
#line 370 "asm.y"
    { (yyval.expr) = (yyvsp[-3].expr); ;}
    break;

  case 174:
#line 371 "asm.y"
    { (yyval.expr) = (yyvsp[-3].expr); ;}
    break;

  case 175:
#line 373 "asm.y"
    { (yyval.expr) = new_expr_op2(PLUS, (yyvsp[-2].expr), (yyvsp[0].expr)); ;}
    break;

  case 176:
#line 374 "asm.y"
    { (yyval.expr) = new_expr_op2(MINUS, (yyvsp[-2].expr), (yyvsp[0].expr)); ;}
    break;

  case 177:
#line 375 "asm.y"
    { (yyval.expr) = new_expr_op2(MULT, (yyvsp[-2].expr), (yyvsp[0].expr)); ;}
    break;

  case 178:
#line 376 "asm.y"
    { (yyval.expr) = new_expr_op2(DIV, (yyvsp[-2].expr), (yyvsp[0].expr)); ;}
    break;

  case 179:
#line 377 "asm.y"
    { (yyval.expr) = new_expr_op2(MOD, (yyvsp[-2].expr), (yyvsp[0].expr)); ;}
    break;

  case 180:
#line 378 "asm.y"
    { (yyval.expr) = new_expr_op1(vNEG, (yyvsp[0].expr)); ;}
    break;

  case 181:
#line 379 "asm.y"
    { (yyval.expr) = (yyvsp[-1].expr); ;}
    break;

  case 182:
#line 380 "asm.y"
    { (yyval.expr) = new_expr_inclen((yyvsp[-1].str)); ;}
    break;

  case 183:
#line 381 "asm.y"
    {
            (yyval.expr) = new_expr_incword((yyvsp[-3].str), (yyvsp[-1].expr)); ;}
    break;

  case 184:
#line 383 "asm.y"
    { (yyval.expr) = new_expr_number((yyvsp[0].num)); ;}
    break;

  case 185:
#line 384 "asm.y"
    { (yyval.expr) = new_expr_symref((yyvsp[0].str)); ;}
    break;

  case 186:
#line 386 "asm.y"
    { (yyval.expr) = new_expr_op2(LOR, (yyvsp[-2].expr), (yyvsp[0].expr)); ;}
    break;

  case 187:
#line 387 "asm.y"
    { (yyval.expr) = new_expr_op2(LAND, (yyvsp[-2].expr), (yyvsp[0].expr)); ;}
    break;

  case 188:
#line 388 "asm.y"
    { (yyval.expr) = new_expr_op1(LNOT, (yyvsp[0].expr)); ;}
    break;

  case 189:
#line 389 "asm.y"
    { (yyval.expr) = (yyvsp[-1].expr); ;}
    break;

  case 190:
#line 390 "asm.y"
    { (yyval.expr) = new_expr_op2(LT, (yyvsp[-2].expr), (yyvsp[0].expr)); ;}
    break;

  case 191:
#line 391 "asm.y"
    { (yyval.expr) = new_expr_op2(GT, (yyvsp[-2].expr), (yyvsp[0].expr)); ;}
    break;

  case 192:
#line 392 "asm.y"
    { (yyval.expr) = new_expr_op2(EQ, (yyvsp[-2].expr), (yyvsp[0].expr)); ;}
    break;

  case 193:
#line 393 "asm.y"
    { (yyval.expr) = new_expr_op2(NEQ, (yyvsp[-2].expr), (yyvsp[0].expr)); ;}
    break;

  case 194:
#line 395 "asm.y"
    { (yyval.expr) = new_is_defined((yyvsp[-1].str)); ;}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 2607 "asm.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  int yytype = YYTRANSLATE (yychar);
	  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
	  YYSIZE_T yysize = yysize0;
	  YYSIZE_T yysize1;
	  int yysize_overflow = 0;
	  char *yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
	  char *yyfmt;
	  char const *yyf;
	  static char const yyunexpected[] = "syntax error, unexpected %s";
	  static char const yyexpecting[] = ", expecting %s";
	  static char const yyor[] = " or %s";
	  char yyformat[sizeof yyunexpected
			+ sizeof yyexpecting - 1
			+ ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
			   * (sizeof yyor - 1))];
	  char const *yyprefix = yyexpecting;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 1;

	  yyarg[0] = yytname[yytype];
	  yyfmt = yystpcpy (yyformat, yyunexpected);

	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
		  {
		    yycount = 1;
		    yysize = yysize0;
		    yyformat[sizeof yyunexpected - 1] = '\0';
		    break;
		  }
		yyarg[yycount++] = yytname[yyx];
		yysize1 = yysize + yytnamerr (0, yytname[yyx]);
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + yystrlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow && yysize <= YYSTACK_ALLOC_MAXIMUM)
	    yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
		{
		  if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		    {
		      yyp += yytnamerr (yyp, yyarg[yyi++]);
		      yyf += 2;
		    }
		  else
		    {
		      yyp++;
		      yyf++;
		    }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    {
	      yyerror (YY_("syntax error"));
	      goto yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (YY_("syntax error"));
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (0)
     goto yyerrorlab;

yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 397 "asm.y"


int
yyerror (char *s)
{
    fprintf (stderr, "line %d, %s\n", num_lines, s);
    return 0;
}

void asm_set_source(struct membuf *buffer);

int assemble(struct membuf *source, struct membuf *dest)
{
    int val;

    yydebug = 0;
    asm_src_buffer_push(source);
    vec_init(asm_atoms, sizeof(struct atom*));
    val = yyparse();
    if(val == 0)
    {
        output_atoms(dest, asm_atoms);
    }
    parse_free();
    vec_free(asm_atoms, NULL);
    return val;
}

