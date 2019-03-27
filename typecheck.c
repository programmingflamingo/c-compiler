
#include "symbolTable.h"
#include "ast.h"

extern AstNodePtr program;
extern SymbolTableStackEntryPtr symbolStackTop;


// compares two types and returns the resulting type
// from the comparison

Type* type_equiv(Type *t1, Type *t2){
  if(t1->kind == t2->kind && t1->dimension == t2->dimension)
    return t1;
  return NULL;
}
// Typechecks a method and returns 1 on success
int typecheck_method(AstNode *node){

  return 1;
}





TypePtr varArray (SymbolTablePtr scope) {

  TypePtr t = NULL;
  int i;
	
	for(i=0; i<MAXHASHSIZE;i++) {	
	  ElementPtr symelement = scope->hashTable[i];
		while(symelement) {

      switch(symelement->stype->kind) {
		    case INT :
          t = new_type(INT);
		      break;
		    case ARRAY :
          t = new_type(ARRAY);
		      break;
		    case VOID:
  		    break;
		    case FUNCTION :
  		    break;
		  }
			symelement = symelement->next; 
		}		
	}	
  return t;
}

TypePtr varDeclarations (TypePtr t) {

  switch (t->kind) 
  {
      case VOID: //printf(" enum: VOID\n"); 
          return t;

      case INT: //printf(" enum: INT\n"); 
          return t;

      case ARRAY://printf(" enum: ARRAY\n"); 
          return t;

      case FUNCTION://printf(" enum: FUNCTION\n"); 
          return varDeclarations(t->function);
  }
  
  return t;

}


// Type checks a given expression and returns its type
// 

