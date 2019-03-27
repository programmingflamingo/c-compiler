#include "ast.h"
#include "codegen.h"

extern FILE *outfile;
extern AstNodePtr program;
extern SymbolTableStackEntryPtr symbolStackTop;

int numGlobals;
int elseCount=0;

int varDeclarations (SymbolTablePtr scope, int isGlobal)
{

	int numVar = 0;
  int i;
	
	for(i=0; i<MAXHASHSIZE;i++) {	
	  ElementPtr symelement = scope->hashTable[i];
		while(symelement) {
      switch(symelement->stype->kind) {
		    case INT :
          if(strcmp(symelement->id,"input")==0)
            break;
		      //printf(" int %s;\n", symelement->id);
		      numVar++;
          if(isGlobal)
            symelement->FPoffset = (4*numVar);
          else  
            symelement->FPoffset = -(4*numVar+4);
		      break;

		    case ARRAY :
		      //printf(" int %s[%d];\n", symelement->id, symelement->stype->dimension);
		      numVar++;
          if(isGlobal)
            symelement->FPoffset = (4*numVar);

          else  
            symelement->FPoffset = -(4*numVar+4);

		      break;

		    case VOID:
		      break;

		    case FUNCTION :
		      break;
		  }
			symelement = symelement->next; 
		}		
	}	
	return numVar;
}

int findOffset (SymbolTablePtr scope)
{
	int numVar = 0;
    int i;
	
	for(i=0; i<MAXHASHSIZE;i++) {	
	  ElementPtr symelement = scope->hashTable[i];
		while(symelement) {
      switch(symelement->stype->kind) {
		    case INT :
		      //printf(" int %s;\n", symelement->id);
          numVar++;
		      break;

		    case ARRAY :
		      //printf(" int %s[%d];\n", symelement->id, symelement->stype->dimension);
          numVar++;
		      break;

		    case VOID:
		      break;

		    case FUNCTION :
		      break;
		  }
			symelement = symelement->next; 
		}		
	}	
	return 4*numVar;
}

void pushStack() {
  printf("\tsubu $sp, $sp, 4\t#push stack space for $v0 in $sp+0\n");
  printf("\tsw $v0, 0($sp)\n");

  emit("subu $sp, $sp, 4");
  emit("sw $v0, 0($sp)");
}

void outPart1() {
  printf("\tli $v0, 4\n");
  printf("\tla $a0, str\n");
  printf("\tsyscall\n");

  emit("li $v0, 4");
  emit("la $a0, str");
  emit("syscall");
}

void outPart2() {
  printf("\tli $v0, 4\n");
  printf("\tla $a0, newline\n");
  printf("\tsyscall\n");


  emit("li $v0, 4");
  emit("la $a0, newline");
  emit("syscall");
}

void popStack() {
  printf("\tlw $v1, 0($sp)\n");
  printf("\taddu $sp, $sp, 4\t#pop stack space in $sp+0\n");

  emit("lw $v1, 0($sp)");
  emit("addu $sp, $sp, 4");
}

void emit_label(const char *label){
	fprintf(outfile, "%s\n", label); 
}
void emit(const char *instr){
	fprintf(outfile, "\t%s\n", instr);
}

void header(void) {



  return;
}

char *gen_new_label(){
;
}

