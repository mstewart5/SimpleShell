/*
Authors: Michael Stewart, Andrew Smith, Drake Richard
Course: COSC 4302, Operating Systems
Assignment: Linux Project Shell
Instructor: Bo Sun 

This program was written to create a shell that operates similarly to the linux terminal. This was achieved through writing functions to read user input,
parse the input, and then check linux directories to see if that command exists or not. If the command exists, a child process is made and execv() is called within that 
process to execute the command. This shell can run commands such as "ls, man, ps, mkdir, etc...". As per the project requirements, this code was written in C.

As of 4/27/23, there are no outstanding bugs that should be noted.
 */

// These headers hold code that is vital to the program functioning properly
// Definitions.h includes several libraries, definitions, and function prototypes to avoid writing
// redundant code.
// Functions.h holds all the functions for formatting the terminal, building an array of directories that hold linux commands,
// reading commands, and parsing commands from user input.
#include <unistd.h>
#include "definitions.h"
#include "functions.h"

#define _CRT_SECURE_NO_WARNINGS


/*
int main() is responsible for executing the program.
The method initializes important variables, gives the user an input prompt, and then calls several functions
to read, parse, and execute the command given by the user.
*/
int main(int argc, char *argv[])
{
    // Initializing all the variables needed for the shell to function properly
    int pid; // variable to check if fork() was successful or not
    char commandLine[MAX_LINE_LEN]; // stores the user's input
    char *pathv[MAX_PATHS]; // stores the directories holding executable commands
    struct command_t command; // struct that contains variables vital to parsing the command

    printTitle();

    parsePath(pathv); // Get directory paths from PATH

    while (1) {
        printPrompt();

        //Read the command line and parse it
        readCommand(commandLine);
        parseCommand(commandLine, &command);

        //exit program when user inputs exit
        if (strcmp(commandLine, "exit") == 0) {
            exit(0);
        }

        //Get the full path name for the file
        command.argv[command.argc] = NULL;
        command.name = lookupPath(command.argv, pathv);

        //Create child and execute the command
        if (pid = fork() == 0) {
            execv(command.name, command.argv);
        }
        //Wait for the child to terminate
        wait(NULL);
    
    }
        // Shell will terminate upon reading "exit" as input from the user.
}
