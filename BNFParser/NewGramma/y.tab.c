/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

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
#define YYBISON_VERSION "2.3"

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
     TERMINAL = 258,
     NONTERMINAL = 259,
     CF = 260,
     DUMMYNONTERMINAL = 261
   };
#endif
/* Tokens.  */
#define TERMINAL 258
#define NONTERMINAL 259
#define CF 260
#define DUMMYNONTERMINAL 261




/* Copy the first part of user declarations.  */
#line 2 "Gramma.y"

	/*********************************************************************/
	// 
	//		Function Gramma y - programmed by Robin Harper (robinh@cse.unsw.edu.au)
	//	    rharper2@mac.com
	//
	//	    Copyright Robin Harper, August 2010.
	//      Its been amended to provide support for DDFs
	//      Ramped H&H and ptc2 (which require some guesswork as to which 
	//      parts of the grammar expand and which terminate)
	//      Enums are now used instead of numbers.
	//		Version 0.3 (of the dynamic grammar form) August 2010
	//
	//	    Based on an original file by:
	//          Gramma v0.63 - programmed by Miguel Nicolau              
	//                                                                   
	//        Gramma.y - written with AT&T yacc specification            
	//                                                                   
	/*********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h> /* Some systems require this */
	

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
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

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 143 "y.tab.c"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
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
      while (YYID (0))
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
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   7

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  8
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  5
/* YYNRULES -- Number of rules.  */
#define YYNRULES  8
/* YYNRULES -- Number of states.  */
#define YYNSTATES  12

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   261

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
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
       2,     2,     2,     2,     7,     2,     2,     2,     2,     2,
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
       5,     6
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,    11,    13,    17,    20
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
       9,     0,    -1,    10,    -1,     9,    10,    -1,     4,    11,
      -1,    12,    -1,    11,     7,    12,    -1,     3,    12,    -1,
       3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    28,    28,    29,    31,    33,    34,    36,    37
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TERMINAL", "NONTERMINAL", "CF",
  "DUMMYNONTERMINAL", "'|'", "$accept", "setOfRules", "rule",
  "transformation", "setOfSymbols", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   124
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,     8,     9,     9,    10,    11,    11,    12,    12
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     1,     3,     2,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     8,     4,     5,     1,     3,     7,
       0,     6
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     3,     5,     6
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -5
static const yytype_int8 yypact[] =
{
      -2,     0,     1,    -5,     0,    -3,    -5,    -5,    -5,    -5,
       0,    -5
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
      -5,    -5,     5,    -5,    -4
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
       9,     7,     1,     4,    10,     1,    11,     8
};

