//printTitle() was given by the textbook, it prints "A Simple Shell" in an ASCII stylized format
void printTitle() {
    printf("              ___       _____ _                 __        _____ __         ____\n");
    printf("             /   |     / ___/(_)___ ___  ____  / /__     / ___// /_  ___  / / /\n");
    printf("            / /| |     \\__ \\/ / __ `__ \\/ __ \\/ / _ \\    \\__ \\/ __ \\/ _ \\/ / /\n");
    printf("           / ___ |    ___/ / / / / / / / /_/ / /  __/   ___/ / / / /  __/ / /\n");
    printf("          /_/  |_|   /____/_/_/ /_/ /_/ .___/_/\\___/   /____/_/ /_/\\___/_/_/\n");
}

/*
printPrompt() gives the user a prompt for input. The function declares two variables to store machine name and directory.
Using built in C functions, the function stores the relevant information in the variables and then prints the prompt
*/
void printPrompt() {
    char machineName[1024]; // char array to store the name of the machine
    char currentDirectory[1024]; // char array to store the current directory

    gethostname(machineName, sizeof(machineName));
    getcwd(currentDirectory, sizeof(currentDirectory));
    printf("%s:%s>", machineName, currentDirectory);
}

/*
readCommand() uses the built in C function fgets() to read input from the user given by the "buffer" parameter. The arguments passed through fgets()
ensure the user does not go over a specified length.
*/
void readCommand(char *buffer) {
    fgets(buffer, MAX_ARG_LEN, stdin);
}

/*
parseCommand() determines command name and constructs the parameter list. This function will build argv[] and set the argc value.
argc is the number of "tokens" or words on the command line. argv[] is an array of strings (pointers to char *).
The last element in argv[] much be NULL. As we scan the command line from the left, the first token goes in argv[0], 
the second in arg[1], and so on. Each time we add a token to argv[], we increment argc.
*/
int parseCommand(char *cLine, struct command_t *cmd) {
    int argc; // variable to store num of tokens from user input
    char **clPtr;

    //Initialization
    clPtr = &cLine; //cLine is command line
    argc = 0;
    cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
    
    //Fill argv[]
    while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
        cmd->argv[++argc] = (char*)malloc(MAX_ARG_LEN);
    }

    //Set the command name and argc
    cmd->argc = argc - 1;
    cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
    strcpy(cmd->name, cmd->argv[0]);
    return 1;
}

/*
parsePath() reads from path variable for this envirornment and builds an array, dirs[], of the directories in path.
This functions ensures that lookupPath() can verify whether a command existes. Returns the number of properly parsed directories.
*/
int parsePath(char* dirs[]) {  
    char *pathEnvVar; // variable to store paths
    char *thePath; // variable to store information from pathEnvVar so that it can be parsed

    for (int i = 0; i < MAX_ARGS; i++) // initialize all directories to be NULL
        dirs[i] = NULL;

    pathEnvVar = (char *) getenv("PATH");
    thePath = (char *) malloc(strlen(pathEnvVar) + 1);
    strcpy(thePath, pathEnvVar);

    //Loop to parse thePath. Look for a ':' delimiter between each path name.
    char *parse = strtok(thePath, ":");
    int parsedString = 0;

    // Parsing every directory using strtok
    while (parse != NULL) {
        dirs[parsedString] = malloc(strlen(parse) + 1); // store the parsed paths in dirs[]
        strcpy(dirs[parsedString], parse);
        parsedString++; // increment number of parsed directories
        parse = strtok(NULL, ":"); // parse next directory
    }

    return parsedString; // return number of parsed directories

}

/*
lookupPath() searches the directiories identified by the dir arguemnt to see if argv[0] (the file name) appears there.
Allocate a new string, place the full path name in it, then return the string.
*/
char *lookupPath(char **argv, char **dir) {
    char *result; // variable to store the path and return it
    char pName[MAX_PATH_LEN]; // temp var to update result

    //check to see if file name is already an absolute path name
    if (*argv[0] == '/') {
        strcpy(pName, argv[0]);
        if (access(pName, F_OK) == 0) {
            result = pName;
            return result;
        }
    }

    //Look in PATH directories.
    //Use access() to see if the file is in a dir.
    for (int i = 0; i < MAX_PATHS; i++) {
        if (dir[i] == NULL) {break;}

        // concatenate directory with command
        strcpy(pName, dir[i]);
        strcat(strcat(pName, "/"), argv[0]);

        if (access(pName, F_OK) == 0) { // check if directory has command
            result = pName;
            return result;
        }
    }

    //Filename not found in any path variable
    fprintf(stderr, "%s: command not found\n", argv[0]);
    return NULL;
}

char *strsep(char **stringp, const char *delim) {
    char* rv = *stringp;
    if (rv) {
        *stringp += strcspn(*stringp, delim);
        if (**stringp)
            *(*stringp)++ = '\0';
        else
            *stringp = 0;
    }
    return rv;
}