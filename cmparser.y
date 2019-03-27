%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"

/* external function prototypes */
extern int yylex();
extern int initLex(int ,  char **);
 
    

/* external global variables */

extern int		yydebug;
extern int		yylineno;


/* function prototypes */ 
void	yyerror(const char *);

/* global variables */

%}

/* YYSTYPE */

/* terminals */
/* Start adding token names here */
/* Your token names must match Project 1 */
/* The file cmparser.tab.h was gets generated here */


%start Start

%token TOK_IF
%token TOK_ELSE
%token TOK_RETURN
%token TOK_INT
%token TOK_VOID
%token TOK_WHILE

%token TOK_ERROR

%token TOK_ID

%token TOK_NUM

%token TOK_SEMI
%token TOK_COMMA
%token TOK_LPAREN
%token TOK_RPAREN
%token TOK_LSQ
%token TOK_RSQ
%token TOK_LBRACE
%token TOK_RBRACE

%token TOK_PLUS
%token TOK_MINUS
%token TOK_MULT
%token TOK_DIV
%token TOK_LT
%token TOK_LE
%token TOK_GT
%token TOK_GE
%token TOK_EQ
%token TOK_NE
%token TOK_ASSIGN



/* associativity and precedence */
/* specify operator precedence (taken care of by grammar) and associatity here --uncomment */

%right '='
%left "==" "!=" 
%nonassoc '<' '>' "<=" ">="
%left '+' '-' 
%left '*' '/' 

%nonassoc IF-THEN
%nonassoc TOK_ELSE

/* Begin your grammar specification here */
%%

/*1*/
Start	: 
        Declarations
      ; /* note that the rule ends with a semicolon */

/*2*/
Declarations : 
        Var-declaration Declarations { $$ = $1; }
      | Fun-declaration Declarations { $$ = $1; }
      | Fun-declaration { $$ = $1; }
      ;

//POSSIBLE ERROR WITH TOK_NUM
//allows for int a[0];
//also another production might be needed
//int a[b];
//TOK_ID
/*3-4*/
Var-declaration : 
        Type-specifier TOK_ID TOK_SEMI
      | Type-specifier TOK_ID TOK_LSQ TOK_NUM TOK_RSQ TOK_SEMI //<==
      ;

/*5*/
Type-specifier : 
        TOK_INT { $$ = $1; }
      | TOK_VOID { $$ = $1; }
      ;

/*6*/
Fun-declaration : 
        Type-specifier TOK_ID TOK_LPAREN Params TOK_RPAREN Compound-stmt { $$ = $1; }
  ;

/*7*/
Params : 
        Param-list { $$ = $1; }
      | TOK_VOID { $$ = $1; };
   ;

/*8*/
Param-list : 
        Param-list TOK_COMMA Param { $$ = $1; }
      | Param { $$ = $1; };
   ;

/*9*/
Param : 
        Type-specifier TOK_ID { $$ = $1; }
      | Type-specifier TOK_ID TOK_LSQ TOK_RSQ { $$ = $1; };
   ;

/*10*/
Compound-stmt : 
        TOK_LBRACE X Y TOK_RBRACE;
   ;

X :
        Local-declaration X { };
      | %empty { };

Y :
        Statement Y { };
      | %empty { };


/*11*/
Local-declaration : 
        Var-declaration { $$ = $1; }
   ;

/*12-13*/
Statement :
        Expression-stmt;
      | Compound-stmt; 
      | Selection-stmt;
      | Iteration-stmt;
      | Return-stmt;

/*14*/
Expression-stmt :
        Expression TOK_SEMI;
      | TOK_SEMI;

/*15-16*/
Selection-stmt :
        TOK_IF TOK_LPAREN Expression TOK_RPAREN Statement %prec IF-THEN;
      | TOK_IF TOK_LPAREN Expression TOK_RPAREN Statement TOK_ELSE Statement %prec TOK_ELSE;

/*17*/
Iteration-stmt :
        TOK_WHILE TOK_LPAREN Expression TOK_RPAREN Statement;

/*18*/
Return-stmt :
        TOK_RETURN Expression TOK_SEMI;
      | TOK_RETURN TOK_SEMI;

/*19*/
Expression :
        Var TOK_ASSIGN Expression;
      | Simple-expression;

/*20*/
Var :
        TOK_ID;
      | TOK_ID TOK_LSQ Expression TOK_RSQ;

/*21*/
Simple-expression :
        Additive-expression Rel-op Additive-expression;
      | Additive-expression;

/*22*/
Rel-op :
        TOK_LT;
      | TOK_LE;
      | TOK_GT;
      | TOK_GE;
      | TOK_EQ;
      | TOK_NE;

/*23*/
Additive-expression :
        Additive-expression Add-op Term;
      | Term;

/*24*/
Add-op :
        TOK_PLUS;
      | TOK_MINUS;

/*25*/
Term :
        Term Mult-op Factor;
      | Factor;

/*26*/
Mult-op :
        TOK_MULT;
      | TOK_DIV;

/*27*/
Factor :
        TOK_LPAREN Expression TOK_RPAREN;
      | Var;
      | Call;
      | TOK_NUM;

/*28*/
Call :
        TOK_ID TOK_LPAREN Args TOK_RPAREN;

/*29*/
Args:
        Args-list;
      | %empty;

/*30*/
Args-list:
        Args-list TOK_COMMA Expression;
      | Expression;

%%
void yyerror (char const *s) {
       fprintf (stderr, "Line %d: %s\n", yylineno, s);
}

int main(int argc, char **argv){

	initLex(argc,argv);

#ifdef YYLLEXER
   while (gettok() !=0) ; //gettok returns 0 on EOF
    return;
#else
    yyparse();
    
#endif
    
} 
