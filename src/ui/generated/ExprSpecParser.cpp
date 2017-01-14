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

/* All symbols defined below should begin with SeExpr2Spec or SeExprSpecYY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define SeExprSpecYYBISON 1

/* Bison version.  */
#define SeExprSpecYYBISON_VERSION "2.3"

/* Skeleton name.  */
#define SeExprSpecYYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define SeExprSpecYYPURE 0

/* Using locations.  */
#define SeExprSpecYYLSP_NEEDED 1

/* Substitute the variable and function names.  */
#define SeExpr2Specparse SeExpr2Specparse
#define SeExpr2Speclex   SeExpr2Speclex
#define SeExpr2Specerror SeExpr2Specerror
#define SeExpr2Speclval  SeExpr2Speclval
#define SeExpr2Specchar  SeExpr2Specchar
#define SeExpr2Specdebug SeExpr2Specdebug
#define SeExpr2Specnerrs SeExpr2Specnerrs
#define SeExpr2Speclloc SeExpr2Speclloc

/* Tokens.  */
#ifndef SeExprSpecYYTOKENTYPE
# define SeExprSpecYYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum SeExpr2Spectokentype {
     IF = 258,
     ELSE = 259,
     NAME = 260,
     VAR = 261,
     STR = 262,
     NUMBER = 263,
     AddEq = 264,
     SubEq = 265,
     MultEq = 266,
     DivEq = 267,
     ExpEq = 268,
     ModEq = 269,
     ARROW = 270,
     OR = 271,
     AND = 272,
     NE = 273,
     EQ = 274,
     GE = 275,
     LE = 276,
     UNARY = 277
   };
#endif
/* Tokens.  */
#define IF 258
#define ELSE 259
#define NAME 260
#define VAR 261
#define STR 262
#define NUMBER 263
#define AddEq 264
#define SubEq 265
#define MultEq 266
#define DivEq 267
#define ExpEq 268
#define ModEq 269
#define ARROW 270
#define OR 271
#define AND 272
#define NE 273
#define EQ 274
#define GE 275
#define LE 276
#define UNARY 277




/* Copy the first part of user declarations.  */
#line 18 "src/ui/ExprSpecParser.y"

#include <algorithm>
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <typeinfo>
#ifdef SEEXPR_USE_ANIMLIB
#include <animlib/AnimCurve.h>
#include <animlib/AnimKeyframe.h>
#else
#define UNUSED(x) (void)(x)
#endif
#include <SeExpr2/Platform.h>
#include <SeExpr2/Mutex.h>
#include "ExprSpecType.h"
#include "Editable.h"
#include "ExprDeepWater.h"


/******************
 lexer declarations
 ******************/


#define SPEC_IS_NUMBER(x) \
    (dynamic_cast<ExprSpecScalarNode*>(x) != 0)
#define SPEC_IS_VECTOR(x) \
    (dynamic_cast<ExprSpecVectorNode*>(x) != 0)
#define SPEC_IS_STR(x) \
    (dynamic_cast<ExprSpecStringNode*>(x) != 0)

// declarations of functions and data in ExprParser.y
int SeExpr2Speclex();
int SeExpr2Specpos();
extern int SeExpr2Spec_start;
extern char* SeExpr2Spectext;
struct SeExpr2Spec_buffer_state;
SeExpr2Spec_buffer_state* SeExpr2Spec_scan_string(const char *str);
void SeExpr2Spec_delete_buffer(SeExpr2Spec_buffer_state*);

//#####################################
// Keep track of mini parse tree nodes

// temporary to the parse... all pointers deleted at end of parse
static std::vector<ExprSpecNode*> specNodes;
/// Remember the spec node, so we can delete it later
static ExprSpecNode* remember(ExprSpecNode* node)
{specNodes.push_back(node);return node;}


/// list of strings duplicated by lexer to avoid error mem leak
static std::vector<char*> tokens;

char* specRegisterToken(char* rawString)
{
    char* tok=strdup(rawString);
    tokens.push_back(tok);
    return tok;
}

//######################################################################
// Expose parser API inputs/outputs to yacc as statics

// these are pointers to the arguments send into parse API
// made static here so the parser can see them in yacc actions
static std::vector<Editable*>* editables;
static std::vector<std::string>* variables;

static const char* ParseStr;    // string being parsed
static std::string ParseError;  // error (set from SeExpr2Specerror)
static ExprSpecNode* ParseResult; // must set result here since SeExpr2Specparse can't return it


//######################################################################
// Helpers used by actions to register data


/// Remember that there is an assignment to this variable (For autocomplete)
static void specRegisterVariable(const char* var)
{
    variables->push_back(var);
}

/// Variable Assignment/String literal should be turned into an editable
/// an editable is the data part of a control (it's model essentially)
static void specRegisterEditable(const char* var,ExprSpecNode* node)
{
    //std::cerr<<"we have editable var "<<var<<std::endl;
    if(!node){
        //std::cerr<<"   null ptr "<<var<<std::endl;
    }else if(ExprSpecScalarNode* n=dynamic_cast<ExprSpecScalarNode*>(node)){
        editables->push_back(new NumberEditable(var,node->startPos,node->endPos,n->v));
    }else if(ExprSpecVectorNode* n=dynamic_cast<ExprSpecVectorNode*>(node)){
        editables->push_back(new VectorEditable(var,node->startPos,node->endPos,n->v));
    }else if(ExprSpecStringNode* n=dynamic_cast<ExprSpecStringNode*>(node)){
        editables->push_back(new StringEditable(node->startPos,node->endPos,n->v));
    }else if(ExprSpecCCurveNode* n=dynamic_cast<ExprSpecCCurveNode*>(node)){
        if(ExprSpecListNode* args=dynamic_cast<ExprSpecListNode*>(n->args)){
            if((args->nodes.size())%3==0){
                ColorCurveEditable* ccurve=new ColorCurveEditable(var,node->startPos,node->endPos);
                bool valid=true;
                for(size_t i=0;i<args->nodes.size();i+=3){
                    ExprSpecScalarNode* xnode=dynamic_cast<ExprSpecScalarNode*>(args->nodes[i]);
                    ExprSpecVectorNode* ynode=dynamic_cast<ExprSpecVectorNode*>(args->nodes[i+1]);
                    ExprSpecScalarNode* interpnode=dynamic_cast<ExprSpecScalarNode*>(args->nodes[i+2]);
                    if(xnode && ynode && interpnode){
                        ccurve->add(xnode->v,ynode->v,interpnode->v);
                    }else{
                        valid=false;
                    }
                }
                if(valid) editables->push_back(ccurve);
                else delete ccurve;
            }else{
                //std::cerr<<"Curve has wrong # of args"<<args->nodes.size()<<std::endl;
            }
        }
    }else if(ExprSpecCurveNode* n=dynamic_cast<ExprSpecCurveNode*>(node)){
        if(ExprSpecListNode* args=dynamic_cast<ExprSpecListNode*>(n->args)){
            if((args->nodes.size())%3==0){
                CurveEditable* ccurve=new CurveEditable(var,node->startPos,node->endPos);
                bool valid=true;
                for(size_t i=0;i<args->nodes.size();i+=3){
                    ExprSpecScalarNode* xnode=dynamic_cast<ExprSpecScalarNode*>(args->nodes[i]);
                    ExprSpecScalarNode* ynode=dynamic_cast<ExprSpecScalarNode*>(args->nodes[i+1]);
                    ExprSpecScalarNode* interpnode=dynamic_cast<ExprSpecScalarNode*>(args->nodes[i+2]);
                    if(xnode && ynode && interpnode){
                        ccurve->add(xnode->v,ynode->v,interpnode->v);
                    }else{
                        valid=false;
                    }
                }
                if(valid) editables->push_back(ccurve);
                else{
                    delete ccurve;
                }
            }
        }
    }else if(ExprSpecColorSwatchNode* n=dynamic_cast<ExprSpecColorSwatchNode*>(node)){
        if(ExprSpecListNode* args=dynamic_cast<ExprSpecListNode*>(n->args)){
            if(args->nodes.size()>0){
                ColorSwatchEditable* swatch=new ColorSwatchEditable(var,node->startPos,node->endPos);
                bool valid=true;
                for(size_t i=0;i<args->nodes.size();i++){
                    ExprSpecVectorNode* colornode=dynamic_cast<ExprSpecVectorNode*>(args->nodes[i]);
                    if(colornode){
                        swatch->add(colornode->v);
                    }else{
                        valid=false;
                    }
                }
                if(valid) editables->push_back(swatch);
                else delete swatch;
            }
        }
    }else if(ExprSpecAnimCurveNode* n=dynamic_cast<ExprSpecAnimCurveNode*>(node)){
        if(ExprSpecListNode* args=dynamic_cast<ExprSpecListNode*>(n->args)){
            // need 3 items for pre inf and post inf and weighting, plus 9 items per key
            if((args->nodes.size()-4)%9==0){
                AnimCurveEditable* animCurve=new AnimCurveEditable(var,node->startPos,node->endPos);
                bool valid=true;


#ifdef SEEXPR_USE_ANIMLIB
                if(ExprSpecStringNode* s=dynamic_cast<ExprSpecStringNode*>(args->nodes[0])){
                    animCurve->curve.setPreInfinity(animlib::AnimCurve::stringToInfinityType(s->v));
                }else valid=false;
                if(ExprSpecStringNode* s=dynamic_cast<ExprSpecStringNode*>(args->nodes[1])){
                    animCurve->curve.setPostInfinity(animlib::AnimCurve::stringToInfinityType(s->v));
                }else valid=false;
                if(ExprSpecScalarNode* v=dynamic_cast<ExprSpecScalarNode*>(args->nodes[2])){
                    animCurve->curve.setWeighted(bool(v->v));
                }
                if(ExprSpecStringNode* v=dynamic_cast<ExprSpecStringNode*>(args->nodes[3])){
                    animCurve->link=v->v;
                }

                for(size_t i=4;i<args->nodes.size();i+=9){
                    ExprSpecScalarNode* xnode=dynamic_cast<ExprSpecScalarNode*>(args->nodes[i]);
                    ExprSpecScalarNode* ynode=dynamic_cast<ExprSpecScalarNode*>(args->nodes[i+1]);
                    ExprSpecScalarNode* inWeight=dynamic_cast<ExprSpecScalarNode*>(args->nodes[i+2]);
                    ExprSpecScalarNode* outWeight=dynamic_cast<ExprSpecScalarNode*>(args->nodes[i+3]);
                    ExprSpecScalarNode* inAngle=dynamic_cast<ExprSpecScalarNode*>(args->nodes[i+4]);
                    ExprSpecScalarNode* outAngle=dynamic_cast<ExprSpecScalarNode*>(args->nodes[i+5]);
                    ExprSpecStringNode* inTangType=dynamic_cast<ExprSpecStringNode*>(args->nodes[i+6]);
                    ExprSpecStringNode* outTangType=dynamic_cast<ExprSpecStringNode*>(args->nodes[i+7]);
                    ExprSpecScalarNode* weighted=dynamic_cast<ExprSpecScalarNode*>(args->nodes[i+8]);
                    if(xnode && ynode && inWeight && outWeight && inAngle && outAngle && inTangType && outTangType ){
                        animlib::AnimKeyframe key(xnode->v,ynode->v);
                        key.setInWeight(inWeight->v);
                        key.setOutWeight(outWeight->v);
                        key.setInAngle(inAngle->v);
                        key.setOutAngle(outAngle->v);
                        key.setInTangentType(animlib::AnimKeyframe::stringToTangentType(inTangType->v));
                        key.setOutTangentType(animlib::AnimKeyframe::stringToTangentType(outTangType->v));
                        key.setWeightsLocked(weighted->v);
                        animCurve->curve.addKey(key);
                    }else{
                        valid=false;
                    }
                }
                if(valid) editables->push_back(animCurve);
                else delete animCurve;
#else
                UNUSED(animCurve);
                UNUSED(valid);
#endif
            }
        }
    }else if(ExprSpecDeepWaterNode* n=dynamic_cast<ExprSpecDeepWaterNode*>(node)){
        if(ExprSpecListNode* args=dynamic_cast<ExprSpecListNode*>(n->args)){
            if(args->nodes.size()==12){
                DeepWaterEditable* deepWater=new DeepWaterEditable(var,node->startPos,node->endPos);
                bool valid=true;

                ExprSpecScalarNode* resolution=dynamic_cast<ExprSpecScalarNode*>(args->nodes[0]);
                ExprSpecScalarNode* tileSize=dynamic_cast<ExprSpecScalarNode*>(args->nodes[1]);
                ExprSpecScalarNode* lengthCutoff=dynamic_cast<ExprSpecScalarNode*>(args->nodes[2]);
                ExprSpecScalarNode* amplitude=dynamic_cast<ExprSpecScalarNode*>(args->nodes[3]);
                ExprSpecScalarNode* windAngle=dynamic_cast<ExprSpecScalarNode*>(args->nodes[4]);
                ExprSpecScalarNode* windSpeed=dynamic_cast<ExprSpecScalarNode*>(args->nodes[5]);
                ExprSpecScalarNode* directionalFactorExponent=dynamic_cast<ExprSpecScalarNode*>(args->nodes[6]);
                ExprSpecScalarNode* directionalReflectionDamping=dynamic_cast<ExprSpecScalarNode*>(args->nodes[7]);
                ExprSpecVectorNode* flowDirection=dynamic_cast<ExprSpecVectorNode*>(args->nodes[8]);
                ExprSpecScalarNode* sharpen=dynamic_cast<ExprSpecScalarNode*>(args->nodes[9]);
                ExprSpecScalarNode* time=dynamic_cast<ExprSpecScalarNode*>(args->nodes[10]);
                ExprSpecScalarNode* filterWidth=dynamic_cast<ExprSpecScalarNode*>(args->nodes[11]);
                if(resolution && tileSize && lengthCutoff && amplitude && windAngle && windSpeed && directionalFactorExponent && directionalReflectionDamping && flowDirection && sharpen && time && filterWidth){
                    deepWater->setParams(SeDeepWaterParams(resolution->v, tileSize->v, lengthCutoff->v, amplitude->v, windAngle->v, windSpeed->v, directionalFactorExponent->v, directionalReflectionDamping->v, flowDirection->v, sharpen->v, time->v, filterWidth->v));
                }else{
                    valid=false;
                }

                if(valid) editables->push_back(deepWater);
                else delete deepWater;
            }
        }
    }else{
        std::cerr<<"SEEXPREDITOR LOGIC ERROR: We didn't recognize the Spec"<<std::endl;
    }
}


