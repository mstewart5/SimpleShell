#include <unistd.h>
#include "definitions.h"
#include "functions.h"
#include <pthread.h>

#define _CRT_SECURE_NO_WARNINGS
/*
It should be noted that this program will only ever be able to run one shell command.
execv() inherently terminates the process it was called in after a successfull call. 
No matter how many threads there are, because the threads are contained within one process,
execv() will still terminate the process which explains why only one shell command can ever be ran. 

I will also note that the program may take a few runs to actually get a successful command execution.
If execv() fails, ctrl+c out of the program and then run it again. Repeat this until the command works.
I am not sure why it does this, but I am inclined to believe there is some mismanaged memory causing the issue.
- Michael S.
*/
int main() {
    // Shell initialization
    int i;
    int pid;
    char commandLine[MAX_LINE_LEN];
    char *pathv[MAX_PATHS];
    struct command_t command;
    pthread_t cmdExecThread;
    parsePath(pathv); // get directory paths from PATH

    system("clear");
    while(1) {
        printPrompt();
    
        // Read from command line and parse

        readCommand(commandLine);
        parseCommand(commandLine, &command);

        // create thread to execute command
        // we wrap execv() in the function execWrapper due to pthreads only being able to take
        // one argument for a function
        pthread_create(&cmdExecThread, NULL, execWrapper, &command);

        command.argv[command.argc] = NULL; // dont know what this does, but the code doesn't work without it
        // get full pathnames
        command.name = lookupPath(command.argv, pathv);
     
        // await thread termination
        pthread_join(cmdExecThread, NULL);
        
    }

}
