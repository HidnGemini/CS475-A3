#ifndef LS2_H_INCLUDED
#define LS2_H_INCLUDED

#include "stack.h"

#define TRUE 1
#define FALSE 0
#define INDENT "    "

// TODO: Any global variables go below
//int thisIsGlobal1 = 10;  // delete before submission

void indent(int); // indents n times based on input
void printFullDir(char*); // calls recursivePrintFullDir with depth 0
void recursivePrintFullDir(char*, int); // recursively prints all files in directory
void printMatchingFiles(char*, char*);
int recursivePrintMatchingFiles(char*, int, char*, stack_t*);

#endif