void code_gen_expr(AstNode *node){

	Type *type1,*type2;
	AstNodePtr params;
	ElementPtr elem;
  char a[50];
  char temp[12];
  int t;

	switch(node->eKind)
	{
		case CONST_EXP:
      t = node->nValue;
      sprintf(temp, "%d", t);  
      strcpy(a,"li $v0, ");
      strcat(a, temp);
      emit(a);
			printf("\tli $v0, %d\t\t#Load constant %d into register v0\n", t, t);
			break;

		case VAR_EXP:
      t = (node->nSymbolPtr->FPoffset);
      sprintf(temp, "%d", t);  
      strcpy(a,"lw $v0, ");
      strcat(a, temp);
      strcat(a, "($fp)");
      emit(a);
			printf("\tlw $v0, %d($fp)\n",t);			//\t\t#Load contents of memory address $fp-%d into $v0
			break;

		case ADD_EXP:
			code_gen_expr(node->children[0]);
      pushStack();

			code_gen_expr(node->children[1]);
      popStack();

      printf("\taddu $v0, $v0, $v1\t#add_exp $v0 = $v0 + $v1\n");
      emit("addu $v0, $v0, $v1");
			break;

		case SUB_EXP:
			code_gen_expr(node->children[0]);
      pushStack();

			code_gen_expr(node->children[1]);
      popStack();
      printf("\tsub $v0, $v1, $v0\n");
      emit("sub $v0, $v1, $v0");
			break;

		case MULT_EXP:
			code_gen_expr(node->children[0]);
      pushStack();

			code_gen_expr(node->children[1]);
      popStack();
      printf("\tmul $v0, $v0, $v1\n");
      emit("mul $v0, $v0, $v1");
			break;

		case DIV_EXP:
			code_gen_expr(node->children[0]);
      pushStack();

			code_gen_expr(node->children[1]);
      popStack();
      printf("\tdivu $v0, $v1, $v0\n");
      emit("divu $v0, $v1, $v0");
			break;

		case EQ_EXP:
			printf("eq_exp\n");
      printf("seq\n");
			code_gen_expr(node->children[0]);
      pushStack();

			code_gen_expr(node->children[1]);
      popStack();
      printf("\tseq $v0, $v1, $v0\n");
      emit("seq $v0, $v1, $0");
			break;

		case NE_EXP:
			printf("ne_exp\n");
      printf("sne\n");
			code_gen_expr(node->children[0]);
      pushStack();

			code_gen_expr(node->children[1]);
      popStack();
      printf("\tseq $v0, $v1, $v0\n");
      emit("seq $v0, $v1, $0");
			break;

		case GT_EXP:
			//printf("gt_exp\n");
      //printf("sgt\n");
			code_gen_expr(node->children[0]);
      pushStack();

			code_gen_expr(node->children[1]);
      popStack();
      printf("\tsgt $v0, $v1, $v0\n");
      emit("sgt $v0, $v1, $0");
			break;

		case GE_EXP:
			printf("ge_exp\n");
      printf("sge\n");
			code_gen_expr(node->children[0]);
      pushStack();

			code_gen_expr(node->children[1]);
      popStack();
      printf("\tsge $v0, $v1, $v0\n");
      emit("sge $v0, $v1, $0");
			break;

		case LT_EXP:
			printf("lt_exp\n");
      printf("slt\n");
			code_gen_expr(node->children[0]);
      pushStack();

			code_gen_expr(node->children[1]);
      popStack();
      printf("\tslt $v0, $v1, $v0\n");
      emit("slt $v0, $v1, $0");
			break;

		case LE_EXP:
			printf("le_exp\n");
      printf("sle\n");
			code_gen_expr(node->children[0]);
      pushStack();

			code_gen_expr(node->children[1]);
      popStack();
      printf("\tsle $v0, $v1, $v0\n");
      emit("sle $v0, $v1, $0");
			break;

		case ASSI_EXP:
			code_gen_expr(node->children[1]);
      //printf("assi_exp\n");

      t = (node->children[0]->nSymbolPtr->FPoffset);
      sprintf(temp, "%d", t);  
      strcpy(a,"sw $v0, ");
      strcat(a, temp);
      strcat(a, "($fp)");
      printf("\tsw $v0, %d($fp)\n",t);
      emit(a);
			break;

		case CALL_EXP:

      if(strcmp(node->fname,"output")==0) {
        //printf("output\n");
        outPart1();

        printf("\tli $v0, 1\n");
        printf("\tlw $a0, %d($fp)\n", node->children[0]->nSymbolPtr->FPoffset);
        printf("\tsyscall\n");

        t = (node->children[0]->nSymbolPtr->FPoffset);
        sprintf(temp, "%d", t);  
        strcpy(a,"lw $a0, ");
        strcat(a, temp);
        strcat(a, "($fp)");

        emit("li $v0, 1");
        emit(a);
        emit("syscall");

        outPart2();
      }

      else if(strcmp(node->fname,"input")==0) {
        //printf("input\n");
      }

      else {

        int count=0;
        for(count=0;count<numGlobals;count++) {
          printf("\tlw $t%d, %d($fp)\n", count,  4*(count+1));
          sprintf(temp, "%d", count);  
          strcpy(a,"lw $t");
          strcat(a, temp);
          strcat(a, ", ");
          sprintf(temp, "%d", 4*(count+1));  
          strcat(a, temp);
          strcat(a, "($fp)");
          emit(a);
        }


			  params = node->children[0];

			  while (params != NULL) {
          printf("\tlw $t%d, %d($fp)\n", count, params->nSymbolPtr->FPoffset);
          sprintf(temp, "%d", count);  
          strcpy(a,"lw $t");
          strcat(a, temp);
          strcat(a, ", ");
          sprintf(temp, "%d", params->nSymbolPtr->FPoffset);  
          strcat(a, temp);
          strcat(a, "($fp)");
          emit(a);// <------------
				  //code_gen_expr(params);
				  params = params->sibling;
          count++;
			  }

        printf("\tjal %s\n", node->fname);
        strcpy(a, "jal ");
        strcat(a, node->fname);
        emit(a);
        int c;
        for(c=0;c<numGlobals;c++) {
          printf("\tlw $v0, %d($gp)\t\t#global\n", 4*(c+1));
          printf("\tsw $v0, %d($fp)\t\t#global\n", 4*(c+1));
          sprintf(temp, "%d", 4*(c+1));  
          strcpy(a,"lw $v0, ");
          strcat(a, temp);
          strcat(a, "($gp)");
          emit(a);
          sprintf(temp, "%d", 4*(c+1));  
          strcpy(a,"sw $v0, ");
          strcat(a, temp);
          strcat(a, "($fp)");
          emit(a);
        }
      }
			break;

		case ARRAY_EXP:
			printf("array_exp\n");
			code_gen_expr(node->children[0]);
			break;
	}	
}