/*******************
 parser declarations
 *******************/

// forward declaration
static void SeExpr2Specerror(const char* msg);



/* Enabling traces.  */
#ifndef SeExprSpecYYDEBUG
# define SeExprSpecYYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef SeExprSpecYYERROR_VERBOSE
# undef SeExprSpecYYERROR_VERBOSE
# define SeExprSpecYYERROR_VERBOSE 1
#else
# define SeExprSpecYYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef SeExprSpecYYTOKEN_TABLE
# define SeExprSpecYYTOKEN_TABLE 0
#endif

#if ! defined SeExprSpecYYSTYPE && ! defined SeExprSpecYYSTYPE_IS_DECLARED
typedef union SeExprSpecYYSTYPE
#line 271 "src/ui/ExprSpecParser.y"
{
    ExprSpecNode* n;
    double d;      // return value for number tokens
    char* s;       /* return value for name tokens.  Note: UNLIKE the regular parser, this is not strdup()'dthe string */
}
/* Line 193 of yacc.c.  */
#line 407 "y.tab.c"
	SeExprSpecYYSTYPE;
# define SeExpr2Specstype SeExprSpecYYSTYPE /* obsolescent; will be withdrawn */
# define SeExprSpecYYSTYPE_IS_DECLARED 1
# define SeExprSpecYYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined SeExprSpecYYLTYPE && ! defined SeExprSpecYYLTYPE_IS_DECLARED
typedef struct SeExprSpecYYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} SeExprSpecYYLTYPE;
# define SeExpr2Specltype SeExprSpecYYLTYPE /* obsolescent; will be withdrawn */
# define SeExprSpecYYLTYPE_IS_DECLARED 1
# define SeExprSpecYYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 432 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef SeExprSpecYYTYPE_UINT8
typedef SeExprSpecYYTYPE_UINT8 SeExpr2Spectype_uint8;
#else
typedef unsigned char SeExpr2Spectype_uint8;
#endif

#ifdef SeExprSpecYYTYPE_INT8
typedef SeExprSpecYYTYPE_INT8 SeExpr2Spectype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char SeExpr2Spectype_int8;
#else
typedef short int SeExpr2Spectype_int8;
#endif

#ifdef SeExprSpecYYTYPE_UINT16
typedef SeExprSpecYYTYPE_UINT16 SeExpr2Spectype_uint16;
#else
typedef unsigned short int SeExpr2Spectype_uint16;
#endif

#ifdef SeExprSpecYYTYPE_INT16
typedef SeExprSpecYYTYPE_INT16 SeExpr2Spectype_int16;
#else
typedef short int SeExpr2Spectype_int16;
#endif

#ifndef SeExprSpecYYSIZE_T
# ifdef __SIZE_TYPE__
#  define SeExprSpecYYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define SeExprSpecYYSIZE_T size_t
# elif ! defined SeExprSpecYYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define SeExprSpecYYSIZE_T size_t
# else
#  define SeExprSpecYYSIZE_T unsigned int
# endif
#endif

#define SeExprSpecYYSIZE_MAXIMUM ((SeExprSpecYYSIZE_T) -1)

#ifndef SeExprSpecYY_
# if defined SeExprSpecYYENABLE_NLS && SeExprSpecYYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define SeExprSpecYY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef SeExprSpecYY_
#  define SeExprSpecYY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define SeExprSpecYYUSE(e) ((void) (e))
#else
# define SeExprSpecYYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define SeExprSpecYYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
SeExprSpecYYID (int i)
#else
static int
SeExprSpecYYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined SeExpr2Specoverflow || SeExprSpecYYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef SeExprSpecYYSTACK_USE_ALLOCA
#  if SeExprSpecYYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define SeExprSpecYYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define SeExprSpecYYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define SeExprSpecYYSTACK_ALLOC alloca
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

