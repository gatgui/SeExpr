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
#ifndef SeExprYYTOKENTYPE
# define SeExprYYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum SeExpr2tokentype {
     IF = 258,
     ELSE = 259,
     EXTERN = 260,
     DEF = 261,
     FLOATPOINT = 262,
     STRING = 263,
     NAME = 264,
     VAR = 265,
     STR = 266,
     NUMBER = 267,
     LIFETIME_CONSTANT = 268,
     LIFETIME_UNIFORM = 269,
     LIFETIME_VARYING = 270,
     LIFETIME_ERROR = 271,
     AddEq = 272,
     SubEq = 273,
     MultEq = 274,
     DivEq = 275,
     ExpEq = 276,
     ModEq = 277,
     ARROW = 278,
     OR = 279,
     AND = 280,
     NE = 281,
     EQ = 282,
     SEEXPR_GE = 283,
     SEEXPR_LE = 284,
     UNARY = 285
   };
#endif
/* Tokens.  */
#define IF 258
#define ELSE 259
#define EXTERN 260
#define DEF 261
#define FLOATPOINT 262
#define STRING 263
#define NAME 264
#define VAR 265
#define STR 266
#define NUMBER 267
#define LIFETIME_CONSTANT 268
#define LIFETIME_UNIFORM 269
#define LIFETIME_VARYING 270
#define LIFETIME_ERROR 271
#define AddEq 272
#define SubEq 273
#define MultEq 274
#define DivEq 275
#define ExpEq 276
#define ModEq 277
#define ARROW 278
#define OR 279
#define AND 280
#define NE 281
#define EQ 282
#define SEEXPR_GE 283
#define SEEXPR_LE 284
#define UNARY 285




#if ! defined SeExprYYSTYPE && ! defined SeExprYYSTYPE_IS_DECLARED
typedef union SeExprYYSTYPE
#line 77 "src/SeExpr/ExprParser.y"
{
    SeExpr2::ExprNode* n; /* a node is returned for all non-terminals to
		      build the parse tree from the leaves up. */
    double d;      // return value for number tokens
    char* s;       /* return value for name tokens.  Note: the string
		      is allocated with strdup() in the lexer and must
		      be freed with free() */
    struct {
        SeExpr2::ExprType::Type     type;
        int                  dim;
        SeExpr2::ExprType::Lifetime lifetime;
    } t;  // return value for types
    SeExpr2::ExprType::Lifetime l; // return value for lifetime qualifiers
}
/* Line 1529 of yacc.c.  */
#line 124 "y.tab.h"
	SeExprYYSTYPE;
# define SeExpr2stype SeExprYYSTYPE /* obsolescent; will be withdrawn */
# define SeExprYYSTYPE_IS_DECLARED 1
# define SeExprYYSTYPE_IS_TRIVIAL 1
#endif

extern SeExprYYSTYPE SeExpr2lval;

#if ! defined SeExprYYLTYPE && ! defined SeExprYYLTYPE_IS_DECLARED
typedef struct SeExprYYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} SeExprYYLTYPE;
# define SeExpr2ltype SeExprYYLTYPE /* obsolescent; will be withdrawn */
# define SeExprYYLTYPE_IS_DECLARED 1
# define SeExprYYLTYPE_IS_TRIVIAL 1
#endif

extern SeExprYYLTYPE SeExpr2lloc;
