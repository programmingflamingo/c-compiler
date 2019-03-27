#include "symbolTable.h"
#include "ast.h"


extern SymbolTableStackEntryPtr symbolStackTop;

extern int scopeDepth;

//creates a new expression node
AstNodePtr  new_ExprNode(ExpKind kind) 
{
  AstNodePtr myN = (AstNodePtr) malloc(sizeof(AstNode));
  if(myN == NULL) {
    return myN;
  }

  myN->nKind = kind;
  myN->eKind = EXPRESSION;
  int i;
  for(i=0; i<MAXCHILDREN; i++) {
    myN->children[i] = NULL;
  }
  myN->sibling = NULL;
  myN->nType = NULL;
  myN->nSymbolPtr = NULL;
  myN->nValue = -1;
  myN->nSymbolTabPtr = NULL;
  myN->fname = NULL;
  myN->nLinenumber = -1;
  
  return myN;
}

//creates a new statement node
AstNodePtr new_StmtNode(StmtKind kind)
{
  AstNodePtr myN = (AstNodePtr) malloc(sizeof(AstNode));

  if(myN == NULL) {
    return myN;
  }

  myN->nKind = kind;
  myN->eKind = STMT;
  int i;
  for(i=0; i<MAXCHILDREN; i++) {
    myN->children[i] = NULL;
  }
  myN->sibling = NULL;
  myN->nType = NULL;
  myN->nSymbolPtr = NULL;
  myN->nValue = -1;
  myN->nSymbolTabPtr = NULL;
  myN->fname = NULL;
  myN->nLinenumber = -1;
  
  return myN;
}

//creates a new type node for entry into symbol table
Type* new_type(TypeKind k)
{

  Type *myT = (Type *) malloc(sizeof(Type));

  if(myT == NULL) {
    return myT;
  }

  myT->kind = k;

  return myT;
}




