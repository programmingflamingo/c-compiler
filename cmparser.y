%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"
#include "symbolTable.h"
#include "util.h"


/* other external function prototypes */
extern int yylex();
extern int initLex(int ,  char **);
 
    
/* external global variables */

extern int		yydebug;
extern int		yylineno;
extern SymbolTableStackEntryPtr symbolStackTop;
extern int scopeDepth;

/* function prototypes */ 
void	yyerror(const char *);

/* global variables */

AstNodePtr  program;
 

%}

/* YYSTYPE */
%union
{
    AstNodePtr nodePtr;
    int        iVal;
    char      *cVal;
    Type      *type;
}

    

/* terminals */

%token TOK_ELSE TOK_IF TOK_RETURN TOK_VOID TOK_INT TOK_WHILE 
%token TOK_PLUS TOK_MINUS TOK_MULT TOK_DIV TOK_LT TOK_LE TOK_GT TOK_GE TOK_EQ TOK_NE TOK_ASSIGN TOK_SEMI TOK_COMMA
%token TOK_LPAREN TOK_RPAREN TOK_LSQ TOK_RSQ TOK_LBRACE TOK_RBRACE TOK_ERROR
%token <cVal> TOK_ID 
%token <iVal> TOK_NUM

%type <nodePtr> Declarations
%type <nodePtr> Var_Declaration
%type <nodePtr> Fun_Declaration
%type <nodePtr> Functions
%type <nodePtr> Compound_Stmt
%type <nodePtr> Statements
%type <nodePtr> Statement

%type <nodePtr> Expr_Statement
%type <nodePtr> If_Else_Statement
%type <nodePtr> Selection_Stmt
%type <nodePtr> Iteration_Stmt
%type <nodePtr> Return_Stmt

%type <nodePtr> Params
%type <nodePtr> Param_List
%type <nodePtr> Param
%type <nodePtr> Local_Declarations
%type <nodePtr> Term

%type <nodePtr> Expression
%type <nodePtr> Simple_Expression
%type <nodePtr> Additive_Expression
%type <nodePtr> Factor Var Call
%type <nodePtr> Args 
%type <nodePtr> Args_List

%type <type> Type_Specifier 

/* associativity and precedence */

%nonassoc TOK_IF
%nonassoc TOK_ELSE
%right TOK_ASSIGN
%left TOK_EQ TOK_NE
%nonassoc TOK_LT TOK_GT	TOK_LE TOK_GE
%left TOK_PLUS TOK_SUB
%left TOK_MULT TOK_DIV 
%nonassoc error

%%


Start : 
        Declarations { 
          program = $1; 
        }
;

Declarations : 
        Functions { 
          $$ = $1; 
        }
      | Var_Declaration Declarations {
          AstNodePtr n = $2;
	        n->sibling = $1;
          $$ = n;
        }
;

Functions : 
        Fun_Declaration { 
          $$ = $1; 
        }
      | Fun_Declaration Functions {
          AstNodePtr n = $2;
	        n->sibling = $1;
          $$ = n;
        }
;

Var_Declaration : 
        Type_Specifier TOK_ID TOK_SEMI {
          if(symLookup($2)==NULL) {
            symInsert($2, $1, yylineno);
          }
        }
  		| Type_Specifier TOK_ID TOK_LSQ TOK_NUM TOK_RSQ TOK_SEMI {
          if(symLookup($2)==NULL) {
            symInsert($2, $1, yylineno);
          }
        }
;

Fun_Declaration : 
        Type_Specifier TOK_ID TOK_LPAREN Params TOK_RPAREN Compound_Stmt {

          //printf("hello5\n");
          AstNodePtr n = (AstNodePtr) malloc(sizeof(AstNode));

          n->nSymbolPtr = symLookup($2);

          if(n->nSymbolPtr==NULL) {
            n->nKind = METHOD;
          //n->sKind = NULL;
          //n->sKind = NULL;
            n->children[0] = $4;
            n->children[1] = $6;


          //n->sibling = NULL;

            n->nType = $1;
            n->nSymbolPtr = symInsert($2, $1, yylineno);
          //n->nValue;
          //n->nSymbolTabPtr;
          //n->fname;
            n->nLinenumber = yylineno;         
            $$ = n;
          }
        }
;

Params : 
        Param_List { $$ = $1; }
      | TOK_VOID { 
          //printf("hello\n");
          $$ = NULL;  
        }
;

Param_List : 
        Param_List TOK_COMMA Param {
	        AstNodePtr n = $3;
    	    n->sibling = $1;
          $$ = n;
        }
      | Param { $$ = $1; }
;

