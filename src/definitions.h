#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

#define _CRT_SECURE_NO_WARNINGS
#define MAX_LINE_LEN  80
#define MAX_ARGS      64
#define MAX_ARG_LEN   16
#define MAX_PATHS     64
#define MAX_PATH_LEN  96
#define WHITESPACE    " .,\t\n"

struct command_t {
    char* name;
    int argc;
    char* argv[MAX_ARGS];
};

void printTitle();
void printPrompt();
void readCommand();
int parseCommand(char* cLine, struct command_t* cmd);
int parsePath(char* dirs[]);
char* lookupPath(char** argv, char** dir);
char* strsep(char** stringp, const char* delim);