static const yytype_uint8 yycheck[] =
{
       4,     0,     4,     3,     7,     4,    10,     2
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     9,    10,     3,    11,    12,     0,    10,    12,
       7,    12
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
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
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
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
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
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
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
      int yychecklim = YYLAST - yyn + 1;
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
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
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
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
  YYUSE (yyvaluep);

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
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

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
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

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

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

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

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
      
/* Line 1267 of yacc.c.  */
#line 1329 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
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
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
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
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
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
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


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
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 39 "Gramma.y"


typedef struct smallelement{
	struct element *value;
	struct smallelement *next;
	int willTerminate,mightTerminate, mightExpand,willExpand;
} selem;

typedef struct element{
    char *symbolName;
	char *symbolRepresentation;
    int code, type, rtype;
    int willTerminate,mightTerminate, mightExpand,willExpand;
    selem *chain,/* chain of RHS elements */
	*last;/* the last of those elements (to help insertion) */
	int total;/* number of RHS elements following this NONTERMINAL */
    struct element *next;
} elem;

extern FILE *yyin;
extern int symbolsTok(int init, char *text, int *c, int *t, int *total,char *text2);
extern int rhsTok(int init, char *text, int *c,char *text2);
extern int lineNo;
extern char *yytext;
extern int retrieveCodeByName(char *text);
extern elem *changeSymbolText(char *text,char *repr);
extern elem *addSymbol(char *text,char *repr,int type);
extern int chainContains(char *text, int match);
extern int enumSymbolTable(int,char*,int*);
extern int addUnique(char *text);
extern elem *retrieveSymbolByName(char *text,int *type);
extern elem *symbolsTokAt;
void suball(char*,char*,char*);
extern elem *header;
extern selem *branchrhsAt;

int DYNAMICGRAMMAR;


void identifyGrammarRules() {
    selem *s,*branchHead;
    elem *p;
    int changed,found,nonTerms;
    int hasTerminatingBranch,hasExpandingBranch,hasUnknownBranch;
    int funcCode;
	int defunCode;
	int paramCode;
    p = header;
    funcCode = retrieveCodeByName("FUNC");
    if (DYNAMICGRAMMAR && (funcCode == -1)) printf("No functions appear to have been defined\n");
	defunCode = retrieveCodeByName("DEFUN");
	if (DYNAMICGRAMMAR && (defunCode == -1)) printf("No function definition appears to have been allowed e.g. DEFUN\n");
	paramCode = retrieveCodeByName("PARAM");
	if (DYNAMICGRAMMAR && (paramCode == -1)) printf("No PARAM keyword appears to have been used.\n");
	// set the rtype of all terminals to TERMINAL
	while (p) {
        if (p->type == TERMINAL) {
            if (p->code == defunCode || p->code == paramCode) {
				// bit of common theme here, keywords are nonterminals.
                p->rtype = NONTERMINAL;
            } else {
                p->rtype = TERMINAL;
                p->willTerminate = 1;
			}
        }
        else p->rtype = NONTERMINAL;
        p = p->next;
    }
    // identify any non-terminals that only expand to terminals
    do {
        changed = 0;
        p = header;
        while (p) {
            if (p->rtype == NONTERMINAL && (p->code!= funcCode)) {
                s = p->chain;
                while (s)  {
                    found = 1;
                    //if (s->value && ((s->value->rtype != TERMINAL) || (s->value->code == funcCode))) {
                    if (s->value && (s->value->rtype != TERMINAL) ) {
                        found = 0;
                        break;
                    }
                    s = s->next;
                }
                if (found) {
                    p->rtype = TERMINAL;
                    p->willTerminate = 1;
                    changed = 1;
                }
            }
            p =p->next;
		}
    } while(changed); 
	// i.e. until we did a full scan and didnt set anything else to TERMINAL
    // So far so good - now we know what non-terminals are really terminals.
    // We now need to identify the following 
    // for each branch of an expansion we need to know if 
    // It 1) leads to a terminal (willTerminate == 1) // Note started to set with Terminal Identification
    // 2) If it might lead to a terminal (mightTerminate == 1)
    // 3) If it might lead to an expansion (mightExpand == 1)
    // And numbers 2 and 3 are  not mutually exclusive!
    p = header;
    changed = 0;
    while (p) {
        if (p->rtype != TERMINAL) {
            hasTerminatingBranch = 0;
            hasExpandingBranch = 0;
            hasUnknownBranch = 0;
            s = p->chain;
            while (s) {
                // s is now at the beginning of an expansion
                nonTerms = 0;
                branchHead = s;
                while (s && s->value) { // s->value is null when we get to the end of a branch
				    if (s->value->rtype != TERMINAL) {
						nonTerms++;
                    }
					/* // removed, turns out its best to view functions as terminals (I think)
					 if (s->value->code == funcCode) {
					 nonTerms+=2; // ie definitely an expansion by our definition
					 }
					 */
			        if (s->value->chain && s->value->chain->willExpand) {
                        nonTerms+=2;
                    }
                    s = s->next;
                } // we have reached the end of one potential expansion of p
                // if nonTerms == 0 it means that expansion only has Terminals
                if (nonTerms == 0) {
                    branchHead->willTerminate = 1;
                    hasTerminatingBranch = 1;
                    //printf("Terminal branch in %s, branch starts %s, will Terminate set\n",p->symbolName,branchHead->value->symbolName);
                }
                else if (nonTerms >1) {
                    // The branch has more than one non-terminal in it
                    // we will therefore assume it might expand our expression
                    // remember if any of the non-terminals automatically leads
                    // to a terminal, the rtype will also be terminal
                    branchHead->willExpand = 1;
                    hasExpandingBranch = 1;
                    //printf("Expanding branch in %s, branch starts %s, will Expand set\n",p->symbolName,branchHead->value->symbolName);
                } else {
                    branchHead->mightTerminate = 1;
                    branchHead->mightExpand = 1;
                    hasUnknownBranch = 1;
                    //printf("Unknown branch in %s, branch starts %s, mightTerminate and mightExpand set\n",p->symbolName,branchHead->value->symbolName);
                }
                if (s) s=s->next;
            }
            // we have done all the branches now to set the flags
            if (hasTerminatingBranch){
                if (!hasExpandingBranch && !hasUnknownBranch) {
                    p->willTerminate = 1; // Odd that I am here I think
                    //printf("Nonterminal %s will terminate\n",p->symbolName);
                } else {
                    p->mightTerminate = 1;
					//printf("Nonterminal %s might terminate\n",p->symbolName);
                }
            }
            if (hasExpandingBranch) {
                if (!hasTerminatingBranch && !hasUnknownBranch) {
                    p->willExpand = 1;
                    //printf("NonTerminal %s will expand\n",p->symbolName);
                } else {
                    p->mightExpand = 1;
                    //printf("NonTerminal %s might expand\n",p->symbolName);
                }
            }
            if (hasUnknownBranch) {
                p->mightTerminate = 1;
                p->mightExpand = 1;
				//printf("NonTerminal %s has unknown branch, might Terminate might Expand\n",p->symbolName);
            }
        }
        p=p->next;
    }
    
}    



int main(int argc, char **argv){
	char text[25555], text2[25555], output[204855], tmp[25555], *ptr;
	char ctext[25555];
	char gname[25555];
	int code, code2, type, total, caseIndex, index, result;
    int functodo,paramstodo;
    int growcount;
	FILE *f,*in;
	int defunCode, paramCode, funcCode, puseCode,funcBodyTypeCode,cvalCode; 
	int commaCode, funcParTypeCode, oFB,cFB;
	int place,willExpand,mightExpand;
	int defunThisBranch,paramsThisBranch,funcsThisBranch,cvalThisBranch;
	int nonTerminal;
	int forceAdvance;
	int terminateElse;
	elem *lhs,*rhs;
    selem *s;
	DYNAMICGRAMMAR = 1;
	if(argc!=2){
		printf("Function GE grammar parser v0.3\nUsage: %s <file>\n",argv[0]);
		printf("Output will be generated to file <name of grammar>.cpp\n");
		printf("Generates a dynamic grammar from the bnf file, which should ");
		printf("slot into the Dynamic GE system.\n");
		return 0;
    }
	if (!strstr(argv[1],".bnf")) {
		printf("Grammar file must end in .bnf\n");
		return 0;
	}
	yyin = fopen(argv[1],"r");
	strcpy(tmp,argv[1]);
	(*strstr(tmp,".bnf"))=0;
	strcpy(gname,tmp); /* This will become the grammar name */
	strcat(tmp,".h");
	f = fopen(tmp,"w");
	in = fopen("grammar.template.h","r");
	while(fgets(output,120,in)) {
		suball(output,"GRAMMARNAME",gname);
		fprintf(f,"%s",output);
	}
	fclose(in);
	strcpy(tmp,gname);
	strcat(tmp,".cpp");
	f = fopen(tmp,"w");
	
	
	in = fopen("grammar.template.p1","r");
	while(fgets(output,120,in)) {
		suball(output,"GRAMMARNAME",gname);
		fprintf(f,"%s",output);
	}
	fclose(in);
	fprintf(f,"/***************************************************************/\n");
	fprintf(f,"// File generated from grammar included in file:\n");
	fprintf(f,"// %s\n",argv[1]);
	fprintf(f,"/***************************************************************/\n");
	while(fgets(output,120,yyin)){
		fprintf(f,"// %s",output);
	}
	fprintf(f,"/***************************************************************/\n");
    fseek(yyin,0,0);
    addSymbol("code","CODE",NONTERMINAL);
    addSymbol(",","COMMA",TERMINAL);    
    addSymbol("(","oRB",TERMINAL);      
    addSymbol(")","cRB",TERMINAL);      
    addSymbol("[","oSB",TERMINAL);	    
    addSymbol("]","cSB",TERMINAL);	    
    addSymbol("DEFUN","DEFUN",TERMINAL); 
    addSymbol("*(","oFB",TERMINAL);     
    addSymbol(")*","cFB",TERMINAL);     
    addSymbol("{","oCB",TERMINAL);      
    addSymbol("}","cCB",TERMINAL);      
    addSymbol("*{","BEGINEXEC",TERMINAL); 
    addSymbol("}*","ENDEXEC",TERMINAL); 
    addSymbol("+","PLUS",TERMINAL);     
    addSymbol("-","MINUS",TERMINAL); 
    addSymbol("*","MULT",TERMINAL);   
    addSymbol("%","DIV",TERMINAL);    
    addSymbol(";","SEMICOLON",TERMINAL); 
    addSymbol(":","COLON",TERMINAL);
    addSymbol(">","GREATER",TERMINAL);
    addSymbol("<","LESS",TERMINAL); 
    addSymbol("==","EEQUAL",TERMINAL);
    addSymbol("=","EQUAL",TERMINAL);  
    addSymbol("0","ZERO",TERMINAL);		
    addSymbol("1","ONE",TERMINAL);		
    addSymbol("2","TWO",TERMINAL);		
    addSymbol("3","THREE",TERMINAL);	
    addSymbol("4","FOUR",TERMINAL);		
    addSymbol("5","FIVE",TERMINAL);		
    addSymbol("6","SIX",TERMINAL);		
    addSymbol("7","SEVEN",TERMINAL);	
    addSymbol("8","EIGHT",TERMINAL);	
    addSymbol("9","NINE",TERMINAL);		
    addSymbol("NOP","NOP",TERMINAL);
    addSymbol("BEPOPTREE","BEPOPTREE",TERMINAL);
    addSymbol("FUNCPLACEHOLDER","FUNCPLACEHOLDER",TERMINAL);
    addSymbol("PUSEPLACEHOLDER","PUSEPLACEHOLDER",TERMINAL);
    //addSymbol("CVAL","CVAL",TERMINAL);
	do{
		yyparse();
	}   while(!feof(yyin));
	
    if (retrieveSymbolByName("CVAL",&type)) {
        if (type == NONTERMINAL) {
            printf("CVAL is a special keyword it stands for: store the next Codon VALue. Its primary use is where you might need to manipulate numbers.\n");
            printf("One of the problems with just using straight grammar eg Number: DIGIT DIGIT DIGIT, is that would take three codons to store\n");
            printf("And might bias searches since a random cut in the bit stream makes it more likely that a number would be found rather than say an\n");
            printf("expression expansion, so by storing a CVAL you can cut down the number of codons needed, in addition the parse tree crossovers implemented know\n");
            printf("they can ignore CVALs when looking for branches to swap (in theory). So the upshot of this is CVAL must be a terminal, and you are using\n");
            printf("it on the left hand side of a grammar rule, so you will just have to change the name of your rule. If you are not worried about numbers just change\n");
            printf("the name of your rule, if you want to use numbers and are confused look in the regression grammars and interpreters provided to see how\n");
            printf("to use it effectively.\n");
            exit(1);
        }
    }
	
	if (addUnique("POPTREE") == 0) {
		printf("You must not use POPTREE, this is a reserved word.\n");
		exit(1);
	}
	if (addUnique("POPTREED") == 0) {
		printf("You must not use POPTREED, this is a reserved word.\n");
		exit(1);
	}
	cvalCode = retrieveCodeByName("CVAL");

  	if ( (funcCode = retrieveCodeByName("FUNC")) == -1) {
		printf("No FUNC was found, I am assuming that the grammar is not a DYNAMIC GRAMMAR - if you are trying to write a grammar which uses");
		printf(" the dynamically defined function protocols you must use the FUNC keyword (to indicate where function calls can be made.\n");
		DYNAMICGRAMMAR = 0;
	} else {
		DYNAMICGRAMMAR = 1;
		// we are going to rewrite FUNC as _FUNC which allows us to break out all the funky FUNC code and 
		// removes the anal problems if FUNC appeared embedded in a chain (and "expansion")
		if (!changeSymbolText("FUNC","_DOFUNC")) {
			printf("PROBLEM changing the FUNC text ot _FUNC, not sure why as I have already checked that func is defined.\n");
			exit(1);
		}
		// Make a dummy rule _DOFUNC -> _FUNC
		
		lhs = addSymbol("_DOFUNC","_DOFUNC",NONTERMINAL);
		rhs = addSymbol("_FUNC","_FUNC",TERMINAL);
		addRightHandSide(lhs,rhs);
		funcCode = rhs->code;
		if (funcCode == -1) {
			printf("Strange error in trying to rename FUNC as _FUNC and to add _FUNC as an expansion.\n");
			exit(1);
		}
	}
	
	if ( (paramCode = retrieveCodeByName("PARAMS")) == -1) {
		if (DYNAMICGRAMMAR) {
			printf("PARAMS token not declared, you need to provide a rule to construct function parameters called PARAMS e.g. PARAMS: PARAM | PARAM PARAMS\n");
			printf("Use NOP if you want no paramaters declared e.g. PARAMS: NOP | PARAM PARAMS or even PARAMS: NOP | SOMEPARAMS and SOMEPARAMS: PARAM | PARAM PARAMS would work\n");
			exit(1);
		}
	}
	if ( (puseCode = retrieveCodeByName("PUSE")) == -1) {
		if (DYNAMICGRAMMAR) {
			printf("PUSE token not declared, you need to provide a rule which tells us when we can refer to parameter within a function call - that is when the paramater can be used by the function.\n");
			exit(1);
		}
	} else {
		if (DYNAMICGRAMMAR) { // only rewrite PUSE if we doing the dynamically defined function thingy
			// same with PUSE -> _PUSE
			if (!changeSymbolText("PUSE","_DOPUSE")) {
				printf("Problem changing the representation of PUSE to _PUSE\n");
				exit(1);
			}
			lhs = addSymbol("_DOPUSE","_DOPUSE",NONTERMINAL);
			rhs = addSymbol("_PUSE","_PUSE",TERMINAL);
			addRightHandSide(lhs,rhs);
			puseCode = rhs->code;
			if (puseCode == -1) {
				printf("Strange error in rewriting puse as _puse\n");
				exit(1);
			}
		}
	}  
	if ( (funcParTypeCode = retrieveCodeByName("FPARS")) == -1) {
		if (DYNAMICGRAMMAR) {
			printf("You need to define a rule for FPARS, to tell me how the paramaters passed to a function are formed e.g. FPARS: Integer\n");
			exit(1);
		}
	} 
	if ( (funcBodyTypeCode = retrieveCodeByName("FUNCBODY")) == -1) {
		if (DYNAMICGRAMMAR) {
			printf("You need to define a rule for FUNCBODY, to tell me what can appear in the body of a funcation e.g FUNCBODY: line | line FUNCBODY\n");
			exit(1);
		}
	}
	if (retrieveCodeByName("DFVAL") == -1) {
        if (DYNAMICGRAMMAR) {
			printf("You need to define DFVAL - which is the grammar expansion you want ");
			printf("to use if I am going to try and call a function but no functions have been defined! Some examples might be defining DFVAL to be an action or a x value.\n");
			exit(1);
		}
    }
	if (retrieveCodeByName("DPVAL") == -1) {
        if (DYNAMICGRAMMAR) {
			printf("You need to define DPVAL - which is the grammar expansion you want ");
			printf("to use if I am going to try and call a function but no functions have been defined! Some examples might be defining DPVAL to be a NOP or a x value.\n");
			exit(1);
		}
    }
	// fprintf(f,"enum { DEFUN = %d, PARAMS = %d, FUNC = %d, PUSE = %d, FPARS = %d, FUNCBODY = %d};\n",defunCode,paramCode,funcCode,puseCode,funcParTypeCode,funcBodyTypeCode);
	in = fopen("grammar.template.p2","r");
	while(fgets(output,120,in)) {
		suball(output,"GRAMMARNAME",gname);
		fprintf(f,"%s",output);
	}
	fclose(in);
	identifyGrammarRules();
	
	fprintf(f,"\tint expr_t = curr_expr[*curr];\n");
	fprintf(f,"\tswitch(curr_expr[*curr])\n\t\t{\n");
	if (symbolsTok(1, text, &code, &type, &total,text2)) {
		do {
			defunThisBranch = paramsThisBranch = funcsThisBranch = cvalThisBranch = 0;
			forceAdvance = 0;
			fprintf(f,"\t\tcase(%s):{ /* %s */\n",text2,text);
			strcpy(ctext,text); // just copy the text somewhere safe for use at the end
			
            if (( chainContains(text,cvalCode))) {
                cvalThisBranch = 1;
            }
			
			if ( ( place = chainContains(text,defunCode))) {
				fprintf(f,"\t\t\tdivisor = %i;\n",total);    
				if (place == total) fprintf(f,"\t\t\tif (!AllowDefun) divisor--;\n");
				else {
					printf("Ideally your DEFUN *( )* should be on the last branch of a particular expansion.\n");
					printf("This allows the ability to define functions to be easily switched off e.g. not allowing function definitions whilst ");
					printf("defining functions. If your grammar doesn't allow that kind of recursion (ie. PARAMS and FVALUE are defined ");
					printf("in such a way that they cannot encounter a DEFUN - then you can blithely ignore this warning. \n");
					printf("However if your grammar would allow a DEFUN to be encountered whilst ");
					printf("expanding the definition of a function (a function definition within a function), then I am not sure what the results will be.\n");
				}
				defunThisBranch = 1;
			}
			if ( DYNAMICGRAMMAR && (place = chainContains(text,funcCode))) {
				printf("Found the chain %s, which contains code %d\n",text,funcCode);
				funcsThisBranch=1;
				strcpy(tmp,"_");
				strcat(tmp,text2);
				addSymbol(tmp,tmp,DUMMYNONTERMINAL);
				fprintf(f,"\t\t\tif (doingPTC2) {\n");
				fprintf(f,"\t\t\tcurr_expr[0]=FUNCPLACEHOLDER;\n");
				fprintf(f,"\t\t\tbreak;\n");
				fprintf(f,"\t\t\t}\n");
				fprintf(f,"\t\t\tif (expandPTC2) {\n");
				fprintf(f,"\t\t\t\t//cout << \"Hit a dofun with expandPTC2 true\";\n");
				fprintf(f,"\t\t\t\tballoonExpansion++;\n");
				fprintf(f,"\t\t\t\tgrowing=true;\n");
				fprintf(f,"\t\t\t\tmaxDepth=2;\n");
				fprintf(f,"\t\t\t\tmaxGrow = false;\n");
				fprintf(f,"\t\t\t\tif (balloonExpansion == 1) {\n");
				fprintf(f,"\t\t\t\t\tdepth = 1; // so we allow them to expand a bit but not too much\n");
				fprintf(f,"\t\t\t\t\tlastCodonPos = codonPos-1; // prime the pump so to speak\n");
				fprintf(f,"\t\t\t\t}\n");
				fprintf(f,"\t\t\t\t// need to think what happens here with recursion\n");
				
				fprintf(f,"\t\t\t\t//print_grammar_individual(cout,curr_expr,*length);\n");
				fprintf(f,"\t\t\t\t//cout << endl;\n");
				fprintf(f,"\t\t\t\tfor(int i=*length-1;i>*curr;i--)\n");
				fprintf(f,"\t\t\t\t\tcurr_expr[i+1]=curr_expr[i];\n");
				fprintf(f,"\t\t\t\t*length = *length + 1;\n");
				fprintf(f,"\t\t\t\tcurr_expr[*curr]=%s;	// And no functions happen to have been defined. \n",tmp);
				fprintf(f,"\t\t\t\tcurr_expr[*curr+1]=BEPOPTREE;\n");
				fprintf(f,"\t\t\t\t//print_grammar_individual(cout,curr_expr,*length);\n");
				fprintf(f,"\t\t\t\t//cout << endl;\n");
				fprintf(f,"\t\t\t\tbreak;\n");
				fprintf(f,"\t\t\t}\n");
				fprintf(f,"\t\t\t curr_expr[*curr]=%s;\n",tmp);
				fprintf(f,"\t\t\tbreak;\n");
				fprintf(f,"\t\t\t\t}\n");
				fprintf(f,"\t\tcase(%s):{ /* %s */\n",tmp,tmp);
				fprintf(f,"\t\t\texpr_t=%s;\n",text2);
			}	   
			
			
			if (DYNAMICGRAMMAR &&  (place = chainContains(text,puseCode))) {
				paramsThisBranch = 1;
				strcpy(tmp,"_");
				strcat(tmp,text2);
				addSymbol(tmp,tmp,DUMMYNONTERMINAL);
				fprintf(f,"\t\t\tif (doingPTC2) {\n");
				fprintf(f,"\t\t\tcurr_expr[0]=PUSEPLACEHOLDER;\n");
				fprintf(f,"\t\t\tbreak;\n");
				fprintf(f,"\t\t\t}\n");
				fprintf(f,"\t\t\tif (expandPTC2) {\n");
				fprintf(f,"\t\t\t\t//cout << \"Hit a dopuse with expandPTC2 true\";\n");
				fprintf(f,"\t\t\t\tballoonExpansion++;\n");
				fprintf(f,"\t\t\t\tgrowing=true;\n");
				fprintf(f,"\t\t\t\tmaxDepth=2;\n");
				fprintf(f,"\t\t\t\tmaxGrow = false;\n");
				fprintf(f,"\t\t\t\tif (balloonExpansion == 1) {\n");
				fprintf(f,"\t\t\t\t\tdepth = 1; // so we allow them to expand a bit but not too much\n");
				fprintf(f,"\t\t\t\t\tlastCodonPos = codonPos-1; // prime the pump so to speak\n");
				fprintf(f,"\t\t\t\t}\n");
				fprintf(f,"\t\t\t\t// need to think what happens here with recursion\n");
				
				fprintf(f,"\t\t\t\t//print_grammar_individual(cout,curr_expr,*length);\n");
				fprintf(f,"\t\t\t\t//cout << endl;\n");
				fprintf(f,"\t\t\t\tfor(int i=*length-1;i>*curr;i--)\n");
				fprintf(f,"\t\t\t\t\tcurr_expr[i+1]=curr_expr[i];\n");
				fprintf(f,"\t\t\t\t*length = *length + 1;\n");
				fprintf(f,"\t\t\t\tcurr_expr[*curr]=%s;	// And no functions happen to have been defined. \n",tmp);
				fprintf(f,"\t\t\t\tcurr_expr[*curr+1]=BEPOPTREE;\n");
				fprintf(f,"\t\t\t\t//print_grammar_individual(cout,curr_expr,*length);\n");
				fprintf(f,"\t\t\t\t//cout << endl;\n");
				fprintf(f,"\t\t\t\tbreak;\n");
				fprintf(f,"\t\t\t}\n");
				fprintf(f,"\t\t\t curr_expr[*curr]=%s;\n",tmp);
				fprintf(f,"\t\t\tbreak;\n");
				fprintf(f,"\t\t\t\t}\n");
				fprintf(f,"\t\tcase(%s):{ /* %s */\n",tmp,tmp);
				fprintf(f,"\t\t\texpr_t=%s;\n",text2);
			}	   
			
			terminateElse = 0;
			if (funcsThisBranch && paramsThisBranch) {
				printf("Odd the grammar was changed to move FUNC to _FUNC and PUSE to _PUSE and to have each of them\n");
				printf("in a seperate expansion, but I appear to have found both of them in the same expansion\n");
				exit(1);
			}
			if (paramsThisBranch) { 
				fprintf(f,"\t\t\tdivisor = %i;\n",total-1); // we have two "defaults defined" so subtract two.
				fprintf(f,"\t\t\tif (AllowParams) divisor = divisor + noOfParams;\n");
				fprintf(f,"\t\t\tif (divisor == 0) {\t\t// This deals with the situation where no other members of the branch \n");
				index = 2;
				fprintf(f,"\t\t\t\tfor(int i=*length-1;i>*curr;i--)\n");
				fprintf(f,"\t\t\t\t\tcurr_expr[i+%i]=curr_expr[i];\n",index-1);
				fprintf(f,"\t\t\t\t*length = *length + %i;\n",index-1);
				fprintf(f,"\t\t\t\tcurr_expr[*curr]=DPVAL;\t// And no functions/params happen to have been defined. \n");
				fprintf(f,"\t\t\t\tcurr_expr[*curr+%i]=POPTREE;\n",index-1);
				fprintf(f,"\t\t\t\tif (ExtractingGrammarInfo) stack.push_back(codonPos);\n");
				fprintf(f,"\t\t\t} else {\n"); // set terminate else so we put a } after the switch statement.
				terminateElse = 1;// NOTE This means we need a } at the end of the switch statement
				total = total -2;
				forceAdvance = 1; // we have used this codon evem though just now there were no paramaters to choose from
			}
			if (funcsThisBranch) {
				fprintf(f,"\t\t\tdivisor = %i;\n",total-1);    
				fprintf(f,"\t\t\tif (functionNos>0) divisor = divisor + functionNos;\n");
				fprintf(f,"\t\t\tif (divisor == 0) {\t\t// This deals with the situation where no other members of the branch \n");
				index = 2;
				fprintf(f,"\t\t\t\tfor(int i=*length-1;i>*curr;i--)\n");
				fprintf(f,"\t\t\t\t\tcurr_expr[i+%i]=curr_expr[i];\n",index-1);
				fprintf(f,"\t\t\t\t*length = *length + %i;\n",index-1);
				fprintf(f,"\t\t\t\tcurr_expr[*curr]=DFVAL;\t// And no functions happen to have been defined. \n");
				fprintf(f,"\t\t\t\tcurr_expr[*curr+%i]=POPTREE;\n",index-1);
				fprintf(f,"\t\t\t\tif (ExtractingGrammarInfo) stack.push_back(codonPos);\n");
				fprintf(f,"\t\t\t} else { \n"); 
				terminateElse = 1;// NOTE This means we need a } at the end of the switch statement
				total = total -1; /* We automatically define this part, regardless of grammar */
				forceAdvance= 1; // we have used this codon, we must advance even though just now there was no choice of branch
			}
			
			
			if(!(defunThisBranch || funcsThisBranch || paramsThisBranch)) 
				fprintf(f,"\t\t\tdivisor = %i;\n",total);    
			
			//    going to change so that we always insert the growing call, have changed the function to take care
			//    of situation where we might want to terminate but there are only grow options and
			//    vice-versa.
			
			//  if (! (symbolsTokAt->willTerminate || symbolsTokAt->willExpand)) { // so there is a choice here. 
			fprintf(f,"\t\t\tif (growing) {\n");
			fprintf(f,"\t\t\t\tnonTerminals.clear();\n\t\t\t\tterminals.clear();\n");
			growcount =0;
			s = symbolsTokAt->chain;
			functodo = paramstodo = 0;
			while (s) {
				if (s && s->value) {
					if (s->value->code == funcCode) {
						functodo = 1;
					} else if (s->value->code == puseCode) {
						paramstodo =1;
					} else {
						if (s->willTerminate || s->mightTerminate) {
							fprintf(f,"\t\t\t\tterminals.push_back(%d);\n",growcount);
						}
						if (s->willExpand || s->mightExpand) {
							fprintf(f,"\t\t\t\tnonTerminals.push_back(%d);\n",growcount);
						}
						growcount++;
					}
					while (s && s->value) s = s->next; // move to end of branch
					//                     growcount++; // expand branch count
				}
				if (s) s = s->next; // if more branches move to start of next one
			}
			if (functodo || paramstodo) {
				if (functodo) {
					fprintf(f,"\t\t\t\tfor (int i=0;i<functionNos;i++) {\n");
					fprintf(f,"\t\t\t\t\tterminals.push_back(%d+i);\n",growcount);
					fprintf(f,"\t\t\t\t}\n");
				}
				if (paramstodo) {
					fprintf(f,"\t\t\t\tif (AllowParams) {\n");
					fprintf(f,"\t\t\t\t\tfor (int i=0;i<noOfParams;i++) {\n");
					if (functodo) fprintf(f,"\t\t\t\t\t\tterminals.push_back(%d + functionNos + i);\n",growcount);
					else fprintf(f,"\t\t\t\t\t\tterminals.push_back(%d+ i);\n",growcount);
					fprintf(f,"\t\t\t\t\t}\n");
					fprintf(f,"\t\t\t\t}\n");
				}
			}
			fprintf(f,"\t\t\t\tchangeCodonsForGrow(divisor);\n");
			fprintf(f,"\t\t\t\tcurrentCodon = (terminating?0:codons[codonPos]) & CODONMASK;\n");
			fprintf(f,"\t\t\t}\n");
			//    }
            fprintf(f,"\t\t\tswitch(currentCodon%%divisor){\n");
			index = 0;
			nonTerminal =0;
			strcpy(output,"");
			for (caseIndex = 0; caseIndex < total; caseIndex++) {
				if(caseIndex) {
					rhsTok(0,text,&code2,text2);
					//	rhsTok(0,text,&code2);
				}
				else {
					rhsTok(1,text,&code2,text2);
					//	rhsTok(1,text,&code2);
				}
				fprintf(f,"\t\t\t\tcase(%i):{\n",caseIndex);
				do {
                    willExpand = branchrhsAt->willExpand;
					mightExpand = branchrhsAt->mightExpand;
					sprintf(tmp,"\t\t\t\t\tcurr_expr[*curr+%i]=%s; //%s\n",index,text2,text);
					if (code2>1000) nonTerminal++;
					index++;
					if(strlen(output))
						strcat(output,tmp);
					else
						strcpy(output,tmp);
				} while ((result = rhsTok(0,text,&code2,text2)) == 1);
				if (total>1 || forceAdvance) index++; //if (nonTerminal>0) index++;
				if (index > 1) {
					fprintf(f,"\t\t\t\t\tfor(int i=*length-1;i>*curr;i--)\n");
					fprintf(f,"\t\t\t\t\t\tcurr_expr[i+%i]=curr_expr[i];\n",index-1);
					fprintf(f,"\t\t\t\t\t*length = *length + %i;\n",index-1);
					
				}
				fprintf(f,"%s",output);
				if ((total>1) || forceAdvance) { // used to be nonTerminal>0) 
					//- note forceAdvance as we will be reading codon regardless if only one
					if (willExpand || mightExpand) {
						fprintf(f,"\t\t\t\t\tcurr_expr[*curr+%i]=POPTREED;\n",index-1);
						fprintf(f,"\t\t\t\t\tif (growing) depth++;\n");
                    }
				    else fprintf(f,"\t\t\t\t\tcurr_expr[*curr+%i]=POPTREE;\n",index-1);
					fprintf(f,"\t\t\t\t\tif (ExtractingGrammarInfo) stack.push_back(codonPos);\n");
				}
				fprintf(f,"\t\t\t\t\tbreak;\n\t\t\t\t\t}\n");
				index = 0;
				strcpy(output,"");
			}
            if (funcsThisBranch) {
				fprintf(f,"\t\t\t\tdefault:{ // We are going to call a function\n");
				fprintf(f,"\t\t\t\t\tif (functionNos == 0) {\n");
				fprintf(f,"\t\t\t\t\t\tcout << \"Error trying to call a non-extant function\\n\";\n");
				fprintf(f,"\t\t\t\t\t\tbreak;\n");
				fprintf(f,"\t\t\t\t\t}\n");
				fprintf(f,"\t\t\t\t\tint function = (currentCodon%%divisor) - (divisor - functionNos); // work out which function to call\n");
				fprintf(f,"\t\t\t\t\tint fPars = (*funcVector)[function].params;\n");
				fprintf(f,"\t\t\t\t\tint shift = (fPars == 0?3:fPars*2 +2); // need to shift by a number dependent on the paramaters of the function\n");
				fprintf(f,"\t\t\t\t\tfor (int i=*length-1;i>*curr;i--)\n");
				fprintf(f,"\t\t\t\t\t\tcurr_expr[i+shift]=curr_expr[i];\n");
				fprintf(f,"\t\t\t\t\t*length = *length+shift;\n");
				fprintf(f,"\t\t\t\t\tcurr_expr[*curr+0]=500+function;\n");
				fprintf(f,"\t\t\t\t\tcurr_expr[*curr+1]=OFB; //  *( = %i\n",oFB);
				fprintf(f,"\t\t\t\t\tif (fPars > 0) {\n");
				fprintf(f,"\t\t\t\t\t\tcurr_expr[*curr+2]=FPARS; // FPARS = %i\n",funcParTypeCode);
				fprintf(f,"\t\t\t\t\t\tfor (int j=0;j<fPars-1;j++) {\n");
				fprintf(f,"\t\t\t\t\t\t\tcurr_expr[*curr+3+2*j]=COMMA; // , = %i\n",commaCode);
				fprintf(f,"\t\t\t\t\t\t\tcurr_expr[*curr+4+2*j]=FPARS; // FPARS = %i\n",funcParTypeCode);
				fprintf(f,"\t\t\t\t\t\t}\n");
				fprintf(f,"\t\t\t\t\t}\n");
				fprintf(f,"\t\t\t\t\tcurr_expr[*curr+shift-1]=CFB; // )* = %i\n",cFB);
				fprintf(f,"\t\t\t\t\tcurr_expr[*curr+shift]=POPTREE;\n");
				fprintf(f,"\t\t\t\t\tif (ExtractingGrammarInfo) stack.push_back(codonPos);\n");
				fprintf(f,"\t\t\t\t\tbreak;\n\t\t\t\t\t} // of default branch\n");
			} else if (paramsThisBranch) {
				fprintf(f,"\t\t\t\tdefault: { // We are using a paramater\n");
				fprintf(f,"\t\t\t\t\tfor(int i=*length-1;i>*curr;i--)\n");
				fprintf(f,"\t\t\t\t\t\tcurr_expr[i+%i]=curr_expr[i];\n",1);
				fprintf(f,"\t\t\t\t\t*length = *length + %i;\n",1);
				fprintf(f,"\t\t\t\t\tcurr_expr[*curr+0] = 400 + currentCodon%%divisor - (divisor-noOfParams);\n");
				fprintf(f,"\t\t\t\t\tcurr_expr[*curr+1]=POPTREE;\n");
				fprintf(f,"\t\t\t\t\tif (ExtractingGrammarInfo) stack.push_back(codonPos);\n");
				fprintf(f,"\t\t\t\t\tbreak;\n");
				fprintf(f,"\t\t\t\t\t} // of default \n");
			} 
     		fprintf(f,"\t\t\t}//of switch\n");
			if (terminateElse) fprintf(f,"\t\t\t}\n");
			
			if(total>1 || forceAdvance) {
				fprintf(f,"\t\t\tif (!terminating && ExtractingGrammarInfo)\n");
				fprintf(f,"\t\t\t\tcodons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));\n");
				fprintf(f,"\t\t\tcodonPos = codonPos + 1; // query if this should be incremented if terminating?\n");
				forceAdvance = 0;
			}
			fprintf(f,"\t\t\tbreak;\n\t\t\t}//of case %s\n",ctext);
		} while((symbolsTok(0, text, &code, &type, &total,text2))&&(type==NONTERMINAL));
        }
	fprintf(f,"\t\tdefault:{ \n\t\t\t*curr = *curr + 1;\n");
	fprintf(f,"\t\t\t}\n\t\t}//Of switch\n\t}\n");
	fprintf(f,"/***************************************************************/\n");
	fprintf(f,"void %s::print_grammar_individual(ostream& output_stream,int curr_expr[],int length){\n",gname);
	fprintf(f,"\tfor(int i=0;i<length;i++)\n\t\tswitch(curr_expr[i]){\n");
	do{/* Since we've parsed all the NONTERMINALs, the remaining symbols are TERMINALs */
		strcpy(tmp,text);
		if((ptr=strtok(tmp,"\""))){
			/* Search for '"', and replace with '\"' */
			strcpy(text,ptr);
			while((ptr=strtok(NULL,"\""))){
				strcat(text,"\\\"");
				strcat(text,ptr);
			}
		}
		if (code == cvalCode) fprintf(f,"\t\tcase %s: { output_stream << \" %s :\"; i++ ; output_stream << curr_expr[i] << \": \"; break;}\n",text2,text);
		else fprintf(f,"\t\tcase %s: {output_stream << \" %s \";break;}\n",text2,text);
	} while(symbolsTok(0, text, &code, &type, &total,text2));
	fprintf(f,"\t\tdefault: {\n");
	fprintf(f,"\t\t\tif (curr_expr[i]>=400 && curr_expr[i]<500) {\n");
	fprintf(f,"\t\t\t\toutput_stream << \"P\" << curr_expr[i]-400;\n");
	fprintf(f,"\t\t\t} else if (curr_expr[i]>=500 && curr_expr[i]<600) {\n");
	fprintf(f,"\t\t\t\toutput_stream << \"F\" << curr_expr[i]-500;\n");
	fprintf(f,"\t\t\t} else {\n\t\t\t\toutput_stream << \" UNKNOWN(\" << curr_expr[i] << \") \";\n\t\t\t}\n");
	fprintf(f,"\t\t\tbreak;\n\t\t\t} // of default branch\n");
	fprintf(f,"\t\t}\n\toutput_stream << endl;\n}\n");
	fclose(f);
	fclose(yyin);
	strcpy(tmp,gname);
	strcat(tmp,".h");
	f = fopen(tmp,"w");	
	in = fopen("grammar.template.h","r");
	while(fgets(output,120,in)) {
		suball(output,"GRAMMARNAME",gname);
		fprintf(f,"%s",output);
	}
	fclose(in);
	if (enumSymbolTable(1,tmp,&code)) { // the one resest the table
		fprintf(f,"\tenum {\t%s = %d",tmp,code);
		while (enumSymbolTable(0,tmp,&code)) {
			fprintf(f,",\n\t\t%s = %d",tmp,code);
		}
		fprintf(f,"};\n}\n\n\n#endif\n");
	}
	
	fclose(f);
	
	if (enumSymbolTable(1,tmp,&code)) { // the one resest the table
		if (code < 1000) printf("%%token %s\n",tmp);
		while (enumSymbolTable(0,tmp,&code)) {
			if (code < 1000) printf("%%token %s\n",tmp);
		}
	}
	if (symbolsTok(1,text,&code,&type,&total,text2)) 
		do {
			strcpy(output,text);
			strcat(output,":");	
			//printf("// %s\n",output);
			printf("%s: \n\t",text2);
			
			for (caseIndex = 0;caseIndex < total;caseIndex++) {
				strcpy(output,"");
				if (caseIndex) rhsTok(0,text,&code2,text2);
				else rhsTok(1,text,&code2,text2);
				do { 
					printf("%s ",text2);
					strcat(output,text);
					strcat(output," ");
				} while (rhsTok(0,text,&code2,text2) ==1);
				if (caseIndex !=total-1) printf("|");
				//printf("\t//%s\n\t",output);
				printf("\n\t");
				
			}
			printf("\n");
		}
	while(symbolsTok(0,text,&code,&type,&total,text2) && type==NONTERMINAL);
	return 1;
}

int yyerror(char *s){
	fprintf(stderr,"Oops...\nYou have an error on line %i!\n",lineNo);
	if(yytext)
		fprintf(stderr,"Token \"%s\" is out of place or not recognized...\n",yytext);
	exit(0);
}


void suball(char *output,char *from,char *to) {
	char *tmp,*tmp2;
	char buff[256];
	strcpy(buff,output);
	tmp2 = buff;
	tmp = strstr(tmp2,from);
	*output = 0;
	while (tmp) {
		*tmp =0;
		strcat(output,tmp2);
		strcat(output,to);
		tmp2=tmp+strlen(from);
		tmp = strstr(tmp2,from);
	}
	strcat(output,tmp2);
}




