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
	DIR *dir;
	struct dirent *directory;
	struct stat *statBuf;
	stack_t *s = initstack();


	if (argc == 1) {
		printf("Usage: ./ls2 <path> [exact-match-pattern]");
	} else if (argc == 2) {
		// Mode 1

		char *path = (char*) malloc(sizeof(*(argv+1)));
		strcat(path, *(argv+1)); // put current path in path variable

		// put path into a different location and push to stack (we will edit path later)
		char *parentPath = (char*) malloc(sizeof(path));
		strcpy(parentPath, path);
		push(s, parentPath);

		dir = opendir(path);
		free(path); // path will be reallocated later

		// printf("%s\n", path);
		// path = (char*) realloc(path, sizeof("extra letters"));
		// strcat(path, "extra letters");
		// printf("%s\n", path);

		if (dir) {
			while ((directory = readdir(dir)) != NULL) {
				if (!((strcmp(directory->d_name, ".") == 0) || (strcmp(directory->d_name, "..") == 0))) { // skips . and .. directories
					// extend path for file
					path = (char*) malloc(sizeof(parentPath) + sizeof(directory->d_name) + 1);
					strcat(path, parentPath);
					strcat(path, "/");
					strcat(path, directory->d_name);

					// get path info
					statBuf = malloc(sizeof(struct stat));
					stat(path, statBuf);

					if (S_ISDIR(statBuf->st_mode)) {
						printf("%s\n", path); // print directory name (#TODO: PATH RIGHT NOW, FIX THAT)
					} else {
						printf("%s (%ld bytes)\n", path, statBuf->st_size); // print file immediately
					}

				}
            }
            closedir(dir);
		}
	
	} else if (argc == 3) {
		// Mode 2
	} else {
		printf("Usage: ./ls2 <path> [exact-match-pattern]");
	}

	// // stack stores the lines to print out
	// stack_t *s = initstack();

	// push(s, "Hello1");
	// push(s, "Hello2");
	// push(s, "Hello3");

	// print stack
	printf("Printing stack now!\n");
	printstack(s);

	// free up stack
	freestack(s);
	return 0;
}