Param : 
        Type_Specifier TOK_ID  {

          AstNodePtr n = (AstNodePtr) malloc(sizeof(AstNode));

          n->nSymbolPtr = symLookup($2);

          if(n->nSymbolPtr==NULL) {
            n->nKind = FORMALVAR;
          //n->sKind = NULL;
          //n->sKind = NULL;

          //n->sibling = NULL;

            n->nType = $1;
            n->nSymbolPtr = symInsert($2, $1, yylineno);
          //n->nValue;
          //n->nSymbolTabPtr;
          //n->fname;
            n->nLinenumber = yylineno;         
            $$ = n;
          }
        }
      | Type_Specifier TOK_ID TOK_LSQ TOK_RSQ  {
          AstNodePtr n = (AstNodePtr) malloc(sizeof(AstNode));

          n->nSymbolPtr = symLookup($2);

          if(n->nSymbolPtr==NULL) {
            n->nKind = FORMALVAR;
          //n->sKind = NULL;
          //n->sKind = NULL;

          //n->sibling = NULL;

            n->nType = $1;
            n->nSymbolPtr = symInsert($2, $1, yylineno);
          //n->nValue;
          //n->nSymbolTabPtr;
          //n->fname;
            n->nLinenumber = yylineno;         
            $$ = n;
          }
        }
;

Type_Specifier : 
        TOK_INT { $$ = new_type(INT); }
      | TOK_VOID { $$ = new_type(VOID); }
;

          //  n->children[1]->nKind = STMT;
        //  printf("hello5e\n");
        //    n->children[1]->sKind = COMPOUND_STMT;

Compound_Stmt :
        TOK_LBRACE { enterScope(); }
        Statements TOK_RBRACE { leaveScope(); }
      | TOK_LBRACE { enterScope(); }
        Local_Declarations Statements TOK_RBRACE { leaveScope(); }
;

Local_Declarations : 
        Var_Declaration Local_Declarations {
	        AstNodePtr n = $1;
    	    n->sibling = $2;
          $$ = n;
        }
		  | Var_Declaration { $$ = NULL; } // leave this here
;

Statements : 
        Statement Statements {
	        AstNodePtr n = $1;
    	    n->sibling = $2;
          $$ = n;
        }
	    | { $$ = NULL; }
;

Statement : 
        Expr_Statement  { $$ = $1; }
	    | Compound_Stmt { $$ = $1; }
  	  | Selection_Stmt { $$ = $1; }
  	  | Iteration_Stmt { $$ = $1; }
  	  | Return_Stmt { $$ = $1; }
;

Expr_Statement : 
        Expression TOK_SEMI {}
      | TOK_SEMI {}
;

Selection_Stmt : 
        If_Else_Statement %prec TOK_IF {}
      | If_Else_Statement TOK_ELSE Statement {}
;

If_Else_Statement : 
        TOK_IF TOK_LPAREN Expression TOK_RPAREN Statement {}
;

Iteration_Stmt : 
        TOK_WHILE TOK_LPAREN Expression TOK_RPAREN Statement {}
;

Return_Stmt : 
        TOK_RETURN Expression TOK_SEMI {}
	    | TOK_RETURN TOK_SEMI {}
;

Expression : 
        Var TOK_ASSIGN Expression  {}
      | Simple_Expression { $$ = $1; }
;

Var : 
        TOK_ID {
          AstNodePtr n = new_ExprNode(VAR_EXP);
          n->nSymbolPtr = symLookup($1);
          n->nLinenumber = yylineno;         
          $$ = n;
        }
      | TOK_ID TOK_LSQ Expression TOK_RSQ {
          AstNodePtr n = new_ExprNode(ARRAY_EXP);
          n->nSymbolPtr = symLookup($1);
          n->nLinenumber = yylineno;         
          $$ = n;
        }
;

Simple_Expression : 
        Additive_Expression TOK_GT Additive_Expression {}
      | Additive_Expression TOK_LT Additive_Expression {}
      | Additive_Expression TOK_GE Additive_Expression {}
      | Additive_Expression TOK_LE Additive_Expression {}
      | Additive_Expression TOK_EQ Additive_Expression {}
      | Additive_Expression TOK_NE Additive_Expression {}
		  | Additive_Expression { $$ = $1; }
;

Additive_Expression : 
        Additive_Expression TOK_PLUS Term {}
      | Additive_Expression TOK_MINUS Term {}
		  | Term { $$ = $1; }
;

Term : 
        Term TOK_MULT Factor  {}
     |  Term TOK_DIV Factor {}
     | Factor { $$ = $1; }
;

Factor : 
        TOK_LPAREN Expression TOK_RPAREN { $$ = $2; }
      | Var { $$ = $1; }
      | Call { $$ = $1; }
      | TOK_NUM {}
;

Call : 
        TOK_ID TOK_LPAREN Args TOK_RPAREN {}
;

Args : 
        Args_List { $$ = $1; }
      | { $$ = NULL; }
;

Args_List : 
        Args_List TOK_COMMA Expression {}
	    | Expression { $$ = $1; }
;
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
    initSymbolTable();
    yyparse();

    printSymbolTable();

    printf("hello0f\n");
    print_Ast();

    printf("hello0g\n");
#endif
    
} 
