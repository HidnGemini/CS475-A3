#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"


/** 
 * 
 *
 */
void indent(int times) {
    for (int i = 0; i < times; i++) {
        printf("%s", INDENT);
    } 
}

void printFullDir(char* inputPath) {
    recursivePrintFullDir(inputPath, 0);
}

/** 
 * 
 *
 */
void recursivePrintFullDir(char* inputPath, int numIndents) {
    // allocate things we will need later
    DIR *dir;
	struct dirent *directory;
	struct stat *statBuf;

    // put path into a different location and push to stack (we will edit path later)
    dir = opendir(inputPath); // PROBLEM HERE

    if (dir) {
        while ((directory = readdir(dir)) != NULL) {
            if (!((strcmp(directory->d_name, ".") == 0) || (strcmp(directory->d_name, "..") == 0))) { // skips . and .. directories

                // extend path for file
                char *path = (char*) malloc(strlen(inputPath) + strlen(directory->d_name) + 2);
                path[0] = '\0';

                // build directory string
                strcat(path, inputPath);
                strcat(path, "/");
                strcat(path, directory->d_name);

                // get path info
                statBuf = malloc(sizeof(struct stat));
                stat(path, statBuf);

                if (S_ISDIR(statBuf->st_mode)) {
                    indent(numIndents);
                    printf("%s/ (directory)\n", directory->d_name); // print directory name
                    recursivePrintFullDir(path, numIndents+1);
                } else {
                    indent(numIndents);
                    printf("%s (%ld bytes)\n", directory->d_name, statBuf->st_size);
                }
                free(statBuf);
                free(path);
            } 
        }
    }
    closedir(dir);
    free(directory);
}

/** 
 * 
 *
 */
void printMatchingFiles(char* inputPath, char* toMatch) {
    stack_t *printStack = initstack();
    recursivePrintMatchingFiles(inputPath, 0, toMatch, printStack);
    printstack(printStack);
    freestack(printStack);
}

/** 
 * 
 *
 */
int recursivePrintMatchingFiles(char* inputPath, int numIndents, char* toMatch, stack_t* stack) {
    int wasFileFound = 0; // initialize as not found

    // allocate things we will need later
    DIR *dir;
    struct dirent *directory;
    struct stat *statBuf;

    dir = opendir(inputPath);

    if (dir) {
        while ((directory = readdir(dir)) != NULL) {
            if (!((strcmp(directory->d_name, ".") == 0) || (strcmp(directory->d_name, "..") == 0))) { // skips . and .. directories

                // extend path for file
                char *path = (char*) malloc(sizeof(inputPath) + sizeof(directory->d_name) + 2);
                path[0] = '\0';

                // build directory string
                strcat(path, inputPath);
                strcat(path, "/");
                strcat(path, directory->d_name);

                // get path info
                statBuf = malloc(sizeof(struct stat));
                stat(path, statBuf);

                // set up path for pushing to stack

                if (S_ISDIR(statBuf->st_mode)) {
                    wasFileFound = recursivePrintMatchingFiles(path, numIndents+1, toMatch, stack);

                    if (wasFileFound) {
                        // setup path for pushing to stack
                        char *stringForStack = (char*) malloc(sizeof(directory->d_name) + 4*sizeof(INDENT) + 13);
                        stringForStack[0] = '\0';
                        for (int i = 0; i<numIndents; i++) {
                            strcat(stringForStack, INDENT);
                        }
                        strcat(stringForStack, directory->d_name);
                        strcat(stringForStack, " (directory)");

                        // push string to stack
                        push(stack, stringForStack);
                    }

                } else {
                    if (strcmp(directory->d_name, toMatch) == 0) {
                        wasFileFound = 1;

                        // setup path for pushing to stack
                        char *stringForStack = (char*) malloc(sizeof(directory->d_name) + 4*sizeof(INDENT) + 9);
                        stringForStack[0] = '\0';
                        for (int i = 0; i<numIndents; i++) {
                            strcat(stringForStack, INDENT);
                        }
                        strcat(stringForStack, directory->d_name);
                        char *intBuf = (char*) malloc(28); // enough for any 64-bit integer in " (%lu bytes)" format (19 digits + 10 for " ( bytes)" part)
                        sprintf(intBuf, " (%lu bytes)", statBuf->st_size);
                        strcat(stringForStack, intBuf);
                        free(intBuf);

                        // push string to stack
                        push(stack, stringForStack);
                    }
                }

                free(path);
                free(statBuf);
            }
        }
    }
    closedir(dir);
    free(directory);

    return wasFileFound;
}