void code_gen_stmt(AstNode *node){
	AstNodePtr stmt;
  int numVar;
  char a[50];
  char temp[12];

	switch(node->sKind)
	{
		case IF_THEN_ELSE_STMT:
      elseCount+=1;
			code_gen_expr(node->children[0]);
      printf("\tbnez $v0, else%d\n", elseCount);
      sprintf(temp, "%d", elseCount);  
      strcpy(a,"bnez $v0, else");
      strcat(a, temp);
      emit(a);

			code_gen_stmt(node->children[1]);

      printf("\tb endElse%d\n", elseCount);
      sprintf(temp, "%d", elseCount);  
      strcpy(a,"b endElse");
      strcat(a, temp);
      emit(a);      

      printf("else%d:\n", elseCount);
      sprintf(temp, "%d", elseCount);  
      strcpy(a,"else");
      strcat(a, temp);
      strcat(a, ":");
      emit_label(a);

			if (node->children[2] != NULL) {
        code_gen_stmt(node->children[2]);
      }

      printf("endElse%d:\n", elseCount);
      sprintf(temp, "%d", elseCount);  
      strcpy(a,"endElse");
      strcat(a, temp);
      strcat(a, ":");
      emit_label(a);
			break;

		case WHILE_STMT:
			code_gen_expr(node->children[0]);
      code_gen_stmt(node->children[1]);
			break;

		case RETURN_STMT:
      if(node->children[0]!=NULL)
  			code_gen_expr(node->children[0]);
      printf("return\n");
			break;

		case COMPOUND_STMT:

      if(node->nSymbolTabPtr != NULL) {
        if((numVar = varDeclarations(node->nSymbolTabPtr, 0)) > 0) {

          numVar *= 4;
          sprintf(temp, "%d", numVar);  
          strcpy(a,"subu $sp, $sp, ");
          strcat(a, temp);
          printf("\t%s\t#allocate space for locals\n", a);
	        emit(a);
        }
      }

			stmt = node->children[0];
			while (stmt != NULL) {
				code_gen_stmt(stmt);
				stmt = stmt->sibling;
			}


			break;

		case EXPRESSION_STMT:
			if (node->children[0] != NULL)
			  code_gen_expr(node->children[0]);		
			break;
	}

}


void code_gen_method(AstNode *node, int globals){


  if(node == NULL)
    return;

  if(strcmp(node->nSymbolPtr->id,"input")==0) {
    //printf("input:\n");
  }

  else if(strcmp(node->nSymbolPtr->id,"output")==0) {
    //printf("output:\n");
  }

  else {

    
	  AstNodePtr params = node->children[0];
    while(params != NULL) {
      globals+=1;
      params->nSymbolPtr->FPoffset = 4*globals;
      //printf("%s:%d\n",params->nSymbolPtr->id, params->nSymbolPtr->FPoffset);
      params = params->sibling;
    }

    int numVar;
    char temp[12];
    char a[50];
    //strcpy(a, node->nSymbolPtr->id);
    //strcat(a, ":");
    //printf("%s\n", a);

    printf("\tsubu $sp, $sp, %d\t#make space on stack for $fp and $ra\n", 8+globals*4);
    printf("\tsw $fp, 4($sp)\t\t#save $fp\n");
    printf("\tsw $ra, 0($sp)\t\t#save $ra\n");
    printf("\taddiu $fp, $sp, 4\t#configure $fp to point to the base of the stack\n");
    

	  //emit_label(a);
    sprintf(temp, "%d", 8+globals*4);  
    strcpy(a,"subu $sp, $sp, ");
    strcat(a, temp);
	  emit(a);
	  emit("sw $fp, 4($sp)");
	  emit("sw $ra, 0($sp)");
	  emit("addiu $fp, $sp, 4");

    if(strcmp(node->nSymbolPtr->id,"main")!=0) {
      int i;
      for(i=0; i<globals; i++) {
        printf("\tsw $t%d, %d($fp)\n", i, 4*(i+1));

        sprintf(temp, "%d", i);  
        strcpy(a,"sw $t");
        strcat(a, temp);
        strcat(a, ", ");
        sprintf(temp, "%d", 4*(i+1));  
        strcat(a, temp);
        strcat(a, "($fp)");

        emit(a);
      }
    }

	  AstNodePtr stmt = node->children[1];
	  while (stmt != NULL) {
		  code_gen_stmt(stmt);

      if(stmt->nSymbolTabPtr != NULL) {
        if((numVar = findOffset(stmt->nSymbolTabPtr)) > 0);
      }

      int c;
      printf("\tsubu $gp, $gp, 8\n");
      emit("subu $gp, $gp, 8");
      for(c=0; c<numGlobals; c++) {
        printf("\tlw $v0, %d($fp)\t\t#global\n", 4*(c+1));
        printf("\tsw $v0, %d($gp)\t\t#global\n", 4*(c+1));
        sprintf(temp, "%d", 4*(c+1));  
        strcpy(a,"lw $v0, ");
        strcat(a, temp);
        strcat(a, "($fp)");
        emit(a);
        sprintf(temp, "%d", 4*(c+1));  
        strcpy(a,"sw $v0, ");
        strcat(a, temp);
        strcat(a, "($gp)");
        emit(a);
      }      

      sprintf(temp, "%d", numVar);  
      strcpy(a,"addu $sp, $sp, ");
      strcat(a, temp);
      printf("\tlw $ra, -4($fp)\t\t#restore value of $ra\n");
      printf("\tlw $fp, 0($fp)\t\t#restore value of $fp\n");
      printf("\t%s\t#adjust the stack back\n", a);
      emit("lw $ra, -4($fp)");
      emit("lw $fp, 0($fp)");
      emit(a);

      printf("\taddu $sp, $sp, %d\t#adjust the stack for $fp and $rp\n", 8+globals*4);

      sprintf(temp, "%d", 8+globals*4);  
      strcpy(a,"addu $sp, $sp, ");
      strcat(a, temp);
      emit(a);


		  stmt = stmt->sibling;
	  } 
  }
}

