/* Original Code: Silvano Bonacia, Fall 2006 */
/* Modifications and changes, Prof. Venkat, Spring 08 */

#include "ast.h"
#include "util.h"

extern AstNode *program;
extern SymbolTableStackEntryPtr symbolStackTop;

extern void printSymbolTable(); 

#define PAD_STR "   "

int indLevel = 0;

void pad (int level) {
     int i;
     for(i=0;i<level;i++)
        printf("%s", PAD_STR);
}

/*
 * Print the parameters of the method (parameters being in the
 * current scope)
 */
void printMethodParams (SymbolTableStackEntryPtr scope) {
	int i;
	SymbolTableStackEntryPtr SymbolStackEntry = scope;
	for(i=0; i<MAXHASHSIZE;i++) {	
	   ElementPtr symelement = SymbolStackEntry->symbolTablePtr->hashTable[i];
		while(symelement) {
		   switch(symelement->stype->kind) {
		   case INT :
		      printf("int %s", symelement->id);
		   break;
		   case ARRAY :
		      printf("int %s[]", symelement->id);
		   break;
		   case VOID :
		      printf("void");
		   break;
		   case FUNCTION :
		   break;
		   }
			symelement = symelement->next; 
			if(symelement)
			   printf(", ");
		}		
	}
} 

/*
 * Print the variable declarations of the scope.
 * Return the number of variables
 */
int printVarDeclarations (SymbolTablePtr scope)
{
	int noVar = 0;
    int i;
	
	for(i=0; i<MAXHASHSIZE;i++) {	

	   ElementPtr symelement = scope->hashTable[i];


		while(symelement) {


		   pad(indLevel);

           switch(symelement->stype->kind) {
		   case INT :

		      printf("int %s;\n", symelement->id);
		      noVar++;
		   break;
		   case ARRAY :
		      printf("int %s[%d];\n", symelement->id, symelement->stype->dimension);
		      noVar++;
		   break;
		   case VOID:

		   break;
		   case FUNCTION :

		   break;
		   }

			symelement = symelement->next; 
		}		
	}	
	return noVar;
}

//traverses through the entire program and symbol table  and prints it
//the output must match the program with the exception of white spaces
//do a diff -w to ignre white spaces 
void printType(TypePtr type, int dummy){
  printf("h2a\n");

	switch(type->kind) {
	   case INT :
      printf("h2b\n");
		   printf("int");
		break;
		case ARRAY :
      printf("h2c\n");
		   printf("int");
		break;
		case VOID:
      printf("h2d\n");
		   printf("void");
		break;
		case FUNCTION :
      printf("h2e\n");
		   printType(type->function, 0);
		break;	
	}
  printf("h2z\n");

}

void print_Expression(AstNodePtr expr, int endWithSemi) {
   printf("h3a\n");

   if(expr == NULL) {
      return;
   }

   printf("h3b\n");


   switch(expr->eKind) {
   case VAR_EXP :  
   printf("h3c\n");

      printf("%s", expr->nSymbolPtr->id);
   break;
   case ARRAY_EXP :
   printf("h3d\n");

      printf("%s[", expr->nSymbolPtr->id);
      print_Expression(expr->children[0], 0);
      printf("]");
   break;
   case ASSI_EXP :
   printf("h3e\n");

      print_Expression(expr->children[0], 0);
      printf(" = ");
      print_Expression(expr->children[1], 0);
   break;
   case ADD_EXP :
   printf("h3f\n");

      print_Expression(expr->children[0], 0);
      printf(" + ");
      print_Expression(expr->children[1], 0);
   break;
   case SUB_EXP :
   printf("h3g\n");

      print_Expression(expr->children[0], 0);
      printf(" - ");
      print_Expression(expr->children[1], 0);
   break;
   case MULT_EXP :
   printf("h3h\n");

      print_Expression(expr->children[0], 0);
      printf(" * ");
      print_Expression(expr->children[1], 0);
   break;
   case DIV_EXP :
   printf("h3i\n");

      print_Expression(expr->children[0], 0);
      printf(" / ");
      print_Expression(expr->children[1], 0);
   break;
   case GT_EXP :
   printf("h3j\n");

      print_Expression(expr->children[0], 0);
      printf(" > ");
      print_Expression(expr->children[1], 0);
   break;
   case LT_EXP :
   printf("h3k\n");

      print_Expression(expr->children[0], 0);
      printf(" < ");
      print_Expression(expr->children[1], 0);
   break;
   case GE_EXP :
   printf("h3l\n");

      print_Expression(expr->children[0], 0);
      printf(" >= ");
      print_Expression(expr->children[1], 0);
   break; 
   case LE_EXP :
   printf("h3m\n");

      print_Expression(expr->children[0], 0);
      printf(" <= ");
      print_Expression(expr->children[1], 0);
   break;
   case EQ_EXP :
   printf("h3n\n");

      print_Expression(expr->children[0], 0);
      printf(" == ");
      print_Expression(expr->children[1], 0);
   break;
   case NE_EXP :
   printf("h3o\n");

      print_Expression(expr->children[0], 0);
      printf(" != ");
      print_Expression(expr->children[1], 0);
   break;
   case CALL_EXP :
   printf("h3p\n");

      printf("%s(", expr->fname);
      if(expr->children[0] != NULL) {
         print_Expression(expr->children[0], 0);
         AstNodePtr ptr = expr->children[0]->sibling;
         while(ptr != NULL) {
            printf(", ");
            print_Expression(ptr, 0);
            ptr = ptr->sibling;
         }
      }
      printf(")");
   break;
   case CONST_EXP :
   printf("h3q\n");

      printf("%d", expr->nValue);
   break;
   }
   if(endWithSemi == 1)
      printf(";\n");
   printf("h3z\n");

}

