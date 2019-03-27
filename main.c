
#include "symbolTable.h"
extern SymbolTableStackEntryPtr symbolStacktop;

void header()
{
  printf("CS 473 - HW#2\n");
  printf("\n");
  printf("Michael Slomczynski\n");
  printf("  660304702\n");
  printf("  ms11\n");
  printf("\n");
  printf("\n");
  printf("Begin Main\n");
  return;
}

void printElemProperties(ElementPtr elem) {

  if(elem==NULL) {
    perror("  ++>Lookup");
  }
  
  else {
    printf("elem\n");
    printf("  elem->Key   : %d\n", elem->key);
    printf("  elem->ID    : %s\n", elem->id);
    printf("  elem->LN    : %d\n", elem->linenumber);
    printf("  elem->Scope : %d\n", elem->scope);
  }
  return;
}

void footer()
{
  printf("End Main\n");
  printf("\n");
  printf("\n");
  return;
}

int main()
{
    header();

    Type *typ_int = (Type *)malloc(sizeof(Type));
    typ_int->kind = INT;
    
    Type *typ_void = (Type *)malloc(sizeof(Type));
    typ_void->kind = VOID;

    Type *typ_func = (Type *)malloc(sizeof(Type));
    typ_func->kind = FUNCTION;

    Type *typ_array = (Type *)malloc(sizeof(Type));
    typ_array->kind = ARRAY;

    if(initSymbolTable()==0) {
      perror("  ++>Init");
    }

    symInsert("a", typ_int, 1);
    symInsert("b", typ_int, 2);
    symInsert("main", typ_func, 3);
    

    enterScope();//entering main

      symInsert("c", typ_int, 4);
      symInsert("d", typ_int, 5);

      enterScope();//entering inner 1

        symInsert("a", typ_int, 5);
        symInsert("c", typ_int, 6);

        enterScope();//entering inner 2

          symInsert("b", typ_int, 7);

          enterScope();//entering inner 3

            symInsert("f", typ_int, 8);
            printSymbolTable();
          leaveScope();//exiting inner 3

          ElementPtr s;
          s = symLookup("f");
          printElement(s);

        leaveScope();//exiting inner 2

      leaveScope();//exiting inner 1

    leaveScope();//exiting main


    footer();
}