# ifdef SeExprSpecYYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define SeExprSpecYYSTACK_FREE(Ptr) do { /* empty */; } while (SeExprSpecYYID (0))
#  ifndef SeExprSpecYYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define SeExprSpecYYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define SeExprSpecYYSTACK_ALLOC SeExprSpecYYMALLOC
#  define SeExprSpecYYSTACK_FREE SeExprSpecYYFREE
#  ifndef SeExprSpecYYSTACK_ALLOC_MAXIMUM
#   define SeExprSpecYYSTACK_ALLOC_MAXIMUM SeExprSpecYYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined SeExprSpecYYMALLOC || defined malloc) \
	     && (defined SeExprSpecYYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef SeExprSpecYYMALLOC
#   define SeExprSpecYYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (SeExprSpecYYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef SeExprSpecYYFREE
#   define SeExprSpecYYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined SeExpr2Specoverflow || SeExprSpecYYERROR_VERBOSE */


#if (! defined SeExpr2Specoverflow \
     && (! defined __cplusplus \
	 || (defined SeExprSpecYYLTYPE_IS_TRIVIAL && SeExprSpecYYLTYPE_IS_TRIVIAL \
	     && defined SeExprSpecYYSTYPE_IS_TRIVIAL && SeExprSpecYYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union SeExpr2Specalloc
{
  SeExpr2Spectype_int16 SeExpr2Specss;
  SeExprSpecYYSTYPE SeExpr2Specvs;
    SeExprSpecYYLTYPE SeExpr2Specls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define SeExprSpecYYSTACK_GAP_MAXIMUM (sizeof (union SeExpr2Specalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define SeExprSpecYYSTACK_BYTES(N) \
     ((N) * (sizeof (SeExpr2Spectype_int16) + sizeof (SeExprSpecYYSTYPE) + sizeof (SeExprSpecYYLTYPE)) \
      + 2 * SeExprSpecYYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef SeExprSpecYYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define SeExprSpecYYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define SeExprSpecYYCOPY(To, From, Count)		\
      do					\
	{					\
	  SeExprSpecYYSIZE_T SeExpr2Speci;				\
	  for (SeExpr2Speci = 0; SeExpr2Speci < (Count); SeExpr2Speci++)	\
	    (To)[SeExpr2Speci] = (From)[SeExpr2Speci];		\
	}					\
      while (SeExprSpecYYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables SeExprSpecYYSIZE and SeExprSpecYYSTACKSIZE give the old and new number of
   elements in the stack, and SeExprSpecYYPTR gives the new location of the
   stack.  Advance SeExprSpecYYPTR to a properly aligned location for the next
   stack.  */
# define SeExprSpecYYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	SeExprSpecYYSIZE_T SeExpr2Specnewbytes;						\
	SeExprSpecYYCOPY (&SeExpr2Specptr->Stack, Stack, SeExpr2Specsize);				\
	Stack = &SeExpr2Specptr->Stack;						\
	SeExpr2Specnewbytes = SeExpr2Specstacksize * sizeof (*Stack) + SeExprSpecYYSTACK_GAP_MAXIMUM; \
	SeExpr2Specptr += SeExpr2Specnewbytes / sizeof (*SeExpr2Specptr);				\
      }									\
    while (SeExprSpecYYID (0))

#endif

/* SeExprSpecYYFINAL -- State number of the termination state.  */
#define SeExprSpecYYFINAL  40
/* SeExprSpecYYLAST -- Last index in SeExprSpecYYTABLE.  */
#define SeExprSpecYYLAST   693

/* SeExprSpecYYNTOKENS -- Number of terminals.  */
#define SeExprSpecYYNTOKENS  44
/* SeExprSpecYYNNTS -- Number of nonterminals.  */
#define SeExprSpecYYNNTS  11
/* SeExprSpecYYNRULES -- Number of rules.  */
#define SeExprSpecYYNRULES  59
/* SeExprSpecYYNRULES -- Number of states.  */
#define SeExprSpecYYNSTATES  139

/* SeExprSpecYYTRANSLATE(SeExprSpecYYLEX) -- Bison symbol number corresponding to SeExprSpecYYLEX.  */
#define SeExprSpecYYUNDEFTOK  2
#define SeExprSpecYYMAXUTOK   277

#define SeExprSpecYYTRANSLATE(SeExprSpecYYX)						\
  ((unsigned int) (SeExprSpecYYX) <= SeExprSpecYYMAXUTOK ? SeExpr2Spectranslate[SeExprSpecYYX] : SeExprSpecYYUNDEFTOK)

/* SeExprSpecYYTRANSLATE[SeExprSpecYYLEX] -- Bison symbol number corresponding to SeExprSpecYYLEX.  */
static const SeExpr2Spectype_uint8 SeExpr2Spectranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    33,     2,     2,     2,    32,     2,     2,
      15,    16,    30,    28,    42,    29,     2,    31,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    18,    39,
      24,    38,    25,    19,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    37,     2,    43,    36,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    40,     2,    41,    34,     2,     2,     2,
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
      17,    20,    21,    22,    23,    26,    27,    35
};

#if SeExprSpecYYDEBUG
/* SeExprSpecYYPRHS[SeExprSpecYYN] -- Index of the first RHS symbol of rule number SeExprSpecYYN in
   SeExprSpecYYRHS.  */
static const SeExpr2Spectype_uint8 SeExpr2Specprhs[] =
{
       0,     0,     3,     6,     8,     9,    11,    13,    16,    18,
      23,    28,    33,    38,    43,    48,    53,    58,    63,    68,
      73,    78,    83,    88,    97,    98,   103,   106,   110,   118,
     123,   129,   133,   137,   141,   145,   149,   153,   157,   161,
     164,   167,   170,   173,   177,   181,   185,   189,   193,   197,
     202,   209,   211,   213,   215,   216,   218,   220,   224,   226
};

/* SeExprSpecYYRHS -- A `-1'-separated list of the rules' RHS.  */
static const SeExpr2Spectype_int8 SeExpr2Specrhs[] =
{
      45,     0,    -1,    47,    51,    -1,    51,    -1,    -1,    47,
      -1,    48,    -1,    47,    48,    -1,    49,    -1,     6,    38,
      51,    39,    -1,     6,     9,    51,    39,    -1,     6,    10,
      51,    39,    -1,     6,    11,    51,    39,    -1,     6,    12,
      51,    39,    -1,     6,    13,    51,    39,    -1,     6,    14,
      51,    39,    -1,     5,    38,    51,    39,    -1,     5,     9,
      51,    39,    -1,     5,    10,    51,    39,    -1,     5,    11,
      51,    39,    -1,     5,    12,    51,    39,    -1,     5,    13,
      51,    39,    -1,     5,    14,    51,    39,    -1,     3,    15,
      51,    16,    40,    46,    41,    50,    -1,    -1,     4,    40,
      46,    41,    -1,     4,    49,    -1,    15,    51,    16,    -1,
      37,    51,    42,    51,    42,    51,    43,    -1,    51,    37,
      51,    43,    -1,    51,    19,    51,    18,    51,    -1,    51,
      20,    51,    -1,    51,    21,    51,    -1,    51,    23,    51,
      -1,    51,    22,    51,    -1,    51,    24,    51,    -1,    51,
      25,    51,    -1,    51,    27,    51,    -1,    51,    26,    51,
      -1,    28,    51,    -1,    29,    51,    -1,    33,    51,    -1,
      34,    51,    -1,    51,    28,    51,    -1,    51,    29,    51,
      -1,    51,    30,    51,    -1,    51,    31,    51,    -1,    51,
      32,    51,    -1,    51,    36,    51,    -1,     5,    15,    52,
      16,    -1,    51,    17,     5,    15,    52,    16,    -1,     6,
      -1,     5,    -1,     8,    -1,    -1,    53,    -1,    54,    -1,
      53,    42,    54,    -1,    51,    -1,     7,    -1
};

/* SeExprSpecYYRLINE[SeExprSpecYYN] -- source line where rule number SeExprSpecYYN was defined.  */
static const SeExpr2Spectype_uint16 SeExpr2Specrline[] =
{
       0,   313,   313,   314,   319,   320,   324,   325,   330,   331,
     335,   336,   337,   338,   339,   340,   341,   345,   346,   347,
     348,   349,   350,   354,   359,   360,   361,   366,   367,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   392,   393,   394,   395,   396,   397,   398,   399,   400,
     425,   426,   427,   428,   433,   434,   439,   448,   460,   461
};
#endif

#if SeExprSpecYYDEBUG || SeExprSpecYYERROR_VERBOSE || SeExprSpecYYTOKEN_TABLE
/* SeExprSpecYYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at SeExprSpecYYNTOKENS, nonterminals.  */
static const char *const SeExpr2Spectname[] =
{
  "$end", "error", "$undefined", "IF", "ELSE", "NAME", "VAR", "STR",
  "NUMBER", "AddEq", "SubEq", "MultEq", "DivEq", "ExpEq", "ModEq", "'('",
  "')'", "ARROW", "':'", "'?'", "OR", "AND", "NE", "EQ", "'<'", "'>'",
  "GE", "LE", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "'~'", "UNARY",
  "'^'", "'['", "'='", "';'", "'{'", "'}'", "','", "']'", "$accept",
  "expr", "optassigns", "assigns", "assign", "ifthenelse", "optelse", "e",
  "optargs", "args", "arg", 0
};
#endif

# ifdef SeExprSpecYYPRINT
/* SeExprSpecYYTOKNUM[SeExprSpecYYLEX-NUM] -- Internal token number corresponding to
   token SeExprSpecYYLEX-NUM.  */
static const SeExpr2Spectype_uint16 SeExpr2Spectoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,    40,    41,   270,    58,    63,
     271,   272,   273,   274,    60,    62,   275,   276,    43,    45,
      42,    47,    37,    33,   126,   277,    94,    91,    61,    59,
     123,   125,    44,    93
};
# endif

/* SeExprSpecYYR1[SeExprSpecYYN] -- Symbol number of symbol that rule SeExprSpecYYN derives.  */
static const SeExpr2Spectype_uint8 SeExpr2Specr1[] =
{
       0,    44,    45,    45,    46,    46,    47,    47,    48,    48,
      48,    48,    48,    48,    48,    48,    48,    48,    48,    48,
      48,    48,    48,    49,    50,    50,    50,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    52,    52,    53,    53,    54,    54
};

/* SeExprSpecYYR2[SeExprSpecYYN] -- Number of symbols composing right hand side of rule SeExprSpecYYN.  */
static const SeExpr2Spectype_uint8 SeExpr2Specr2[] =
{
       0,     2,     2,     1,     0,     1,     1,     2,     1,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     8,     0,     4,     2,     3,     7,     4,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     4,
       6,     1,     1,     1,     0,     1,     1,     3,     1,     1
};

/* SeExprSpecYYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when SeExprSpecYYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const SeExpr2Spectype_uint8 SeExpr2Specdefact[] =
{
       0,     0,    52,    51,    53,     0,     0,     0,     0,     0,
       0,     0,     0,     6,     8,     3,     0,     0,     0,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       0,     0,    52,    51,     0,    39,    40,    41,    42,     0,
       1,     7,     2,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,    58,     0,
      55,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      27,     0,     0,     0,    31,    32,    34,    33,    35,    36,
      38,    37,    43,    44,    45,    46,    47,    48,     0,     0,
      17,    18,    19,    20,    21,    22,    49,     0,    16,    10,
      11,    12,    13,    14,    15,     9,     0,    54,     0,    29,
       4,    57,     0,     0,    30,     0,     0,     0,     5,     0,
      50,    24,    28,     0,    23,     4,    26,     0,    25
};

/* SeExprSpecYYDEFGOTO[NTERM-NUM].  */
static const SeExpr2Spectype_int16 SeExpr2Specdefgoto[] =
{
      -1,    11,   127,   128,    13,    14,   134,    68,    69,    70,
      71
};

/* SeExprSpecYYPACT[STATE-NUM] -- Index in SeExprSpecYYTABLE of the portion describing
   STATE-NUM.  */
#define SeExprSpecYYPACT_NINF -65
static const SeExpr2Spectype_int16 SeExpr2Specpact[] =
{
      57,    25,    23,   127,   -65,    98,    98,    98,    98,    98,
      98,    15,    57,   -65,   -65,   590,    98,    98,    98,    98,
      98,    98,    98,    68,    98,    98,    98,    98,    98,    98,
      98,    98,    26,   -65,   526,   -33,   -33,   -33,   -33,   181,
     -65,   -65,   590,    18,    98,    98,    98,    98,    98,    98,
      98,    98,    98,    98,    98,    98,    98,    98,    98,    98,
     548,   229,   250,   271,   292,   313,   334,   -65,   590,    27,
      22,   -65,   355,   376,   397,   418,   439,   460,   481,   502,
     -65,    98,    51,   569,   626,   642,   656,   656,    79,    79,
      79,    79,    93,    93,   -33,   -33,   -33,   -33,   131,     2,
     -65,   -65,   -65,   -65,   -65,   -65,   -65,    68,   -65,   -65,
     -65,   -65,   -65,   -65,   -65,   -65,   205,    68,    98,   -65,
       8,   -65,    98,    52,   609,   133,   127,    28,     8,   156,
     -65,    63,   -65,    -1,   -65,     8,   -65,    29,   -65
};

/* SeExprSpecYYPGOTO[NTERM-NUM].  */
static const SeExpr2Spectype_int8 SeExpr2Specpgoto[] =
{
     -65,   -65,   -64,    77,   -11,   -55,   -65,     0,   -38,   -65,
     -27
};

/* SeExprSpecYYTABLE[SeExprSpecYYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what SeExprSpecYYDEFACT says.
   If SeExprSpecYYTABLE_NINF, syntax error.  */
#define SeExprSpecYYTABLE_NINF -1
static const SeExpr2Spectype_uint8 SeExpr2Spectable[] =
{
      15,    41,     1,    58,    59,    34,    35,    36,    37,    38,
      39,     1,    42,   125,   126,    40,    60,    61,    62,    63,
      64,    65,    66,    82,    72,    73,    74,    75,    76,    77,
      78,    79,    17,    18,    19,    20,    21,    22,    23,   135,
      16,    23,   120,   106,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
       1,    24,     2,     3,   107,     4,   117,   133,   130,   131,
     138,   137,     5,    32,    33,    67,     4,    12,   136,   123,
     121,   116,     0,     5,     0,     6,     7,     0,     0,     0,
       8,     9,     0,     0,    10,     0,     6,     7,     0,     0,
       0,     8,     9,    32,    33,    10,     4,    53,    54,    55,
      56,    57,     0,     5,     0,    58,    59,    41,   124,     0,
       0,     0,   129,    55,    56,    57,     6,     7,     0,    58,
      59,     8,     9,     0,     0,    10,    25,    26,    27,    28,
      29,    30,    17,    18,    19,    20,    21,    22,    43,     0,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,     0,    31,     0,    58,    59,     0,
       0,    24,     0,    43,   119,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,     0,
       0,     0,    58,    59,     0,     0,     0,     0,    43,   132,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,     0,     0,     0,    58,    59,     0,
       0,     0,    43,    81,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,     0,     0,
       0,    58,    59,     0,     0,     0,    43,   122,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,     0,     0,     0,    58,    59,    43,   100,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,     0,     0,     0,    58,    59,    43,   101,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,     0,     0,     0,    58,    59,    43,
     102,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,     0,     0,     0,    58,    59,
      43,   103,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,     0,     0,     0,    58,
      59,    43,   104,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,     0,     0,     0,
      58,    59,    43,   105,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,     0,     0,
       0,    58,    59,    43,   108,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,     0,
       0,     0,    58,    59,    43,   109,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
       0,     0,     0,    58,    59,    43,   110,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,     0,     0,     0,    58,    59,    43,   111,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,     0,     0,     0,    58,    59,    43,   112,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,     0,     0,     0,    58,    59,    43,   113,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,     0,     0,     0,    58,    59,    43,
     114,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,     0,     0,     0,    58,    59,
       0,   115,    80,    43,     0,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,     0,
       0,     0,    58,    59,    99,    43,     0,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,     0,     0,     0,    58,    59,    43,   118,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,     0,     0,     0,    58,    59,    43,     0,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,     0,     0,     0,    58,    59,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,     0,     0,     0,    58,    59,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,     0,
       0,     0,    58,    59,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,     0,     0,     0,    58,    59,
      49,    50,    51,    52,    53,    54,    55,    56,    57,     0,
       0,     0,    58,    59
};

static const SeExpr2Spectype_int16 SeExpr2Speccheck[] =
{
       0,    12,     3,    36,    37,     5,     6,     7,     8,     9,
      10,     3,    12,     5,     6,     0,    16,    17,    18,    19,
      20,    21,    22,     5,    24,    25,    26,    27,    28,    29,
      30,    31,     9,    10,    11,    12,    13,    14,    15,    40,
      15,    15,    40,    16,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
       3,    38,     5,     6,    42,     8,    15,     4,    16,    41,
      41,   135,    15,     5,     6,     7,     8,     0,   133,   117,
     107,    81,    -1,    15,    -1,    28,    29,    -1,    -1,    -1,
      33,    34,    -1,    -1,    37,    -1,    28,    29,    -1,    -1,
      -1,    33,    34,     5,     6,    37,     8,    28,    29,    30,
      31,    32,    -1,    15,    -1,    36,    37,   128,   118,    -1,
      -1,    -1,   122,    30,    31,    32,    28,    29,    -1,    36,
      37,    33,    34,    -1,    -1,    37,     9,    10,    11,    12,
      13,    14,     9,    10,    11,    12,    13,    14,    17,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    38,    -1,    36,    37,    -1,
      -1,    38,    -1,    17,    43,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    17,    43,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,    36,    37,    -1,
      -1,    -1,    17,    42,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    36,    37,    -1,    -1,    -1,    17,    42,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    36,    37,    17,    39,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    36,    37,    17,    39,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,    36,    37,    17,
      39,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    36,    37,
      17,    39,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    36,
      37,    17,    39,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    -1,    -1,
      36,    37,    17,    39,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    36,    37,    17,    39,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    36,    37,    17,    39,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    36,    37,    17,    39,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    -1,    36,    37,    17,    39,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    36,    37,    17,    39,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    36,    37,    17,    39,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,    36,    37,    17,
      39,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    36,    37,
      -1,    39,    16,    17,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    36,    37,    16,    17,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    -1,    36,    37,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    36,    37,    17,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    36,    37,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    36,    37,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    36,    37,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    36,    37,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    36,    37
};

/* SeExprSpecYYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const SeExpr2Spectype_uint8 SeExpr2Specstos[] =
{
       0,     3,     5,     6,     8,    15,    28,    29,    33,    34,
      37,    45,    47,    48,    49,    51,    15,     9,    10,    11,
      12,    13,    14,    15,    38,     9,    10,    11,    12,    13,
      14,    38,     5,     6,    51,    51,    51,    51,    51,    51,
       0,    48,    51,    17,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    36,    37,
      51,    51,    51,    51,    51,    51,    51,     7,    51,    52,
      53,    54,    51,    51,    51,    51,    51,    51,    51,    51,
      16,    42,     5,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    16,
      39,    39,    39,    39,    39,    39,    16,    42,    39,    39,
      39,    39,    39,    39,    39,    39,    51,    15,    18,    43,
      40,    54,    42,    52,    51,     5,     6,    46,    47,    51,
      16,    41,    43,     4,    50,    40,    49,    46,    41
};

#define SeExpr2Specerrok		(SeExpr2Specerrstatus = 0)
#define SeExpr2Specclearin	(SeExpr2Specchar = SeExprSpecYYEMPTY)
#define SeExprSpecYYEMPTY		(-2)
#define SeExprSpecYYEOF		0

#define SeExprSpecYYACCEPT	goto SeExpr2Specacceptlab
#define SeExprSpecYYABORT		goto SeExpr2Specabortlab
#define SeExprSpecYYERROR		goto SeExpr2Specerrorlab


/* Like SeExprSpecYYERROR except do call SeExpr2Specerror.  This remains here temporarily
   to ease the transition to the new meaning of SeExprSpecYYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define SeExprSpecYYFAIL		goto SeExpr2Specerrlab

#define SeExprSpecYYRECOVERING()  (!!SeExpr2Specerrstatus)

#define SeExprSpecYYBACKUP(Token, Value)					\
do								\
  if (SeExpr2Specchar == SeExprSpecYYEMPTY && SeExpr2Speclen == 1)				\
    {								\
      SeExpr2Specchar = (Token);						\
      SeExpr2Speclval = (Value);						\
      SeExpr2Spectoken = SeExprSpecYYTRANSLATE (SeExpr2Specchar);				\
      SeExprSpecYYPOPSTACK (1);						\
      goto SeExpr2Specbackup;						\
    }								\
  else								\
    {								\
      SeExpr2Specerror (SeExprSpecYY_("syntax error: cannot back up")); \
      SeExprSpecYYERROR;							\
    }								\
while (SeExprSpecYYID (0))


#define SeExprSpecYYTERROR	1
#define SeExprSpecYYERRCODE	256


/* SeExprSpecYYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define SeExprSpecYYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef SeExprSpecYYLLOC_DEFAULT
# define SeExprSpecYYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (SeExprSpecYYID (N))                                                    \
	{								\
	  (Current).first_line   = SeExprSpecYYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = SeExprSpecYYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = SeExprSpecYYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = SeExprSpecYYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    SeExprSpecYYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    SeExprSpecYYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (SeExprSpecYYID (0))
#endif


/* SeExprSpecYY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef SeExprSpecYY_LOCATION_PRINT
# if defined SeExprSpecYYLTYPE_IS_TRIVIAL && SeExprSpecYYLTYPE_IS_TRIVIAL
#  define SeExprSpecYY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define SeExprSpecYY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* SeExprSpecYYLEX -- calling `SeExpr2Speclex' with the right arguments.  */

#ifdef SeExprSpecYYLEX_PARAM
# define SeExprSpecYYLEX SeExpr2Speclex (SeExprSpecYYLEX_PARAM)
#else
# define SeExprSpecYYLEX SeExpr2Speclex ()
#endif

/* Enable debugging if requested.  */
#if SeExprSpecYYDEBUG

# ifndef SeExprSpecYYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define SeExprSpecYYFPRINTF fprintf
# endif

# define SeExprSpecYYDPRINTF(Args)			\
do {						\
  if (SeExpr2Specdebug)					\
    SeExprSpecYYFPRINTF Args;				\
} while (SeExprSpecYYID (0))

# define SeExprSpecYY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (SeExpr2Specdebug)								  \
    {									  \
      SeExprSpecYYFPRINTF (stderr, "%s ", Title);					  \
      SeExpr2Spec_symbol_print (stderr,						  \
		  Type, Value, Location); \
      SeExprSpecYYFPRINTF (stderr, "\n");						  \
    }									  \
} while (SeExprSpecYYID (0))


/*--------------------------------.
| Print this symbol on SeExprSpecYYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
SeExpr2Spec_symbol_value_print (FILE *SeExpr2Specoutput, int SeExpr2Spectype, SeExprSpecYYSTYPE const * const SeExpr2Specvaluep, SeExprSpecYYLTYPE const * const SeExpr2Speclocationp)
#else
static void
SeExpr2Spec_symbol_value_print (SeExpr2Specoutput, SeExpr2Spectype, SeExpr2Specvaluep, SeExpr2Speclocationp)
    FILE *SeExpr2Specoutput;
    int SeExpr2Spectype;
    SeExprSpecYYSTYPE const * const SeExpr2Specvaluep;
    SeExprSpecYYLTYPE const * const SeExpr2Speclocationp;
#endif
{
  if (!SeExpr2Specvaluep)
    return;
  SeExprSpecYYUSE (SeExpr2Speclocationp);
# ifdef SeExprSpecYYPRINT
  if (SeExpr2Spectype < SeExprSpecYYNTOKENS)
    SeExprSpecYYPRINT (SeExpr2Specoutput, SeExpr2Spectoknum[SeExpr2Spectype], *SeExpr2Specvaluep);
# else
  SeExprSpecYYUSE (SeExpr2Specoutput);
# endif
  switch (SeExpr2Spectype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on SeExprSpecYYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
SeExpr2Spec_symbol_print (FILE *SeExpr2Specoutput, int SeExpr2Spectype, SeExprSpecYYSTYPE const * const SeExpr2Specvaluep, SeExprSpecYYLTYPE const * const SeExpr2Speclocationp)
#else
static void
SeExpr2Spec_symbol_print (SeExpr2Specoutput, SeExpr2Spectype, SeExpr2Specvaluep, SeExpr2Speclocationp)
    FILE *SeExpr2Specoutput;
    int SeExpr2Spectype;
    SeExprSpecYYSTYPE const * const SeExpr2Specvaluep;
    SeExprSpecYYLTYPE const * const SeExpr2Speclocationp;
#endif
{
  if (SeExpr2Spectype < SeExprSpecYYNTOKENS)
    SeExprSpecYYFPRINTF (SeExpr2Specoutput, "token %s (", SeExpr2Spectname[SeExpr2Spectype]);
  else
    SeExprSpecYYFPRINTF (SeExpr2Specoutput, "nterm %s (", SeExpr2Spectname[SeExpr2Spectype]);

  SeExprSpecYY_LOCATION_PRINT (SeExpr2Specoutput, *SeExpr2Speclocationp);
  SeExprSpecYYFPRINTF (SeExpr2Specoutput, ": ");
  SeExpr2Spec_symbol_value_print (SeExpr2Specoutput, SeExpr2Spectype, SeExpr2Specvaluep, SeExpr2Speclocationp);
  SeExprSpecYYFPRINTF (SeExpr2Specoutput, ")");
}

/*------------------------------------------------------------------.
| SeExpr2Spec_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
SeExpr2Spec_stack_print (SeExpr2Spectype_int16 *bottom, SeExpr2Spectype_int16 *top)
#else
static void
SeExpr2Spec_stack_print (bottom, top)
    SeExpr2Spectype_int16 *bottom;
    SeExpr2Spectype_int16 *top;
#endif
{
  SeExprSpecYYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    SeExprSpecYYFPRINTF (stderr, " %d", *bottom);
  SeExprSpecYYFPRINTF (stderr, "\n");
}

# define SeExprSpecYY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (SeExpr2Specdebug)							\
    SeExpr2Spec_stack_print ((Bottom), (Top));				\
} while (SeExprSpecYYID (0))


/*------------------------------------------------.
| Report that the SeExprSpecYYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
SeExpr2Spec_reduce_print (SeExprSpecYYSTYPE *SeExpr2Specvsp, SeExprSpecYYLTYPE *SeExpr2Speclsp, int SeExpr2Specrule)
#else
static void
SeExpr2Spec_reduce_print (SeExpr2Specvsp, SeExpr2Speclsp, SeExpr2Specrule)
    SeExprSpecYYSTYPE *SeExpr2Specvsp;
    SeExprSpecYYLTYPE *SeExpr2Speclsp;
    int SeExpr2Specrule;
#endif
{
  int SeExpr2Specnrhs = SeExpr2Specr2[SeExpr2Specrule];
  int SeExpr2Speci;
  unsigned long int SeExpr2Speclno = SeExpr2Specrline[SeExpr2Specrule];
  SeExprSpecYYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     SeExpr2Specrule - 1, SeExpr2Speclno);
  /* The symbols being reduced.  */
  for (SeExpr2Speci = 0; SeExpr2Speci < SeExpr2Specnrhs; SeExpr2Speci++)
    {
      fprintf (stderr, "   $%d = ", SeExpr2Speci + 1);
      SeExpr2Spec_symbol_print (stderr, SeExpr2Specrhs[SeExpr2Specprhs[SeExpr2Specrule] + SeExpr2Speci],
		       &(SeExpr2Specvsp[(SeExpr2Speci + 1) - (SeExpr2Specnrhs)])
		       , &(SeExpr2Speclsp[(SeExpr2Speci + 1) - (SeExpr2Specnrhs)])		       );
      fprintf (stderr, "\n");
    }
}

# define SeExprSpecYY_REDUCE_PRINT(Rule)		\
do {					\
  if (SeExpr2Specdebug)				\
    SeExpr2Spec_reduce_print (SeExpr2Specvsp, SeExpr2Speclsp, Rule); \
} while (SeExprSpecYYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int SeExpr2Specdebug;
#else /* !SeExprSpecYYDEBUG */
# define SeExprSpecYYDPRINTF(Args)
# define SeExprSpecYY_SYMBOL_PRINT(Title, Type, Value, Location)
# define SeExprSpecYY_STACK_PRINT(Bottom, Top)
# define SeExprSpecYY_REDUCE_PRINT(Rule)
#endif /* !SeExprSpecYYDEBUG */


/* SeExprSpecYYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	SeExprSpecYYINITDEPTH
# define SeExprSpecYYINITDEPTH 200
#endif

/* SeExprSpecYYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SeExprSpecYYSTACK_ALLOC_MAXIMUM < SeExprSpecYYSTACK_BYTES (SeExprSpecYYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef SeExprSpecYYMAXDEPTH
# define SeExprSpecYYMAXDEPTH 10000
#endif



#if SeExprSpecYYERROR_VERBOSE

# ifndef SeExpr2Specstrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define SeExpr2Specstrlen strlen
#  else
/* Return the length of SeExprSpecYYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static SeExprSpecYYSIZE_T
SeExpr2Specstrlen (const char *SeExpr2Specstr)
#else
static SeExprSpecYYSIZE_T
SeExpr2Specstrlen (SeExpr2Specstr)
    const char *SeExpr2Specstr;
#endif
{
  SeExprSpecYYSIZE_T SeExpr2Speclen;
  for (SeExpr2Speclen = 0; SeExpr2Specstr[SeExpr2Speclen]; SeExpr2Speclen++)
    continue;
  return SeExpr2Speclen;
}
#  endif
# endif

# ifndef SeExpr2Specstpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define SeExpr2Specstpcpy stpcpy
#  else
/* Copy SeExprSpecYYSRC to SeExprSpecYYDEST, returning the address of the terminating '\0' in
   SeExprSpecYYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
SeExpr2Specstpcpy (char *SeExpr2Specdest, const char *SeExpr2Specsrc)
#else
static char *
SeExpr2Specstpcpy (SeExpr2Specdest, SeExpr2Specsrc)
    char *SeExpr2Specdest;
    const char *SeExpr2Specsrc;
#endif
{
  char *SeExpr2Specd = SeExpr2Specdest;
  const char *SeExpr2Specs = SeExpr2Specsrc;

  while ((*SeExpr2Specd++ = *SeExpr2Specs++) != '\0')
    continue;

  return SeExpr2Specd - 1;
}
#  endif
# endif

# ifndef SeExpr2Spectnamerr
/* Copy to SeExprSpecYYRES the contents of SeExprSpecYYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for SeExpr2Specerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  SeExprSpecYYSTR is taken from SeExpr2Spectname.  If SeExprSpecYYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static SeExprSpecYYSIZE_T
SeExpr2Spectnamerr (char *SeExpr2Specres, const char *SeExpr2Specstr)
{
  if (*SeExpr2Specstr == '"')
    {
      SeExprSpecYYSIZE_T SeExpr2Specn = 0;
      char const *SeExpr2Specp = SeExpr2Specstr;

      for (;;)
	switch (*++SeExpr2Specp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++SeExpr2Specp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (SeExpr2Specres)
	      SeExpr2Specres[SeExpr2Specn] = *SeExpr2Specp;
	    SeExpr2Specn++;
	    break;

	  case '"':
	    if (SeExpr2Specres)
	      SeExpr2Specres[SeExpr2Specn] = '\0';
	    return SeExpr2Specn;
	  }
    do_not_strip_quotes: ;
    }

  if (! SeExpr2Specres)
    return SeExpr2Specstrlen (SeExpr2Specstr);

  return SeExpr2Specstpcpy (SeExpr2Specres, SeExpr2Specstr) - SeExpr2Specres;
}
# endif

/* Copy into SeExprSpecYYRESULT an error message about the unexpected token
   SeExprSpecYYCHAR while in state SeExprSpecYYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If SeExprSpecYYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return SeExprSpecYYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static SeExprSpecYYSIZE_T
SeExpr2Specsyntax_error (char *SeExpr2Specresult, int SeExpr2Specstate, int SeExpr2Specchar)
{
  int SeExpr2Specn = SeExpr2Specpact[SeExpr2Specstate];

  if (! (SeExprSpecYYPACT_NINF < SeExpr2Specn && SeExpr2Specn <= SeExprSpecYYLAST))
    return 0;
  else
    {
      int SeExpr2Spectype = SeExprSpecYYTRANSLATE (SeExpr2Specchar);
      SeExprSpecYYSIZE_T SeExpr2Specsize0 = SeExpr2Spectnamerr (0, SeExpr2Spectname[SeExpr2Spectype]);
      SeExprSpecYYSIZE_T SeExpr2Specsize = SeExpr2Specsize0;
      SeExprSpecYYSIZE_T SeExpr2Specsize1;
      int SeExpr2Specsize_overflow = 0;
      enum { SeExprSpecYYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *SeExpr2Specarg[SeExprSpecYYERROR_VERBOSE_ARGS_MAXIMUM];
      int SeExpr2Specx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      SeExprSpecYY_("syntax error, unexpected %s");
      SeExprSpecYY_("syntax error, unexpected %s, expecting %s");
      SeExprSpecYY_("syntax error, unexpected %s, expecting %s or %s");
      SeExprSpecYY_("syntax error, unexpected %s, expecting %s or %s or %s");
      SeExprSpecYY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *SeExpr2Specfmt;
      char const *SeExpr2Specf;
      static char const SeExpr2Specunexpected[] = "syntax error, unexpected %s";
      static char const SeExpr2Specexpecting[] = ", expecting %s";
      static char const SeExpr2Specor[] = " or %s";
      char SeExpr2Specformat[sizeof SeExpr2Specunexpected
		    + sizeof SeExpr2Specexpecting - 1
		    + ((SeExprSpecYYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof SeExpr2Specor - 1))];
      char const *SeExpr2Specprefix = SeExpr2Specexpecting;

      /* Start SeExprSpecYYX at -SeExprSpecYYN if negative to avoid negative indexes in
	 SeExprSpecYYCHECK.  */
      int SeExpr2Specxbegin = SeExpr2Specn < 0 ? -SeExpr2Specn : 0;

      /* Stay within bounds of both SeExpr2Speccheck and SeExpr2Spectname.  */
      int SeExpr2Specchecklim = SeExprSpecYYLAST - SeExpr2Specn + 1;
      int SeExpr2Specxend = SeExpr2Specchecklim < SeExprSpecYYNTOKENS ? SeExpr2Specchecklim : SeExprSpecYYNTOKENS;
      int SeExpr2Speccount = 1;

      SeExpr2Specarg[0] = SeExpr2Spectname[SeExpr2Spectype];
      SeExpr2Specfmt = SeExpr2Specstpcpy (SeExpr2Specformat, SeExpr2Specunexpected);

      for (SeExpr2Specx = SeExpr2Specxbegin; SeExpr2Specx < SeExpr2Specxend; ++SeExpr2Specx)
	if (SeExpr2Speccheck[SeExpr2Specx + SeExpr2Specn] == SeExpr2Specx && SeExpr2Specx != SeExprSpecYYTERROR)
	  {
	    if (SeExpr2Speccount == SeExprSpecYYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		SeExpr2Speccount = 1;
		SeExpr2Specsize = SeExpr2Specsize0;
		SeExpr2Specformat[sizeof SeExpr2Specunexpected - 1] = '\0';
		break;
	      }
	    SeExpr2Specarg[SeExpr2Speccount++] = SeExpr2Spectname[SeExpr2Specx];
	    SeExpr2Specsize1 = SeExpr2Specsize + SeExpr2Spectnamerr (0, SeExpr2Spectname[SeExpr2Specx]);
	    SeExpr2Specsize_overflow |= (SeExpr2Specsize1 < SeExpr2Specsize);
	    SeExpr2Specsize = SeExpr2Specsize1;
	    SeExpr2Specfmt = SeExpr2Specstpcpy (SeExpr2Specfmt, SeExpr2Specprefix);
	    SeExpr2Specprefix = SeExpr2Specor;
	  }

      SeExpr2Specf = SeExprSpecYY_(SeExpr2Specformat);
      SeExpr2Specsize1 = SeExpr2Specsize + SeExpr2Specstrlen (SeExpr2Specf);
      SeExpr2Specsize_overflow |= (SeExpr2Specsize1 < SeExpr2Specsize);
      SeExpr2Specsize = SeExpr2Specsize1;

      if (SeExpr2Specsize_overflow)
	return SeExprSpecYYSIZE_MAXIMUM;

      if (SeExpr2Specresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *SeExpr2Specp = SeExpr2Specresult;
	  int SeExpr2Speci = 0;
	  while ((*SeExpr2Specp = *SeExpr2Specf) != '\0')
	    {
	      if (*SeExpr2Specp == '%' && SeExpr2Specf[1] == 's' && SeExpr2Speci < SeExpr2Speccount)
		{
		  SeExpr2Specp += SeExpr2Spectnamerr (SeExpr2Specp, SeExpr2Specarg[SeExpr2Speci++]);
		  SeExpr2Specf += 2;
		}
	      else
		{
		  SeExpr2Specp++;
		  SeExpr2Specf++;
		}
	    }
	}
      return SeExpr2Specsize;
    }
}
#endif /* SeExprSpecYYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
SeExpr2Specdestruct (const char *SeExpr2Specmsg, int SeExpr2Spectype, SeExprSpecYYSTYPE *SeExpr2Specvaluep, SeExprSpecYYLTYPE *SeExpr2Speclocationp)
#else
static void
SeExpr2Specdestruct (SeExpr2Specmsg, SeExpr2Spectype, SeExpr2Specvaluep, SeExpr2Speclocationp)
    const char *SeExpr2Specmsg;
    int SeExpr2Spectype;
    SeExprSpecYYSTYPE *SeExpr2Specvaluep;
    SeExprSpecYYLTYPE *SeExpr2Speclocationp;
#endif
{
  SeExprSpecYYUSE (SeExpr2Specvaluep);
  SeExprSpecYYUSE (SeExpr2Speclocationp);

  if (!SeExpr2Specmsg)
    SeExpr2Specmsg = "Deleting";
  SeExprSpecYY_SYMBOL_PRINT (SeExpr2Specmsg, SeExpr2Spectype, SeExpr2Specvaluep, SeExpr2Speclocationp);

  switch (SeExpr2Spectype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef SeExprSpecYYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int SeExpr2Specparse (void *SeExprSpecYYPARSE_PARAM);
#else
int SeExpr2Specparse ();
#endif
#else /* ! SeExprSpecYYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int SeExpr2Specparse (void);
#else
int SeExpr2Specparse ();
#endif
#endif /* ! SeExprSpecYYPARSE_PARAM */



/* The look-ahead symbol.  */
int SeExpr2Specchar;

/* The semantic value of the look-ahead symbol.  */
SeExprSpecYYSTYPE SeExpr2Speclval;

/* Number of syntax errors so far.  */
int SeExpr2Specnerrs;
/* Location data for the look-ahead symbol.  */
SeExprSpecYYLTYPE SeExpr2Speclloc;



/*----------.
| SeExpr2Specparse.  |
`----------*/

#ifdef SeExprSpecYYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
SeExpr2Specparse (void *SeExprSpecYYPARSE_PARAM)
#else
int
SeExpr2Specparse (SeExprSpecYYPARSE_PARAM)
    void *SeExprSpecYYPARSE_PARAM;
#endif
#else /* ! SeExprSpecYYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
SeExpr2Specparse (void)
#else
int
SeExpr2Specparse ()

#endif
#endif
{
  
  int SeExpr2Specstate;
  int SeExpr2Specn;
  int SeExpr2Specresult;
  /* Number of tokens to shift before error messages enabled.  */
  int SeExpr2Specerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int SeExpr2Spectoken = 0;
#if SeExprSpecYYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char SeExpr2Specmsgbuf[128];
  char *SeExpr2Specmsg = SeExpr2Specmsgbuf;
  SeExprSpecYYSIZE_T SeExpr2Specmsg_alloc = sizeof SeExpr2Specmsgbuf;
#endif

  /* Three stacks and their tools:
     `SeExpr2Specss': related to states,
     `SeExpr2Specvs': related to semantic values,
     `SeExpr2Specls': related to locations.

     Refer to the stacks thru separate pointers, to allow SeExpr2Specoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  SeExpr2Spectype_int16 SeExpr2Specssa[SeExprSpecYYINITDEPTH];
  SeExpr2Spectype_int16 *SeExpr2Specss = SeExpr2Specssa;
  SeExpr2Spectype_int16 *SeExpr2Specssp;

  /* The semantic value stack.  */
  SeExprSpecYYSTYPE SeExpr2Specvsa[SeExprSpecYYINITDEPTH];
  SeExprSpecYYSTYPE *SeExpr2Specvs = SeExpr2Specvsa;
  SeExprSpecYYSTYPE *SeExpr2Specvsp;

  /* The location stack.  */
  SeExprSpecYYLTYPE SeExpr2Speclsa[SeExprSpecYYINITDEPTH];
  SeExprSpecYYLTYPE *SeExpr2Specls = SeExpr2Speclsa;
  SeExprSpecYYLTYPE *SeExpr2Speclsp;
  /* The locations where the error started and ended.  */
  SeExprSpecYYLTYPE SeExpr2Specerror_range[2];

#define SeExprSpecYYPOPSTACK(N)   (SeExpr2Specvsp -= (N), SeExpr2Specssp -= (N), SeExpr2Speclsp -= (N))

  SeExprSpecYYSIZE_T SeExpr2Specstacksize = SeExprSpecYYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  SeExprSpecYYSTYPE SeExpr2Specval;
  SeExprSpecYYLTYPE SeExpr2Specloc;

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int SeExpr2Speclen = 0;

  SeExprSpecYYDPRINTF ((stderr, "Starting parse\n"));

  SeExpr2Specstate = 0;
  SeExpr2Specerrstatus = 0;
  SeExpr2Specnerrs = 0;
  SeExpr2Specchar = SeExprSpecYYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  SeExpr2Specssp = SeExpr2Specss;
  SeExpr2Specvsp = SeExpr2Specvs;
  SeExpr2Speclsp = SeExpr2Specls;
#if defined SeExprSpecYYLTYPE_IS_TRIVIAL && SeExprSpecYYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  SeExpr2Speclloc.first_line   = SeExpr2Speclloc.last_line   = 1;
  SeExpr2Speclloc.first_column = SeExpr2Speclloc.last_column = 0;
#endif

  goto SeExpr2Specsetstate;

/*------------------------------------------------------------.
| SeExpr2Specnewstate -- Push a new state, which is found in SeExpr2Specstate.  |
`------------------------------------------------------------*/
 SeExpr2Specnewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  SeExpr2Specssp++;

 SeExpr2Specsetstate:
  *SeExpr2Specssp = SeExpr2Specstate;

  if (SeExpr2Specss + SeExpr2Specstacksize - 1 <= SeExpr2Specssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      SeExprSpecYYSIZE_T SeExpr2Specsize = SeExpr2Specssp - SeExpr2Specss + 1;

#ifdef SeExpr2Specoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	SeExprSpecYYSTYPE *SeExpr2Specvs1 = SeExpr2Specvs;
	SeExpr2Spectype_int16 *SeExpr2Specss1 = SeExpr2Specss;
	SeExprSpecYYLTYPE *SeExpr2Specls1 = SeExpr2Specls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if SeExpr2Specoverflow is a macro.  */
	SeExpr2Specoverflow (SeExprSpecYY_("memory exhausted"),
		    &SeExpr2Specss1, SeExpr2Specsize * sizeof (*SeExpr2Specssp),
		    &SeExpr2Specvs1, SeExpr2Specsize * sizeof (*SeExpr2Specvsp),
		    &SeExpr2Specls1, SeExpr2Specsize * sizeof (*SeExpr2Speclsp),
		    &SeExpr2Specstacksize);
	SeExpr2Specls = SeExpr2Specls1;
	SeExpr2Specss = SeExpr2Specss1;
	SeExpr2Specvs = SeExpr2Specvs1;
      }
#else /* no SeExpr2Specoverflow */
# ifndef SeExprSpecYYSTACK_RELOCATE
      goto SeExpr2Specexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (SeExprSpecYYMAXDEPTH <= SeExpr2Specstacksize)
	goto SeExpr2Specexhaustedlab;
      SeExpr2Specstacksize *= 2;
      if (SeExprSpecYYMAXDEPTH < SeExpr2Specstacksize)
	SeExpr2Specstacksize = SeExprSpecYYMAXDEPTH;

      {
	SeExpr2Spectype_int16 *SeExpr2Specss1 = SeExpr2Specss;
	union SeExpr2Specalloc *SeExpr2Specptr =
	  (union SeExpr2Specalloc *) SeExprSpecYYSTACK_ALLOC (SeExprSpecYYSTACK_BYTES (SeExpr2Specstacksize));
	if (! SeExpr2Specptr)
	  goto SeExpr2Specexhaustedlab;
	SeExprSpecYYSTACK_RELOCATE (SeExpr2Specss);
	SeExprSpecYYSTACK_RELOCATE (SeExpr2Specvs);
	SeExprSpecYYSTACK_RELOCATE (SeExpr2Specls);
#  undef SeExprSpecYYSTACK_RELOCATE
	if (SeExpr2Specss1 != SeExpr2Specssa)
	  SeExprSpecYYSTACK_FREE (SeExpr2Specss1);
      }
# endif
#endif /* no SeExpr2Specoverflow */

      SeExpr2Specssp = SeExpr2Specss + SeExpr2Specsize - 1;
      SeExpr2Specvsp = SeExpr2Specvs + SeExpr2Specsize - 1;
      SeExpr2Speclsp = SeExpr2Specls + SeExpr2Specsize - 1;

      SeExprSpecYYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) SeExpr2Specstacksize));

      if (SeExpr2Specss + SeExpr2Specstacksize - 1 <= SeExpr2Specssp)
	SeExprSpecYYABORT;
    }

  SeExprSpecYYDPRINTF ((stderr, "Entering state %d\n", SeExpr2Specstate));

  goto SeExpr2Specbackup;

/*-----------.
| SeExpr2Specbackup.  |
`-----------*/
SeExpr2Specbackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  SeExpr2Specn = SeExpr2Specpact[SeExpr2Specstate];
  if (SeExpr2Specn == SeExprSpecYYPACT_NINF)
    goto SeExpr2Specdefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* SeExprSpecYYCHAR is either SeExprSpecYYEMPTY or SeExprSpecYYEOF or a valid look-ahead symbol.  */
  if (SeExpr2Specchar == SeExprSpecYYEMPTY)
    {
      SeExprSpecYYDPRINTF ((stderr, "Reading a token: "));
      SeExpr2Specchar = SeExprSpecYYLEX;
    }

  if (SeExpr2Specchar <= SeExprSpecYYEOF)
    {
      SeExpr2Specchar = SeExpr2Spectoken = SeExprSpecYYEOF;
      SeExprSpecYYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      SeExpr2Spectoken = SeExprSpecYYTRANSLATE (SeExpr2Specchar);
      SeExprSpecYY_SYMBOL_PRINT ("Next token is", SeExpr2Spectoken, &SeExpr2Speclval, &SeExpr2Speclloc);
    }

  /* If the proper action on seeing token SeExprSpecYYTOKEN is to reduce or to
     detect an error, take that action.  */
  SeExpr2Specn += SeExpr2Spectoken;
  if (SeExpr2Specn < 0 || SeExprSpecYYLAST < SeExpr2Specn || SeExpr2Speccheck[SeExpr2Specn] != SeExpr2Spectoken)
    goto SeExpr2Specdefault;
  SeExpr2Specn = SeExpr2Spectable[SeExpr2Specn];
  if (SeExpr2Specn <= 0)
    {
      if (SeExpr2Specn == 0 || SeExpr2Specn == SeExprSpecYYTABLE_NINF)
	goto SeExpr2Specerrlab;
      SeExpr2Specn = -SeExpr2Specn;
      goto SeExpr2Specreduce;
    }

  if (SeExpr2Specn == SeExprSpecYYFINAL)
    SeExprSpecYYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (SeExpr2Specerrstatus)
    SeExpr2Specerrstatus--;

  /* Shift the look-ahead token.  */
  SeExprSpecYY_SYMBOL_PRINT ("Shifting", SeExpr2Spectoken, &SeExpr2Speclval, &SeExpr2Speclloc);

  /* Discard the shifted token unless it is eof.  */
  if (SeExpr2Specchar != SeExprSpecYYEOF)
    SeExpr2Specchar = SeExprSpecYYEMPTY;

  SeExpr2Specstate = SeExpr2Specn;
  *++SeExpr2Specvsp = SeExpr2Speclval;
  *++SeExpr2Speclsp = SeExpr2Speclloc;
  goto SeExpr2Specnewstate;


/*-----------------------------------------------------------.
| SeExpr2Specdefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
SeExpr2Specdefault:
  SeExpr2Specn = SeExpr2Specdefact[SeExpr2Specstate];
  if (SeExpr2Specn == 0)
    goto SeExpr2Specerrlab;
  goto SeExpr2Specreduce;


/*-----------------------------.
| SeExpr2Specreduce -- Do a reduction.  |
`-----------------------------*/
SeExpr2Specreduce:
  /* SeExpr2Specn is the number of a rule to reduce with.  */
  SeExpr2Speclen = SeExpr2Specr2[SeExpr2Specn];

  /* If SeExprSpecYYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets SeExprSpecYYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to SeExprSpecYYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that SeExprSpecYYVAL may be used uninitialized.  */
  SeExpr2Specval = SeExpr2Specvsp[1-SeExpr2Speclen];

  /* Default location.  */
  SeExprSpecYYLLOC_DEFAULT (SeExpr2Specloc, (SeExpr2Speclsp - SeExpr2Speclen), SeExpr2Speclen);
  SeExprSpecYY_REDUCE_PRINT (SeExpr2Specn);
  switch (SeExpr2Specn)
    {
        case 2:
#line 313 "src/ui/ExprSpecParser.y"
    { ParseResult = 0; }
    break;

  case 3:
#line 314 "src/ui/ExprSpecParser.y"
    { ParseResult = 0; }
    break;

  case 4:
#line 319 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 5:
#line 320 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 6:
#line 324 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 7:
#line 325 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 8:
#line 330 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 9:
#line 331 "src/ui/ExprSpecParser.y"
    {
        specRegisterVariable((SeExpr2Specvsp[(1) - (4)].s));
        specRegisterEditable((SeExpr2Specvsp[(1) - (4)].s),(SeExpr2Specvsp[(3) - (4)].n));
      }
    break;

  case 10:
#line 335 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 11:
#line 336 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 12:
#line 337 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 13:
#line 338 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 14:
#line 339 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 15:
#line 340 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 16:
#line 341 "src/ui/ExprSpecParser.y"
    {
        specRegisterVariable((SeExpr2Specvsp[(1) - (4)].s));
        specRegisterEditable((SeExpr2Specvsp[(1) - (4)].s),(SeExpr2Specvsp[(3) - (4)].n));
      }
    break;

  case 17:
#line 345 "src/ui/ExprSpecParser.y"
    {  (SeExpr2Specval.n) = 0; }
    break;

  case 18:
#line 346 "src/ui/ExprSpecParser.y"
    {  (SeExpr2Specval.n) = 0; }
    break;

  case 19:
#line 347 "src/ui/ExprSpecParser.y"
    {  (SeExpr2Specval.n) = 0; }
    break;

  case 20:
#line 348 "src/ui/ExprSpecParser.y"
    {  (SeExpr2Specval.n) = 0; }
    break;

  case 21:
#line 349 "src/ui/ExprSpecParser.y"
    {  (SeExpr2Specval.n) = 0; }
    break;

  case 22:
#line 350 "src/ui/ExprSpecParser.y"
    {  (SeExpr2Specval.n) = 0; }
    break;

  case 23:
#line 355 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 24:
#line 359 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 25:
#line 360 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0;}
    break;

  case 26:
#line 361 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0;}
    break;

  case 27:
#line 366 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 28:
#line 367 "src/ui/ExprSpecParser.y"
    {
        if(SPEC_IS_NUMBER((SeExpr2Specvsp[(2) - (7)].n)) && SPEC_IS_NUMBER((SeExpr2Specvsp[(4) - (7)].n)) && SPEC_IS_NUMBER((SeExpr2Specvsp[(6) - (7)].n))){
            (SeExpr2Specval.n)=remember(new ExprSpecVectorNode((SeExpr2Specloc).first_column,(SeExpr2Specloc).last_column,(SeExpr2Specvsp[(2) - (7)].n),(SeExpr2Specvsp[(4) - (7)].n),(SeExpr2Specvsp[(6) - (7)].n)));
        }else (SeExpr2Specval.n)=0;
      }
    break;

  case 29:
#line 372 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 30:
#line 373 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 31:
#line 374 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 32:
#line 375 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 33:
#line 376 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 34:
#line 377 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 35:
#line 378 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 36:
#line 379 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 37:
#line 380 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 38:
#line 381 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 39:
#line 382 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = (SeExpr2Specvsp[(2) - (2)].n); }
    break;

  case 40:
#line 383 "src/ui/ExprSpecParser.y"
    {
        if(SPEC_IS_NUMBER((SeExpr2Specvsp[(2) - (2)].n))){
            ExprSpecScalarNode* node=(ExprSpecScalarNode*)(SeExpr2Specvsp[(2) - (2)].n);
            node->v*=-1;
            node->startPos=(SeExpr2Specloc).first_column;
            node->endPos=(SeExpr2Specloc).last_column;
            (SeExpr2Specval.n)=(SeExpr2Specvsp[(2) - (2)].n);
        }else (SeExpr2Specval.n)=0;
      }
    break;

  case 41:
#line 392 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 42:
#line 393 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 43:
#line 394 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 44:
#line 395 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 45:
#line 396 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 46:
#line 397 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 47:
#line 398 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 48:
#line 399 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0; }
    break;

  case 49:
#line 400 "src/ui/ExprSpecParser.y"
    {
        if((SeExpr2Specvsp[(3) - (4)].n) && strcmp((SeExpr2Specvsp[(1) - (4)].s),"curve")==0){
            (SeExpr2Specval.n)=remember(new ExprSpecCurveNode((SeExpr2Specvsp[(3) - (4)].n)));
        }else if((SeExpr2Specvsp[(3) - (4)].n) && strcmp((SeExpr2Specvsp[(1) - (4)].s),"ccurve")==0){
            (SeExpr2Specval.n)=remember(new ExprSpecCCurveNode((SeExpr2Specvsp[(3) - (4)].n)));
        }else if((SeExpr2Specvsp[(3) - (4)].n) && strcmp((SeExpr2Specvsp[(1) - (4)].s),"swatch")==0){
            (SeExpr2Specval.n)=remember(new ExprSpecColorSwatchNode((SeExpr2Specvsp[(3) - (4)].n)));
        }else if((SeExpr2Specvsp[(3) - (4)].n) && strcmp((SeExpr2Specvsp[(1) - (4)].s),"animCurve")==0){
            (SeExpr2Specval.n)=remember(new ExprSpecAnimCurveNode((SeExpr2Specvsp[(3) - (4)].n)));
        }else if((SeExpr2Specvsp[(3) - (4)].n) && strcmp((SeExpr2Specvsp[(1) - (4)].s),"deepWater")==0){
            (SeExpr2Specval.n)=remember(new ExprSpecDeepWaterNode((SeExpr2Specvsp[(3) - (4)].n)));
        }else if((SeExpr2Specvsp[(3) - (4)].n)){
            // function arguments not parse of curve, ccurve, or animCurve
            // check if there are any string args that need to be made into controls
            // but be sure to return 0 as this parseable
            if(ExprSpecListNode* list=dynamic_cast<ExprSpecListNode*>((SeExpr2Specvsp[(3) - (4)].n))){
                for(size_t i=0;i<list->nodes.size();i++){
                    if(ExprSpecStringNode* str=dynamic_cast<ExprSpecStringNode*>(list->nodes[i])){
                        specRegisterEditable("<UNKNOWN>",str);
                    }
                }
            }
            (SeExpr2Specval.n)=0;
        }else (SeExpr2Specval.n)=0;
      }
    break;

  case 50:
#line 425 "src/ui/ExprSpecParser.y"
    {(SeExpr2Specval.n) = 0; }
    break;

  case 51:
#line 426 "src/ui/ExprSpecParser.y"
    {  (SeExpr2Specval.n) = 0; }
    break;

  case 52:
#line 427 "src/ui/ExprSpecParser.y"
    {  (SeExpr2Specval.n) = 0; }
    break;

  case 53:
#line 428 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n)=remember(new ExprSpecScalarNode((SeExpr2Specloc).first_column,(SeExpr2Specloc).last_column,(SeExpr2Specvsp[(1) - (1)].d))); }
    break;

  case 54:
#line 433 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = 0;}
    break;

  case 55:
#line 434 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = (SeExpr2Specvsp[(1) - (1)].n);}
    break;

  case 56:
#line 439 "src/ui/ExprSpecParser.y"
    {
       // ignore first argument unless it is a string (because we parse strings in weird ways)
       ExprSpecListNode* list=new ExprSpecListNode((SeExpr2Specloc).last_column,(SeExpr2Specloc).last_column);
       if((SeExpr2Specvsp[(1) - (1)].n) && SPEC_IS_STR((SeExpr2Specvsp[(1) - (1)].n))){
           list->add((SeExpr2Specvsp[(1) - (1)].n));
       }
       remember(list);
       (SeExpr2Specval.n)=list;
   }
    break;

  case 57:
#line 448 "src/ui/ExprSpecParser.y"
    {

      if((SeExpr2Specvsp[(1) - (3)].n) && (SeExpr2Specvsp[(3) - (3)].n) && ((SPEC_IS_NUMBER((SeExpr2Specvsp[(3) - (3)].n)) || SPEC_IS_VECTOR((SeExpr2Specvsp[(3) - (3)].n)) || SPEC_IS_STR((SeExpr2Specvsp[(3) - (3)].n))))){
          (SeExpr2Specval.n)=(SeExpr2Specvsp[(1) - (3)].n);
          dynamic_cast<ExprSpecListNode*>((SeExpr2Specvsp[(1) - (3)].n))->add((SeExpr2Specvsp[(3) - (3)].n));
      }else{
          (SeExpr2Specval.n)=0;
      }
    }
    break;

  case 58:
#line 460 "src/ui/ExprSpecParser.y"
    { (SeExpr2Specval.n) = (SeExpr2Specvsp[(1) - (1)].n);}
    break;

  case 59:
#line 461 "src/ui/ExprSpecParser.y"
    {
        ExprSpecStringNode* str=new ExprSpecStringNode((SeExpr2Specloc).first_column,(SeExpr2Specloc).last_column,(SeExpr2Specvsp[(1) - (1)].s));
        //specRegisterEditable("<UNKNOWN>",str);
        // TODO: move string stuff out
        (SeExpr2Specval.n) = remember(str);
      }
    break;


/* Line 1267 of yacc.c.  */
#line 2220 "y.tab.c"
      default: break;
    }
  SeExprSpecYY_SYMBOL_PRINT ("-> $$ =", SeExpr2Specr1[SeExpr2Specn], &SeExpr2Specval, &SeExpr2Specloc);

  SeExprSpecYYPOPSTACK (SeExpr2Speclen);
  SeExpr2Speclen = 0;
  SeExprSpecYY_STACK_PRINT (SeExpr2Specss, SeExpr2Specssp);

  *++SeExpr2Specvsp = SeExpr2Specval;
  *++SeExpr2Speclsp = SeExpr2Specloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  SeExpr2Specn = SeExpr2Specr1[SeExpr2Specn];

  SeExpr2Specstate = SeExpr2Specpgoto[SeExpr2Specn - SeExprSpecYYNTOKENS] + *SeExpr2Specssp;
  if (0 <= SeExpr2Specstate && SeExpr2Specstate <= SeExprSpecYYLAST && SeExpr2Speccheck[SeExpr2Specstate] == *SeExpr2Specssp)
    SeExpr2Specstate = SeExpr2Spectable[SeExpr2Specstate];
  else
    SeExpr2Specstate = SeExpr2Specdefgoto[SeExpr2Specn - SeExprSpecYYNTOKENS];

  goto SeExpr2Specnewstate;


/*------------------------------------.
| SeExpr2Specerrlab -- here on detecting error |
`------------------------------------*/
SeExpr2Specerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!SeExpr2Specerrstatus)
    {
      ++SeExpr2Specnerrs;
#if ! SeExprSpecYYERROR_VERBOSE
      SeExpr2Specerror (SeExprSpecYY_("syntax error"));
#else
      {
	SeExprSpecYYSIZE_T SeExpr2Specsize = SeExpr2Specsyntax_error (0, SeExpr2Specstate, SeExpr2Specchar);
	if (SeExpr2Specmsg_alloc < SeExpr2Specsize && SeExpr2Specmsg_alloc < SeExprSpecYYSTACK_ALLOC_MAXIMUM)
	  {
	    SeExprSpecYYSIZE_T SeExpr2Specalloc = 2 * SeExpr2Specsize;
	    if (! (SeExpr2Specsize <= SeExpr2Specalloc && SeExpr2Specalloc <= SeExprSpecYYSTACK_ALLOC_MAXIMUM))
	      SeExpr2Specalloc = SeExprSpecYYSTACK_ALLOC_MAXIMUM;
	    if (SeExpr2Specmsg != SeExpr2Specmsgbuf)
	      SeExprSpecYYSTACK_FREE (SeExpr2Specmsg);
	    SeExpr2Specmsg = (char *) SeExprSpecYYSTACK_ALLOC (SeExpr2Specalloc);
	    if (SeExpr2Specmsg)
	      SeExpr2Specmsg_alloc = SeExpr2Specalloc;
	    else
	      {
		SeExpr2Specmsg = SeExpr2Specmsgbuf;
		SeExpr2Specmsg_alloc = sizeof SeExpr2Specmsgbuf;
	      }
	  }

	if (0 < SeExpr2Specsize && SeExpr2Specsize <= SeExpr2Specmsg_alloc)
	  {
	    (void) SeExpr2Specsyntax_error (SeExpr2Specmsg, SeExpr2Specstate, SeExpr2Specchar);
	    SeExpr2Specerror (SeExpr2Specmsg);
	  }
	else
	  {
	    SeExpr2Specerror (SeExprSpecYY_("syntax error"));
	    if (SeExpr2Specsize != 0)
	      goto SeExpr2Specexhaustedlab;
	  }
      }
#endif
    }

  SeExpr2Specerror_range[0] = SeExpr2Speclloc;

  if (SeExpr2Specerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (SeExpr2Specchar <= SeExprSpecYYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (SeExpr2Specchar == SeExprSpecYYEOF)
	    SeExprSpecYYABORT;
	}
      else
	{
	  SeExpr2Specdestruct ("Error: discarding",
		      SeExpr2Spectoken, &SeExpr2Speclval, &SeExpr2Speclloc);
	  SeExpr2Specchar = SeExprSpecYYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto SeExpr2Specerrlab1;


/*---------------------------------------------------.
| SeExpr2Specerrorlab -- error raised explicitly by SeExprSpecYYERROR.  |
`---------------------------------------------------*/
SeExpr2Specerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     SeExprSpecYYERROR and the label SeExpr2Specerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto SeExpr2Specerrorlab;

  SeExpr2Specerror_range[0] = SeExpr2Speclsp[1-SeExpr2Speclen];
  /* Do not reclaim the symbols of the rule which action triggered
     this SeExprSpecYYERROR.  */
  SeExprSpecYYPOPSTACK (SeExpr2Speclen);
  SeExpr2Speclen = 0;
  SeExprSpecYY_STACK_PRINT (SeExpr2Specss, SeExpr2Specssp);
  SeExpr2Specstate = *SeExpr2Specssp;
  goto SeExpr2Specerrlab1;


/*-------------------------------------------------------------.
| SeExpr2Specerrlab1 -- common code for both syntax error and SeExprSpecYYERROR.  |
`-------------------------------------------------------------*/
SeExpr2Specerrlab1:
  SeExpr2Specerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      SeExpr2Specn = SeExpr2Specpact[SeExpr2Specstate];
      if (SeExpr2Specn != SeExprSpecYYPACT_NINF)
	{
	  SeExpr2Specn += SeExprSpecYYTERROR;
	  if (0 <= SeExpr2Specn && SeExpr2Specn <= SeExprSpecYYLAST && SeExpr2Speccheck[SeExpr2Specn] == SeExprSpecYYTERROR)
	    {
	      SeExpr2Specn = SeExpr2Spectable[SeExpr2Specn];
	      if (0 < SeExpr2Specn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (SeExpr2Specssp == SeExpr2Specss)
	SeExprSpecYYABORT;

      SeExpr2Specerror_range[0] = *SeExpr2Speclsp;
      SeExpr2Specdestruct ("Error: popping",
		  SeExpr2Specstos[SeExpr2Specstate], SeExpr2Specvsp, SeExpr2Speclsp);
      SeExprSpecYYPOPSTACK (1);
      SeExpr2Specstate = *SeExpr2Specssp;
      SeExprSpecYY_STACK_PRINT (SeExpr2Specss, SeExpr2Specssp);
    }

  if (SeExpr2Specn == SeExprSpecYYFINAL)
    SeExprSpecYYACCEPT;

  *++SeExpr2Specvsp = SeExpr2Speclval;

  SeExpr2Specerror_range[1] = SeExpr2Speclloc;
  /* Using SeExprSpecYYLLOC is tempting, but would change the location of
     the look-ahead.  SeExprSpecYYLOC is available though.  */
  SeExprSpecYYLLOC_DEFAULT (SeExpr2Specloc, (SeExpr2Specerror_range - 1), 2);
  *++SeExpr2Speclsp = SeExpr2Specloc;

  /* Shift the error token.  */
  SeExprSpecYY_SYMBOL_PRINT ("Shifting", SeExpr2Specstos[SeExpr2Specn], SeExpr2Specvsp, SeExpr2Speclsp);

  SeExpr2Specstate = SeExpr2Specn;
  goto SeExpr2Specnewstate;


/*-------------------------------------.
| SeExpr2Specacceptlab -- SeExprSpecYYACCEPT comes here.  |
`-------------------------------------*/
SeExpr2Specacceptlab:
  SeExpr2Specresult = 0;
  goto SeExpr2Specreturn;

/*-----------------------------------.
| SeExpr2Specabortlab -- SeExprSpecYYABORT comes here.  |
`-----------------------------------*/
SeExpr2Specabortlab:
  SeExpr2Specresult = 1;
  goto SeExpr2Specreturn;

#ifndef SeExpr2Specoverflow
/*-------------------------------------------------.
| SeExpr2Specexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
SeExpr2Specexhaustedlab:
  SeExpr2Specerror (SeExprSpecYY_("memory exhausted"));
  SeExpr2Specresult = 2;
  /* Fall through.  */
#endif

SeExpr2Specreturn:
  if (SeExpr2Specchar != SeExprSpecYYEOF && SeExpr2Specchar != SeExprSpecYYEMPTY)
     SeExpr2Specdestruct ("Cleanup: discarding lookahead",
		 SeExpr2Spectoken, &SeExpr2Speclval, &SeExpr2Speclloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this SeExprSpecYYABORT or SeExprSpecYYACCEPT.  */
  SeExprSpecYYPOPSTACK (SeExpr2Speclen);
  SeExprSpecYY_STACK_PRINT (SeExpr2Specss, SeExpr2Specssp);
  while (SeExpr2Specssp != SeExpr2Specss)
    {
      SeExpr2Specdestruct ("Cleanup: popping",
		  SeExpr2Specstos[*SeExpr2Specssp], SeExpr2Specvsp, SeExpr2Speclsp);
      SeExprSpecYYPOPSTACK (1);
    }
#ifndef SeExpr2Specoverflow
  if (SeExpr2Specss != SeExpr2Specssa)
    SeExprSpecYYSTACK_FREE (SeExpr2Specss);
#endif
#if SeExprSpecYYERROR_VERBOSE
  if (SeExpr2Specmsg != SeExpr2Specmsgbuf)
    SeExprSpecYYSTACK_FREE (SeExpr2Specmsg);
#endif
  /* Make sure SeExprSpecYYID is used.  */
  return SeExprSpecYYID (SeExpr2Specresult);
}


#line 469 "src/ui/ExprSpecParser.y"


/* SeExpr2Specerror - Report an error.  This is called by the parser.
(Note: the "msg" param is useless as it is usually just "sparse error".
so it's ignored.)
*/
static void SeExpr2Specerror(const char* /*msg*/)
{
    // find start of line containing error
    int pos = SeExpr2Specpos(), lineno = 1, start = 0, end = strlen(ParseStr);
    bool multiline = 0;
    for (int i = start; i < pos; i++)
	if (ParseStr[i] == '\n') { start = i + 1; lineno++; multiline=1; }

    // find end of line containing error
    for (int i = end; i > pos; i--)
	if (ParseStr[i] == '\n') { end = i - 1; multiline=1; }

    ParseError = SeExpr2Spectext[0] ? "Syntax error" : "Unexpected end of expression";
    if (multiline) {
	char buff[30];
	snprintf(buff, 30, " at line %d", lineno);
	ParseError += buff;
    }
    if (SeExpr2Spectext[0]) {
	ParseError += " near '";
	ParseError += SeExpr2Spectext;
    }
    ParseError += "':\n    ";

    int s = std::max(start, pos-30);
    int e = std::min(end, pos+30);

    if (s != start) ParseError += "...";
    ParseError += std::string(ParseStr, s, e-s+1);
    if (e != end) ParseError += "...";
}

extern void specResetCounters(std::vector<std::pair<int,int> >& comments);


/* CallParser - This is our entrypoint from the rest of the expr library. 
   A string is passed in and a parse tree is returned.	If the tree is null,
   an error string is returned.  Any flags set during parsing are passed
   along.
 */

static SeExprInternal2::Mutex mutex;

/// Main entry point to parser
bool ExprSpecParse(std::vector<Editable*>& outputEditables,
    std::vector<std::string>& outputVariables,
    std::vector<std::pair<int,int> >& comments,
    const char* str)
{
    SeExprInternal2::AutoMutex locker(mutex);

    /// Make inputs/outputs accessible to parser actions
    editables=&outputEditables;
    variables=&outputVariables;
    ParseStr=str;

    // setup and startup parser
    specResetCounters(comments); // reset lineNumber and columnNumber in scanner
    SeExpr2Spec_buffer_state* buffer = SeExpr2Spec_scan_string(str); // setup lexer
    ParseResult = 0;
    int resultCode = SeExpr2Specparse(); // parser (don't care if it is a parse error)
    UNUSED(resultCode);
    SeExpr2Spec_delete_buffer(buffer);

    // delete temporary data -- specs(mini parse tree) and tokens(strings)!
    for(size_t i=0;i<specNodes.size();i++) delete specNodes[i];
    specNodes.clear();
    for(size_t i=0;i<tokens.size();i++) free(tokens[i]);
    tokens.clear();
    return true;
}