void print_Statement(AstNodePtr stmt) {
   if(stmt == NULL)
      return;
   
   pad(indLevel);   
   switch(stmt->sKind) {
   case IF_THEN_ELSE_STMT :
      printf("if(");
      print_Expression(stmt->children[0], 0);
      printf(")\n");
      if(stmt->children[1] != NULL) {
         if(stmt->children[1]->sKind != COMPOUND_STMT) {
            indLevel++;
            print_Statement(stmt->children[1]);
            indLevel--;
         } else print_Statement(stmt->children[1]);  
      } else {
          pad(indLevel+1);
          printf(";\n");
      }
      if(stmt->children[2] != NULL) {
         pad(indLevel);                  
         printf("else\n");
         if(stmt->children[2]->sKind != COMPOUND_STMT) {
            indLevel++;
            print_Statement(stmt->children[2]);
            indLevel--;
         } else print_Statement(stmt->children[2]); 
      } 
   break;
   case WHILE_STMT :
      printf("while(");
      print_Expression(stmt->children[0], 0);
      printf(")\n");
      if(stmt->children[1] != NULL) {
         if(stmt->children[1]->sKind != COMPOUND_STMT) {
            indLevel++;
            print_Statement(stmt->children[1]);
            indLevel--;
         } else print_Statement(stmt->children[1]);
      } else {
          pad(indLevel+1);
          printf(";\n");
      }
   break;
   case RETURN_STMT :
      printf("return ");
      if(stmt->children[0] == NULL)
         printf(";\n");
      else   
         print_Expression(stmt->children[0], 1);
   break;
   case COMPOUND_STMT :
      printf("{\n");
      indLevel++;
      if(stmt->nSymbolTabPtr != NULL) {
         if(printVarDeclarations(stmt->nSymbolTabPtr) > 0)
            printf("\n");
      }  
      if(stmt->children[0] != NULL)          
         print_Statement(stmt->children[0]); // Print the first statement
      else {
         pad(indLevel);  
         printf(";\n");
      }
      indLevel--;
      pad(indLevel);
      printf("}\n");
   break;
   case EXPRESSION_STMT : 
      print_Expression(stmt->children[0], 1);
   break;
   }  
//printf("sibling : %d\n", stmt->sibling);   
   print_Statement(stmt->sibling); // Print the next statement
}

void print_Ast_Recursion(AstNodePtr root) { 
//printf("print_Ast_Recursion: root = %d\n", root);
   /*
    * End the recursion
    */
    printf("h1a\n");
   if(root == NULL)
      return;
     
    printf("h1b\n");
   switch(root->nKind) {
   case METHOD :
//printf("root->nKind = METHOD\n");   
    printf("h1ca\n");

      printType(root->nType, 0);
    printf("h1cb\n");

      printf(" %s(", root->nSymbolPtr->id);
    printf("h1cc\n");

      if(root->children[0] == NULL)
         printf("void");
      else
         print_Ast_Recursion(root->children[0]); // print the parameters of the method
      printf(")\n");
      print_Ast_Recursion(root->children[1]); // print the body of the method
      printf("\n");
      print_Ast_Recursion(root->sibling); // print the next method
    printf("h1cz\n");

   break;
   case FORMALVAR :
    printf("h1d\n");

//printf("root->nKind = FORMALVAR\n");   
      printType(root->nSymbolPtr->stype, 0); // print the type
      printf(" %s", root->nSymbolPtr->id); // print the name of the variable
      if(root->nSymbolPtr->stype->kind == ARRAY)
         printf("[]");
      /*
       * Print the next parameter if there's one
       */
      if(root->sibling != NULL) {
         printf(", ");
         print_Ast_Recursion(root->sibling);
      }
   break;
   case STMT :
    printf("h1e\n");

//printf("root->nKind = STMT\n");   
      print_Statement(root);
   break;
   case EXPRESSION :
    printf("h1f\n");

//printf("root->nKind = EXPRESSION\n");   
      print_Expression(root, 1); // I don't think it ever gets here
   break;
   }
    printf("h1z\n");

}

void print_Ast() {
   /*
    * First print the entire symbol table
    */
   //printSymbolTable();

   /*
    * Then print the program
    */
    printf("h0a\n");
   indLevel = 0; 
    printf("h0b\n");
   if(printVarDeclarations(symbolStackTop->symbolTablePtr) > 0)
      printf("\n");
    printf("h0c\n");
   print_Ast_Recursion(program);
    printf("h0d\n");
}

 