Type *typecheck_expr (AstNode *node){
  //printf("  EXPRESSION\n");

  TypePtr a = NULL;
  TypePtr b = NULL;
  TypePtr ret = NULL;
  

  if(node == NULL) {
    ret = new_type(VOID);
    return ret;
  }

  switch (node->eKind) 
  {
      case VAR_EXP: //printf(" enum: VAR_EXP\n"); 
          ret = varDeclarations(node->nSymbolPtr->stype);
          break;

      case ARRAY_EXP: //printf(" enum: ARRAY_EXP\n"); 
          ;
          a = varDeclarations(node->nSymbolPtr->stype);
          b = typecheck_expr(node->children[0]);
          if(a->kind == ARRAY && b->kind == INT)
            ret = new_type(INT);

          else if(b->kind != INT) {
            printf("Line %d: Array index cannot be VOID.\n", node->nLinenumber);
          }

          else {
            printf("Line %d: Cannot index variable of type INT.\n", node->nLinenumber);
          }
          break;

      case ASSI_EXP: //printf(" enum: ASSI_EXP\n"); 
          a = typecheck_expr(node->children[0]);
          b = typecheck_expr(node->children[1]);

          if(a == NULL || b == NULL)
            ret = NULL;
          else {
            if(a->kind == b->kind)
              ret = new_type(INT);
            else {
              printf("Line %d: Type Mismatch (ASSIGN).\n", node->nLinenumber);
              ret = NULL;
            }
          }
          break;

      case ADD_EXP: //printf(" enum: ADD_EXP\n"); 
          a = typecheck_expr(node->children[0]);
          b = typecheck_expr(node->children[1]);

          if(a == NULL || b == NULL)
            ret = NULL;
          else {
            if(a->kind == b->kind)
              ret = new_type(INT);
            else {
              printf("Line %d: Type Mismatch (ADD).\n", node->nLinenumber);
              ret = NULL;
            }
          }
          break;

      case SUB_EXP: //printf(" enum: SUB_EXP\n"); 
          a = typecheck_expr(node->children[0]);
          b = typecheck_expr(node->children[1]);

          if(a == NULL || b == NULL)
            ret = NULL;
          else {
            if(a->kind == b->kind)
              ret = new_type(INT);
            else {
              printf("Line %d: Type Mismatch (SUB).\n", node->nLinenumber);
              ret = NULL;
            }
          }
          break;

      case MULT_EXP: //printf(" enum: MULT_EXP\n");
          a = typecheck_expr(node->children[0]);
          b = typecheck_expr(node->children[1]);

          if(a == NULL || b == NULL)
            ret = NULL;
          else {
            if(a->kind == b->kind)
              ret = new_type(INT);
            else {
              printf("Line %d: Type Mismatch (MULT).\n", node->nLinenumber);
              ret = NULL;
            }
          }
          break;

      case DIV_EXP: //printf(" enum: DIV_EXP\n"); 
          a = typecheck_expr(node->children[0]);
          b = typecheck_expr(node->children[1]);

          if(a == NULL || b == NULL)
            ret = NULL;
          else {
            if(a->kind == b->kind)
              ret = new_type(INT);
            else {
              printf("Line %d: Type Mismatch (DIV).\n", node->nLinenumber);
              ret = NULL;
            }
          }
          break;

      case GT_EXP: //printf(" enum: GT_EXP\n"); 
          a = typecheck_expr(node->children[0]);
          b = typecheck_expr(node->children[1]);

          if(a == NULL || b == NULL)
            ret = NULL;
          else {
            if((a->kind == b->kind) && (a->kind == INT))
              ret = new_type(INT);
            else {
              printf("Line %d: Cannot compare type VOID.\n", node->nLinenumber);
              ret = NULL;
            }
          }
          break;

      case LT_EXP: //printf(" enum: LT_EXP\n"); 
          a = typecheck_expr(node->children[0]);
          b = typecheck_expr(node->children[1]);

          if(a == NULL || b == NULL)
            ret = NULL;
          else {
            if((a->kind == b->kind) && (a->kind == INT))
              ret = new_type(INT);
            else {
              printf("Line %d: Cannot compare type VOID.\n", node->nLinenumber);
              ret = NULL;
            }
          }
          break;

      case GE_EXP: //printf(" enum: GE_EXP\n"); 
          a = typecheck_expr(node->children[0]);
          b = typecheck_expr(node->children[1]);

          if(a == NULL || b == NULL)
            ret = NULL;
          else {
            if((a->kind == INT) && (b->kind == INT))
              ret = new_type(INT);
            else {
              printf("Line %d: Cannot compare type VOID.\n", node->nLinenumber);
              ret = NULL;
            }
          }
          break;

      case LE_EXP: //printf(" enum: LE_EXP\n"); 
          a = typecheck_expr(node->children[0]);
          b = typecheck_expr(node->children[1]);

          if(a == NULL || b == NULL)
            ret = NULL;
          else {
            if((a->kind == b->kind) && (a->kind == INT))
              ret = new_type(INT);
            else {
              printf("Line %d: Cannot compare type VOID.\n", node->nLinenumber);
              ret = NULL;
            }
          }
          break;

      case EQ_EXP: //printf(" enum: EQ_EXP\n"); 
          a = typecheck_expr(node->children[0]);
          b = typecheck_expr(node->children[1]);

          if(a == NULL || b == NULL)
            ret = NULL;
          else {
            if(a->kind == b->kind)
              ret = new_type(INT);
            else {
              printf("Line %d: Type Mismatch (EQUAL).\n", node->nLinenumber);
              ret = NULL;
            }
          }
          break;

      case NE_EXP: //printf(" enum: NE_EXP\n"); 
          a = typecheck_expr(node->children[0]);
          b = typecheck_expr(node->children[1]);

          if(a == NULL || b == NULL)
            ret = NULL;
          else {
            if(a->kind == b->kind)
              ret = new_type(INT);
            else {
              printf("Line %d: Type Mismatch (NOT EQUAL).\n", node->nLinenumber);
              ret = NULL;
            }
          }
          break;

      case CALL_EXP: //printf(" enum: CALL_EXP\n"); 
          ;
          int flag1 = 0;
          AstNodePtr temp = program;
          while(temp != NULL) {
            if(node->nSymbolPtr != NULL) {           
              if(strcmp(temp->nSymbolPtr->id, node->nSymbolPtr->id)==0) {
                if(temp->nKind == METHOD) {
                  flag1 = 1;
                  AstNodePtr x = node->children[0];
                  AstNodePtr y = temp->children[0];
                  int c1 = 0, c2 = 0;

                  while(x!=NULL) {
                    c1++;
                    x = x->sibling;
                  }

                  while(y!=NULL) {
                    c2++;
                    y = y->sibling;
                  }

                  if(c1 != c2) {
                    printf("Line %d: Wrong Number of Arguments.\n", node->nLinenumber);
                    ret = NULL;
                  }
                  else {
                    int flag2 = 0;
                    x = node->children[0];
                    y = temp->children[0];
                    while (x!=NULL) {
                      if(varDeclarations(typecheck_expr (x))->kind!=y->nType->kind) {
                        flag2 = 1;
                        printf("Line %d: Wrong Argument Type.\n", node->nLinenumber);
                        ret = NULL;
                      }
                      x = x->sibling;
                      y = y->sibling;         
                    }
                    if(flag2 == 0)
                      ret = varDeclarations(temp->nType);

                  }
                }
              }
            }
            temp = temp->sibling;
          }
          if(flag1==0) {
            printf("Line %d: Forward Reference.\n", node->nLinenumber);
            ret = NULL;
          }
          break;

      case CONST_EXP: //printf(" enum: CONST_EXP\n"); 
          ret = new_type(INT);
          break;
  }


  return ret;
}

