/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     REAL = 258,
     INT = 259,
     NAME = 260,
     STRING = 261,
     RBNULL = 262,
     FALSE = 263,
     TRUE = 264,
     FUNCTION = 265,
     CLASS = 266,
     FOR = 267,
     IN = 268,
     IF = 269,
     ELSE = 270,
     WHILE = 271,
     NEXT = 272,
     BREAK = 273,
     RETURN = 274,
     ARROW_ASSIGN = 275,
     TILDE_ASSIGN = 276,
     TILDEIID_ASSIGN = 277,
     EQUATION_ASSIGN = 278,
     EQUAL = 279,
     AND = 280,
     OR = 281,
     AND2 = 282,
     OR2 = 283,
     GT = 284,
     GE = 285,
     LT = 286,
     LE = 287,
     EQ = 288,
     NE = 289,
     END_OF_INPUT = 290,
     UNOT = 291,
     UPLUS = 292,
     UMINUS = 293,
     UAND = 294
   };
#endif
/* Tokens.  */
#define REAL 258
#define INT 259
#define NAME 260
#define STRING 261
#define RBNULL 262
#define FALSE 263
#define TRUE 264
#define FUNCTION 265
#define CLASS 266
#define FOR 267
#define IN 268
#define IF 269
#define ELSE 270
#define WHILE 271
#define NEXT 272
#define BREAK 273
#define RETURN 274
#define ARROW_ASSIGN 275
#define TILDE_ASSIGN 276
#define TILDEIID_ASSIGN 277
#define EQUATION_ASSIGN 278
#define EQUAL 279
#define AND 280
#define OR 281
#define AND2 282
#define OR2 283
#define GT 284
#define GE 285
#define LT 286
#define LE 287
#define EQ 288
#define NE 289
#define END_OF_INPUT 290
#define UNOT 291
#define UPLUS 292
#define UMINUS 293
#define UAND 294




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 80 "./grammar.y"
{
    char*                                           string;
    double                                          realValue;
    int                                             intValue;
    bool                                            boolValue;
    RbString*                                       idString;
    SyntaxElement*                                  syntaxElement;
    SyntaxVariable*                                 syntaxVariable;
    SyntaxFunctionCall*                             syntaxFunctionCall;
    SyntaxLabeledExpr*                              syntaxLabeledExpr;
    SyntaxFormal*                                   syntaxFormal;
    std::list<SyntaxElement*>*                      syntaxElementList;
    std::list<SyntaxLabeledExpr*>*                  argumentList;
    std::list<SyntaxFormal*>*                       formalList;
}
/* Line 1529 of yacc.c.  */
#line 143 "./grammar.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;
