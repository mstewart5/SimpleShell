/*
Authors: Michael Stewart, Andrew Smith, Drake Richard
Course: COSC 4302, Operating Systems
Assignment: Linux Project Shell
Instructor: Bo Sun 

This program was written to create a shell that operates similarly to the linux terminal. This was achieved through writing functions to read user input,
parse the input, and then check linux directories to see if that command exists or not. If the command exists, a new thread is createde and a 
function designed to wrap execv() is called within that thread to execute the command. 
This shell can run commands such as "ls, man, ps, mkdir, etc...". As per the project requirements, this code was written in C.

It should be noted that this program will only ever be able to run one shell command.
execv() inherently terminates the process it was called in after a successfull call. 
No matter how many threads there are, because the threads are contained within one process,
execv() will still terminate the process which explains why only one shell command can ever be ran. 

*/

// These headers hold code that is vital to the program functioning properly
// Definitions.h includes several libraries, definitions, and function prototypes to avoid writing
// redundant code.
// Functions.h holds all the functions for formatting the terminal, building an array of directories that hold linux commands,
// reading commands, and parsing commands from user input.
#include <unistd.h>
#include "definitions.h"
#include "functions.h"
#include <pthread.h>

struct command_t command; // struct that contains multiple variables vital to parsing the command
void *execWrapper(void *arg); // prototype for function exclusive to multithread version of shell

#define _CRT_SECURE_NO_WARNINGS

/*
int main() is responsible for executing the program.
The method initializes important variables, gives the user an input prompt, and then calls several functions
to read, parse, and execute the command given by the user.
*/
int main() {
    // Shell initialization
    int pid; // variable to check if fork() was successful or not
    char commandLine[MAX_LINE_LEN]; // stores the user's input
    char *pathv[MAX_PATHS]; // stores the directories holding executable commands
    
    pthread_t cmdExecThread; // initialize a thread data type for executing the command

    parsePath(pathv); // get directory paths from PATH

    while(1) {
        printPrompt();
    
        // Read from command line and parse

        readCommand(commandLine);
        parseCommand(commandLine, &command);

        // Create thread to execute command
        // Wrap execv() in the function execWrapper due to pthreads only being able to take
        // one argument for a function
        pthread_create(&cmdExecThread, NULL, execWrapper, &command);

        // Get full pathnames
        command.argv[command.argc] = NULL;
        command.name = lookupPath(command.argv, pathv);
     
        // Await thread termination
        pthread_join(cmdExecThread, NULL);
        
    }

}

/*
execWrapper() exclusively for the multithreaded version of the shell.
Because pthreads can only take one argument for a function it is necessary to 
wrap execv() so that it can work given only one argument.
The argument in question being the commant_t struct.
*/
void *execWrapper(void *arg) {
    execv(command.name, command.argv);
    printf("execv failed\n"); // this line will only ever run in the event execv() doesn't work.
}
