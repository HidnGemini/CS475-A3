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
	return 0;
}
