void printTitle() {
    printf("              ___       _____ _                 __        _____ __         ____\n");
    printf("             /   |     / ___/(_)___ ___  ____  / /__     / ___// /_  ___  / / /\n");
    printf("            / /| |     \\__ \\/ / __ `__ \\/ __ \\/ / _ \\    \\__ \\/ __ \\/ _ \\/ / /\n");
    printf("           / ___ |    ___/ / / / / / / / /_/ / /  __/   ___/ / / / /  __/ / /\n");
    printf("          /_/  |_|   /____/_/_/ /_/ /_/ .___/_/\\___/   /____/_/ /_/\\___/_/_/\n");
}

//Build the prompt string to have the machine name, current directory, or other desired info
void printPrompt() {
    char machineName[1024];
    char currentDirectory[1024];

    gethostname(machineName, sizeof(machineName));
    getcwd(currentDirectory, sizeof(currentDirectory));
    printf("%s:%s>", machineName, currentDirectory);
}

//This code uses any set of I/O functions, such as those in the stdio library
//to read the entire command line into the buffer.
//This implementation is greatly simplified, but it does the job.
void readCommand(char *buffer) {
    fgets(buffer, MAX_ARG_LEN, stdin); // used fgets to avoid warnings every compile
}

//Determine command name and construct the parameter list. This function will build argv[] and set the argc value.
//argc is the number of "tokens" or words on the command line. argv[] is an array of strings (pointers to char *).
//The last element in argv[] much be NULL. As we scan the command line from the left, the first token goes in argv[0], 
//the second in arg[1], and so on. Each time we add a token to argv[], we increment argc.
int parseCommand(char *cLine, struct command_t *cmd) {
    int argc;
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

//this function reads the path variable for this environment, then builds an array, dirs[], of the directories in path
int parsePath(char* dirs[]) {  
    char *pathEnvVar;
    char *thePath;

    for (int i = 0; i < MAX_ARGS; i++)
        dirs[i] = NULL;
    pathEnvVar = (char *) getenv("PATH");
    thePath = (char *) malloc(strlen(pathEnvVar) + 1);
    strcpy(thePath, pathEnvVar);

    //Loop to parse thePath. Look for a ':' delimiter between each path name.
    char *parse = strtok(thePath, ":");
    int parsedString = 0;

    // parsing every directory using strtok
    while (parse != NULL) {
        dirs[parsedString] = malloc(strlen(parse) + 1); // store the parsed paths in dirs[]
        strcpy(dirs[parsedString], parse);
        parsedString++;
        parse = strtok(NULL, ":");
    }

    return parsedString; // return number of parsed directories

}

//This function searches the directiories identified by the dir arguemnt to see if argv[0] (the file name) appears there.
//Allocate a new string, place the full path name in it, then return the string.
char *lookupPath(char **argv, char **dir) {
    char *result;
    char pName[MAX_PATH_LEN];

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
        strcat(strcat(pName, "/", argv[0]));

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