// Typechecks a statement and returns 1 on success
int typecheck_stmt(AstNode *node, TypePtr t ){
  //printf("  STMT\n");

  int ret = 1;

  if(node == NULL)
    return ret;

  switch (node->sKind) 
  {
      case IF_THEN_ELSE_STMT: //printf(" enum: IF_THEN_ELSE_STMT\n"); 
          ;
          TypePtr i = typecheck_expr(node->children[0]);

          if(i->kind == VOID) {
            printf("Line %d: If condition cannot be VOID.\n", node->nLinenumber);
            ret = ret*0;
          }
          else if(i->kind == ARRAY) {
            printf("Line %d: If condition cannot be ARRAY.\n", node->nLinenumber);
            ret = ret*0;
          }
          else {
            if(node->children[1]!=NULL) {
              ret = ret * typecheck_stmt(node->children[1], t);
            }
            if(node->children[2]!=NULL) {
              ret = ret * typecheck_stmt(node->children[2], t);
           }
          }
          break;

      case WHILE_STMT: //printf(" enum: WHILE_STMT\n"); 
          ;
          TypePtr w = typecheck_expr(node->children[0]);

          if(w->kind == VOID) {
            printf("Line %d: While condition cannot be VOID.\n", node->nLinenumber);
            ret = ret*0;
          }
          else if(w->kind == ARRAY) {
            printf("Line %d: While condition cannot be ARRAY.\n", node->nLinenumber);
            ret = ret*0;
          }
          else
            if(node->children[1]!=NULL) {
              typecheck_stmt(node->children[1], t);
            }
          break;

      case RETURN_STMT: //printf(" enum: RETURN_STMT\n"); 
          ;
          TypePtr n = typecheck_expr(node->children[0]);
          if(n->kind == t->kind) {
            ret = ret * 1;
          }
          else {
            printf("Line %d: Incorrect Return Type.\n", node->nLinenumber);
            ret = ret * 0;
          }

          break;

      case COMPOUND_STMT: //printf(" enum: COMPOUND_STMT\n"); 
          //if(node->nSymbolTabPtr != NULL) {
          //  TypePtr x = varArray(node->nSymbolTabPtr);
          //}

          if(node->children[0]!=NULL)
            ret = ret * typecheck_stmt(node->children[0], t);
          break;

      case EXPRESSION_STMT: //printf(" enum: EXPRESSION_STMT\n"); 
          if(typecheck_expr(node->children[0])==NULL)
            ret = ret * 0;
          else 
            ret = ret * 1;
          break;
  }
  ret = ret * typecheck_stmt(node->sibling, t);

  return ret;
}



int typecheckRecursion(AstNodePtr root, int *ret, TypePtr temp){

  //printf("**START**\n");

   if(root == NULL)
      return (*ret);
     
   switch(root->nKind) {
   case METHOD :
      temp = new_type(varDeclarations(root->nType)->kind);
      if(root->children[0] != NULL)
         typecheckRecursion(root->children[0], ret, temp); // print the parameters of the method
      typecheckRecursion(root->children[1], ret, temp); // print the body of the method
      typecheckRecursion(root->sibling, ret, temp); // print the next method
      break;

   case FORMALVAR :
      if(root->sibling != NULL) {
         typecheckRecursion(root->sibling, ret, temp);
      }
      break;

   case STMT :
      (*ret) = (*ret) * typecheck_stmt(root, temp);
      break;

   case EXPRESSION :
      typecheck_expr(root);
      break;
   }

  return (*ret);
}


int typecheck(){
  int i = 1;
  AstNodePtr root = program;

  typecheckRecursion(root, &i, NULL);
  return i;
}






