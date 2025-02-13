#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "stack.h"
#include "ls2.h"

/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 */
int main(int argc, char* argv[]) {
	if (argc == 1) {
		printf("Usage: ./ls2 <path> [exact-match-pattern]\n");
	} else if (argc == 2) {
		// Mode 1
		printFullDir(*(argv+1));
	} else if (argc == 3) {
		// Mode 2
		printMatchingFiles(*(argv+1), *(argv+2));
	} else {
		printf("Usage: ./ls2 <path> [exact-match-pattern]\n");
	}

	// // stack stores the lines to print out
	// stack_t *s = initstack();

	// push(s, "Hello1");
	// push(s, "Hello2");
	// push(s, "Hello3");

	// print stack
	// printf("Printing stack now!\n");
	// printstack(s);

	// // free up stack
	// freestack(s);
	return 0;
}
