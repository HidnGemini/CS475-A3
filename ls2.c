#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"


/** 
 * indent(n) prints an indent n times. It is only used
 * as a helper for recursivePrintFullDir.
 *
 * @param times is the number of times to indent
 */
void indent(int times) {
    for (int i = 0; i < times; i++) {
        printf("%s", INDENT);
    } 
}

/** 
 * printFullDir(PATH) calls the recursive function 
 * recursivePrintFullDir(PATH, 0). This function exists
 * purely to make the inputs more intuitive.
 *
 * @param inputPath is a string of the path to print 
 * the contents of
 */
void printFullDir(char* inputPath) {
    recursivePrintFullDir(inputPath, 0);
}

/** 
 * recursivePrintDir(PATH, depth) traverses the given PATH,
 * printing every file and its size, and recursing on any
 * directories.
 *
 * @param inputPath is a string of the path to print
 * the contents of
 * @param numIndents is an int representing how deep
 * into recursion we are
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
 * printMatchingFiles creates a stack, calls recursive
 * version of the function, and prints the stack. This
 * function only exists to simplify the calling of
 * recursivePrintMatchingFiles.
 * 
 * @param inputPath is a string of the path to print
 * the contents of
 * @param toMatch is the string name of a file to 
 * search for
 */
void printMatchingFiles(char* inputPath, char* toMatch) {
    stack_t *printStack = initstack();
    recursivePrintMatchingFiles(inputPath, 0, toMatch, printStack);
    printstack(printStack);
    freestack(printStack);
}

/** 
 * recursivePrintMatchingFiles(PATH, FILENAME) traverses
 * the given PATH, printing all folders that contain a 
 * file named FILENAME and the files. It is not supposed 
 * to be called without the help of printMatchingFiles.
 *
 * @param inputPath is a string of the path to print
 * the contents of
 * @param numIndents is an int representing how deep
 * into recursion we are
 * @param toMatch is the string name of a file to 
 * search for
 * @param stack is a stack that will contain string
 * representations of all matching files.
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

                if (S_ISDIR(statBuf->st_mode)) {
                    wasFileFound = recursivePrintMatchingFiles(path, numIndents+1, toMatch, stack);

                    if (wasFileFound) {
                        // setup path for pushing to stack
                        char *stringForStack = (char*) malloc(sizeof(directory->d_name) + 4*sizeof(INDENT) + 14);
                        stringForStack[0] = '\0';
                        for (int i = 0; i<numIndents; i++) {
                            strcat(stringForStack, INDENT);
                        }
                        strcat(stringForStack, directory->d_name);
                        strcat(stringForStack, "/ (directory)");

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