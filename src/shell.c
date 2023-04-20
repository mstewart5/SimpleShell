#include <unistd.h>
#include "definitions.h"
#include "functions.h"

#define _CRT_SECURE_NO_WARNINGS


//NOTE: From the book, this is the intital codebehaves like a shell program.You
//can use this code as part of your solution to the UNIX shell Laboratory Exercise at the end
//of the chapter.This program reads a list of commands from a file, then executes each command just as a shell would execute it.Each command line in the file will be the same as
//a line you would type to the shell
//Just implementing this for ground work, and then will go back and add what's needed...

int main(int argc, char *argv[])
{
    // ...
    //Shell initialization
    // Initializing all the variables needed for the shell to actually work
    int i;
    int pid;
    char commandLine[MAX_LINE_LEN];
    char *pathv[MAX_PATHS];
    struct command_t command;
    printTitle();

    parsePath(pathv); // Get directory paths from PATH

    while (1) {
        printPrompt();

        //Read the command line and parse it
        readCommand(commandLine);
        parseCommand(commandLine, &command);

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
        // figure out how to get the shell to terminate eventually - Michael S.

/* // I'm commenting this out because this runs commands from a file, not from the linux directories. Should probably just outright get rid of this code - Michael S.
    int i;
    int pid, numChildren;
    int status;
    FILE* fid;
    char cmdLine[MAX_LINE_LEN];
    struct command_t command;

    //Read the command line parameters
    if (argc != 2) {
        fprintf(stderr, "Usage: launch <launch_set_filename>\n");
        exit(0);
    }

    //Open a file that conatins a set of commands
    fid = fopen(argv[1], "r");

    //Process each command in the launch file
    numChildren = 0;
    while (fgets(cmdLine, MAX_LINE_LEN, fid) != NULL) {
        parseCommand(cmdLine, &command);
        command.argv[command.argc] = NULL;

        //Create a child process to execute the command
        if ((pid = fork()) == 0) {

            //Child executing command
            execvp(command.name, command.argv);
        }

        //Parent continuing to the next command in the file
        numChildren++;
    }

    printf("\n\nlaunch: Launched %d commands\n", numChildren);

    //Terminate after all childern have terminated
    for (int i = 0; i < numChildren; i++) {
        wait(&status);
        //Should free dynamic storage in command data structure
    }

    printf("\n\nlaunch: Terminating successfully\n");
    return 0;
*/
}
