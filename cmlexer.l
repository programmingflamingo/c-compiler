
/**********************/
/* C header files */
/**********************/

%{
#include "globals.h"
#include "cmparser.tab.h"
char tokenString[TOKENMAX];
int printoutScan = 1;

%}
%option yylineno
/**********************/
/* start your regular definitions  here */
/**********************/
BCOM1       "/*"
BCOM2       (([^*])|("*"+[^*/]))*
BCOM3       "*"+"/"
LCOM        "//"[^\n]*"\n"
WHITESPACE  [ \n\t\r]

F1          "auto"|"break"|"case"|"char"|"const"
F2          "continue"|"default"|"do"|"double"|"for"|"byte"
F3          "goto"|"long"|"short"|"register"|"switch"|"float"
F4          "~"|"?"|":"|"++"|"--"|"!"|"&"|"|"
F5          "^"|"<<"|">>"|">>>"|"+="|"-="|"*="
F6          "/="|"&="|"|="|"^="|"%="|"<<="|">>="|">>>="

IDENTIFIER  [a-zA-Z_$][a-zA-Z0-9_$]*

NUMBER		  (0)|([1-9][0-9]*)



/* start your token specifications here */
/* Token names must come from cmparser.tab.h */

%%
{BCOM1}{BCOM2}{BCOM3} ;
{LCOM}                ;
{WHITESPACE}          ;

"if"                  { return TOK_IF; }
"else"                { return TOK_ELSE; }
"return"              { return TOK_RETURN; }
"int"                 { return TOK_INT; }
"void"                { return TOK_VOID; }
"while"               { return TOK_WHILE; }

{F1}|{F2}|{F3}        { return TOK_ERROR; }
{F4}|{F5}|{F6}        { return TOK_ERROR; }

{IDENTIFIER}          { return TOK_ID; }

{NUMBER}              { return TOK_NUM; }

";"                   {return TOK_SEMI;}
","                   {return TOK_COMMA;}
"("                   {return TOK_LPAREN;}
")"                   {return TOK_RPAREN;}
"["                   {return TOK_LSQ;}
"]"                   {return TOK_RSQ;}
"{"                   {return TOK_LBRACE;}
"}"                   {return TOK_RBRACE;}

"+"                   {return TOK_PLUS;}
"-"                   {return TOK_MINUS;}
"*"                   {return TOK_MULT;}
"/"                   {return TOK_DIV;}
"<"                   {return TOK_LT;}
"<="                  {return TOK_LE;}
">"                   {return TOK_GT;}
">="                  {return TOK_GE;}
"=="                  {return TOK_EQ;}
"!="                  {return TOK_NE;}
"="                   {return TOK_ASSIGN;}

.                     { return TOK_ERROR; }

%%
/**********************/
/* C support functions */
/**********************/

void printToken(int token, char *str)
{
/* Print the line number, token name and matched lexeme
   -- one per line without any additional characters exactly as below */ 
/* Example  13:TOK_INT: 37*/

    switch(token)
        {
            case TOK_IF:
                fprintf(yyout,"%5d:TOK_IF      : %s\n",yylineno,str);
                break;
            case TOK_ELSE:
                fprintf(yyout,"%5d:TOK_ELSE    : %s\n",yylineno,str);
                break;
            case TOK_RETURN:
                fprintf(yyout,"%5d:TOK_RETURN  : %s\n",yylineno,str);
                break;
            case TOK_INT:
                fprintf(yyout,"%5d:TOK_INT     : %s\n",yylineno,str);
                break;
            case TOK_VOID:
                fprintf(yyout,"%5d:TOK_VOID    : %s\n",yylineno,str);
                break;
            case TOK_WHILE:
                fprintf(yyout,"%5d:TOK_WHILE   : %s\n",yylineno,str);
                break;

            case TOK_ID:
                fprintf(yyout,"%5d:TOK_ID      : %s\n",yylineno,str);
                break;

            case TOK_NUM:
                fprintf(yyout,"%5d:TOK_NUM     : %s\n",yylineno,str);
                break;

            case TOK_SEMI:
                fprintf(yyout,"%5d:TOK_SEMI    : %s\n",yylineno,str);
                break;
            case TOK_COMMA:
                fprintf(yyout,"%5d:TOK_COMMA   : %s\n",yylineno,str);
                break;
            case TOK_LPAREN:
                fprintf(yyout,"%5d:TOK_LPAREN  : %s\n",yylineno,str);
                break;
            case TOK_RPAREN:
                fprintf(yyout,"%5d:TOK_RPAREN  : %s\n",yylineno,str);
                break;
            case TOK_LSQ:
                fprintf(yyout,"%5d:TOK_LSQ     : %s\n",yylineno,str);
                break;
            case TOK_RSQ:
                fprintf(yyout,"%5d:TOK_RSQ     : %s\n",yylineno,str);
                break;
            case TOK_LBRACE:
                fprintf(yyout,"%5d:TOK_LBRACE  : %s\n",yylineno,str);
                break;
            case TOK_RBRACE:
                fprintf(yyout,"%5d:TOK_RBRACE  : %s\n",yylineno,str);
                break;

            case TOK_PLUS:
                fprintf(yyout,"%5d:TOK_PLUS    : %s\n",yylineno,str);
                break;
            case TOK_MINUS:
                fprintf(yyout,"%5d:TOK_MINUS   : %s\n",yylineno,str);
                break;
            case TOK_MULT:
                fprintf(yyout,"%5d:TOK_MULT    : %s\n",yylineno,str);
                break;
            case TOK_DIV:
                fprintf(yyout,"%5d:TOK_DIV     : %s\n",yylineno,str);
                break;
            case TOK_LT:
                fprintf(yyout,"%5d:TOK_LT      : %s\n",yylineno,str);
                break;
            case TOK_LE:
                fprintf(yyout,"%5d:TOK_LE      : %s\n",yylineno,str);
                break;
            case TOK_GT:
                fprintf(yyout,"%5d:TOK_GT      : %s\n",yylineno,str);
                break;
            case TOK_GE:
                fprintf(yyout,"%5d:TOK_GE      : %s\n",yylineno,str);
                break;
            case TOK_EQ:
                fprintf(yyout,"%5d:TOK_EQ      : %s\n",yylineno,str);
                break;
            case TOK_NE:
                fprintf(yyout,"%5d:TOK_NE      : %s\n",yylineno,str);
                break;
            case TOK_ASSIGN:
                fprintf(yyout,"%5d:TOK_ASSIGN  : %s\n",yylineno,str);
                break;

            case TOK_ERROR:
                fprintf(yyout,"%5d:TOK_ERROR   : %s\n",yylineno,str);
                break;
        }
}


int gettok(void){
    int currentToken;
    
    currentToken=yylex();
    if (currentToken == 0) { // means EOF}
            return 0;
    }
    strncpy(tokenString, yytext, TOKENMAX);
    if (printoutScan) {
        printToken(currentToken,tokenString);
    }
    return currentToken;
}

int main(int argc, char **argv){

    
   if ( argc > 1 )
       yyin = fopen( argv[1], "r" );
   else
    yyin = stdin;

   while (gettok() !=0) ; //gettok returns 0 on EOF
    return;
} 
