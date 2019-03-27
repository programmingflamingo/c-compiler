

#include "symbolTable.h"

// Top should point to the top of the scope stack,
// which is the most recent scope pushed

SymbolTableStackEntryPtr symbolStackTop;

int scopeDepth;

static int hash (char *name) {

  int i, sum;
  sum = 0; 
  for(i=0; i<strlen(name); i++) {
    sum = sum + name[i];
  }

  return sum%MAXHASHSIZE;
}

/* global function prototypes */

//allocate the global scope entry and symbol table --and set scopeDepth to 0
// The global scope remains till the end of the program
// return 1 on success, 0 on failure (such as a failed malloc)
int		initSymbolTable() 
{
  //printf("initSymbolTable start\n");
  scopeDepth = 0;

  symbolStackTop = (struct symbolTableStackEntry*) malloc(sizeof(struct symbolTableStackEntry));

  symbolStackTop->symbolTablePtr = (struct symbolTable *) malloc(sizeof(struct symbolTable));
  symbolStackTop->prevScope = NULL;


  int i;
  for(i=0;i<MAXHASHSIZE;i++) {
    symbolStackTop->symbolTablePtr->hashTable[i] = (struct element *) malloc(sizeof(struct element));

    symbolStackTop->symbolTablePtr->hashTable[i]->key = -1;
    symbolStackTop->symbolTablePtr->hashTable[i]->id = (char *) malloc(512*sizeof(char));;
    symbolStackTop->symbolTablePtr->hashTable[i]->linenumber = -1;
    symbolStackTop->symbolTablePtr->hashTable[i]->scope = scopeDepth;
    symbolStackTop->symbolTablePtr->hashTable[i]->stype = NULL;
    symbolStackTop->symbolTablePtr->hashTable[i]->ast = NULL;
    symbolStackTop->symbolTablePtr->hashTable[i]->next = NULL;
  }

  if(symbolStackTop == NULL) {
    return 0;
  }

  return 1;
}


// Look up a given entry 
ElementPtr		symLookup(char *name)
{
  //printf("symLookup start\n");
  int i;
  struct symbolTableStackEntry *myNode;

  for(myNode = symbolStackTop; myNode != NULL; myNode = myNode->prevScope) {
    for(i=0;i<MAXHASHSIZE;i++) {
      ElementPtr curr = myNode->symbolTablePtr->hashTable[i];

      while(curr != NULL) {
        if(strcmp(curr->id, name)==0) {
          return curr;
        }
        curr = curr->next;
      }
    }
  }
  return NULL;
}


// Insert an element with a specified type in a particular line number
// initialize the scope depth of the entry from the global var scopeDepth
ElementPtr		symInsert(char *name, struct type *type, int line)
{
  //printf("symInsert start\n");
  ElementPtr sym = (struct element	*) malloc(sizeof(struct element));
  sym->key = hash(name);
  sym->id = name;
  sym->linenumber = line;
  sym->scope = scopeDepth;
  sym->stype = type;
  sym->ast = NULL;
  sym->next = NULL;

  if(symbolStackTop->symbolTablePtr->hashTable[sym->key]->key == -1)
    symbolStackTop->symbolTablePtr->hashTable[sym->key] = sym;
  
  else 
    symbolStackTop->symbolTablePtr->hashTable[sym->key]->next = sym;

  return sym;
}


//push a new entry to the symbol stack
// This should modify the variable top and change the scope depth
// return 1 on success, 0 on failure (such as a failed malloc)
int			enterScope()
{
  //printf("enterScope start\n");
  scopeDepth += 1;

  struct symbolTableStackEntry* myNode;
  myNode = (struct symbolTableStackEntry*) malloc(sizeof(struct symbolTableStackEntry));


  myNode->symbolTablePtr = (struct symbolTable *) malloc(sizeof(struct symbolTable));
  myNode->prevScope = symbolStackTop;

  int i;
  for(i=0;i<MAXHASHSIZE;i++) {
    myNode->symbolTablePtr->hashTable[i] = (struct element *) malloc(sizeof(struct element));

    myNode->symbolTablePtr->hashTable[i]->key = -1;
    myNode->symbolTablePtr->hashTable[i]->id = (char *) malloc(512*sizeof(char));;
    myNode->symbolTablePtr->hashTable[i]->linenumber = -1;
    myNode->symbolTablePtr->hashTable[i]->scope = scopeDepth;
    myNode->symbolTablePtr->hashTable[i]->stype = NULL;
    myNode->symbolTablePtr->hashTable[i]->ast = NULL;
    myNode->symbolTablePtr->hashTable[i]->next = NULL;
  }

  symbolStackTop = myNode;


  if(myNode == NULL) {
   return 0;
  }

  return 1;
}


//pop an entry off the symbol stack
// This should modify top and change scope depth
void			leaveScope()
{
  //printf("leaveScope start\n");
  if(scopeDepth != 0) {
    scopeDepth -= 1;

    //struct symbolTableStackEntry* myNode = symbolStackTop;    
    //ElementPtr  = symbolStackTop->hashtable; <- not necessary
    symbolStackTop = symbolStackTop->prevScope;
    //free(myNode);
 
  }

  return;
}




// Do not modify this function
void printElement(ElementPtr symelement) {
    if (symelement != NULL) {
        printf("Line %d: %s", symelement->linenumber,symelement->id);
    }
    else printf("Wrong call! symbol table entry NULL");
  return;
}

//should traverse through the entire symbol table and print it
// must use the printElement function given above
void printSymbolTable()
{
  //printf("printSymbolTable start\n");
  struct symbolTableStackEntry *myNode;

  printf("\nLocalized Scope\n");
  int count;
  count = scopeDepth;
  for(myNode = symbolStackTop; myNode != NULL; myNode = myNode->prevScope) {
    int i;
    printf("  Scope Depth {%d}\n  +-----------------------+\n",count);
    for(i=0;i<MAXHASHSIZE;i++) {
      ElementPtr curr = myNode->symbolTablePtr->hashTable[i];

      while(curr != NULL && curr->linenumber!=-1) {
        printf("  | ");
        printElement(curr);
        printf("\n");
        curr = curr->next;
      }
    }
    printf("  +-----------------------+\n");
    if(count>0)
      printf("\n");
    count -= 1;

  }
  printf("Globalized Scope\n\n");
  return;
}




            


    