void codegen(){

  //print_Ast();
  char temp[12];
  char a[50];

  printf("\t.text\n");
  printf("\t.align 2\n");
  printf("\n");

	emit(".text");
	emit(".align 2");
	emit("");


  if((numGlobals = varDeclarations(symbolStackTop->symbolTablePtr, 1)) > 0) {
  }  

  //header();

  AstNodePtr currentFun;
  currentFun = program;
	while (currentFun != NULL) {
    if(strcmp(currentFun->nSymbolPtr->id,"main")==0) {		
      strcpy(a, currentFun->nSymbolPtr->id);
      strcat(a, ":");
      printf("%s\n", a);
      emit_label(a);

  
      //printf("\tsubu $sp, $sp, %d\t#globals\n", numVar*4);
      //strcpy(a, "subu $sp, $sp, ");
      //sprintf(temp, "%d", numVar*4);  
      //strcat(a, temp);
      //emit(a);

      code_gen_method(currentFun, numGlobals);
      //printf("\taddu $sp, $sp, %d\t#globals\n", numVar*4);
      //strcpy(a, "addu $sp, $sp, ");
      //sprintf(temp, "%d", numVar*4);  
      //strcat(a, temp);
      //emit(a);

      printf("\tjr $ra\t\t\t#back to caller\n");
      emit("jr $ra");
    }
		currentFun = currentFun->sibling;
	}

  currentFun = program;
	while (currentFun != NULL) {
    if(strcmp(currentFun->nSymbolPtr->id,"main")!=0)	{
      if(strcmp(currentFun->nSymbolPtr->id,"input")!=0)	{
        if(strcmp(currentFun->nSymbolPtr->id,"output")!=0)	{
          strcpy(a, currentFun->nSymbolPtr->id);
          strcat(a, ":");
          printf("%s\n", a);
          emit_label(a);

      
          //printf("\tsubu $sp, $sp, %d\t#globals\n", numVar*4);
          //strcpy(a, "subu $sp, $sp, ");
          //sprintf(temp, "%d", numVar*4);  
          //strcat(a, temp);
          //emit(a);

          code_gen_method(currentFun, numGlobals);
          //printf("\taddu $sp, $sp, %d\t#globals\n", numVar*4);
          //strcpy(a, "addu $sp, $sp, ");
          //sprintf(temp, "%d", numVar*4);  
          //strcat(a, temp);
          //emit(a);

          printf("\tjr $ra\t\t\t#back to caller\n");
          emit("jr $ra");
        }
      }
    }
		currentFun = currentFun->sibling;
	}


  printf("\n");
  printf("\t.data\n");

  printf("str:\n");
  printf("\t.asciiz \"Output: \"\n");
  printf("newline:\n");
  printf("\t.asciiz \"\\n\"\n");

  emit("");
  emit(".data");
  emit_label("str:");
  emit(".asciiz \"Output: \"");
  emit_label("newline:");
  emit(".asciiz \"\\n\"");
  


 /* from now on call code gen on the main stmt in main */
}	


