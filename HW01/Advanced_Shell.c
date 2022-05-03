#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define clear() printf("\033[H\033[J")

#define CMDLEN 100
#define NOTBCMDS 5
#define ARGCNT 4
#define SPACE " "

int regExecute(char** parsed);
int parse(char* cmd, char** parsed);
int execute(char* b_path, char** args);
int notBinCommands(char* n_path, char** parsed);


// main function.
int main() {
    // declaring variables
    int index = 0;
    char b_path[CMDLEN] = "/bin/";
    char n_path[CMDLEN] = "/home/braude/Desktop/";
    char cmd[CMDLEN];
    char* parsed[ARGCNT];

    // clear screen
    clear();
    do {
        // ask for input
        printf("AdvShell> ");
        fgets(cmd, 100, stdin);
        cmd[strlen(cmd) - 1] = '\0';
        if ( !strcmp("Exit", cmd)) {
            // if exit we print godbye 
            printf("Goodbye...\n");
            exit(1);
        }
        // "reseting" the paths to the command directories.
        strcpy(b_path, "/bin/");
        strcpy(n_path, "/home/braude/");

        // if we got an empty string we do nothing.
        if(!strcmp(cmd,""))
            continue;

        // getting the last index of the parsed array and nulling it.
        index = parse(cmd, parsed);
        parsed[index] = NULL;

        // if the command is "cd" we use chdir to navigate through the file system.
        if (!strcmp(parsed[0], "cd")) {
            chdir(parsed[1]);
            continue;
        }

        // executing commands that start with "./" .
        if (regExecute(parsed))
            continue;
        
        // executing our custom commands.
        if (notBinCommands(n_path, parsed))
            continue;

        // executing "regular" commands from /bin.
        if (execute(b_path, parsed))
            continue;

    } while (1);
}

// parser function
int parse(char* cmd, char** parsed) {
    // declare variables.
    int i = 0, j = 0, index = 0;
    char temp[CMDLEN];
    // looping through the input and collecting words and if 
    // we encounter a " " we "dump" the collected word into the string array.
    // its kind of an analog to strtok and strsep.
    for (i; i < strlen(cmd) + 1; i++) {
        if (cmd[i] == ' ' || cmd[i] == '\0') {
            temp[j] = '\0';
            parsed[index] = malloc(sizeof(char) * strlen(temp));
            strcpy(parsed[index], temp);
            index += 1;
            j = 0;
        }
        else if (index == ARGCNT + 1 || cmd[i] == '\0') {
            return index;
        }
        else {
            temp[j] = cmd[i];
            j += 1;
        }
    }
    return index;
}

// executing commands that start with "./"
int regExecute(char** parsed) {
    // declaring variables
    int i = 2;
    char prog[CMDLEN];
    // check if starts with"./", if not return 0;
    if (parsed[0][0] == '.' && parsed[0][1] == '/') {
        // if it is then we remove the "./" and run the command
        for (i; i < strlen(parsed[0]) + 1; i++) {
            prog[i - 2] = parsed[0][i];
        }
        prog[i - 2] = '\0';

        pid_t pid = fork();

        if (pid == -1) {
            perror("Forking child");
            return;
        }
        else if (pid == 0) {
            if (execl(prog, 0) < 0) {
                printf("NOT SUPPORTED\n");
            }
            exit(0);
        }
        else {
            // waiting for child to terminate
            wait(NULL);
            return 1;
        }
    }
    return 0;
}

// executing our custom commands.
int notBinCommands(char* n_path, char** parsed) {
    // declare and initialize variables.
    int i = 0;
    char* notBcmd[NOTBCMDS];
    notBcmd[0] = "CreateMenu";
    notBcmd[1] = "getMenu";
    notBcmd[2] = "MakeOrder";
    notBcmd[3] = "getPrice";
    notBcmd[4] = "getOrderNum";

    // searching a command
    for (i; i < NOTBCMDS; i++) {
        // if found we execute.
        if (!strcmp(parsed[0], notBcmd[i])) {
            strcat(n_path, parsed[0]);
            
            pid_t pid = fork();

            if (pid == -1) {
                perror("Forking child");
                return;
            }
            else if (pid == 0) {
                if (execvp(n_path, parsed) < 0) {
                    printf("NOT SUPPORTED\n");
                }
                exit(0);
            }
            else {
                // waiting for child to terminate
                wait(NULL);
                return 1;
            }
            
            return 1;
        }
    }
    // else we return 0
    return 0;
}

// executing the "regular binary commands".
int execute(char* b_path, char** args) {
    // concatenating the path and the program name.
    strcat(b_path, args[0]);

    // executing the command
    pid_t pid = fork();

    if (pid == -1) {
        perror("Forking child");
        return 0;
    }
    else if (pid == 0) {
        if (execvp(b_path, args) < 0) {
            printf("NOT SUPPORTED\n");
        }
        exit(0);
    }
    else {
        // waiting for child to terminate
        wait(NULL);
        return 1;
    }
    return 0;
}








// BACKUP FROM 20/03/2022 //
/*
//void parse(char* str, char** parsed[]);
//void clean(char* parsed[]);
void execute(char* path, char** args[]); // <-- maybe leave this one. also add c commands.

int main() {
    int i = 0, index = 0;
    char b_path[CMDLEN] = "/bin/";
    char c_path[CMDLEN] = ""; // <------ add path to non binary commands.
    char cmd[CMDLEN];
    char args[ARGCNT][CMDLEN];
    char* temp;

    clear();
    do {
        printf("AdvShell> ");
        gets(cmd);
        if (!strcmp("Exit", cmd)) {
            break;
        }

        temp = strtok(cmd, SPACE);
        strcpy(args[i], temp);
        i += 1;
        index += 1;
        for (i; i < ARGCNT; i++) {
            temp = strtok(NULL, SPACE);
            if (temp == NULL) {
                break;
            }
            strcpy(args[i], temp);
            index += 1;
        }
        i = 0;

        

        pid_t pid = fork();

        if (pid == -1) {
            printf("\nFailed forking child", stderr);
            return;
        }
        else if (pid == 0) {
            if (index == 1) {
                if (execl(b_path, args[0]) < 0) {
                    printf("\nCould not execute command", stderr);
                }
                exit(0);
            }
            else if (index == 2) {
                if (execl(b_path, args[0], args[1]) < 0) {
                    printf("\nCould not execute command", stderr);
                }
                exit(0);
            }
            else if (index == 3) {
                if (execl(b_path, args[0], args[1], args[2]) < 0) {
                    printf("\nCould not execute command", stderr);
                }
                exit(0);
            }
            else if (index == 4) {
                if (execl(b_path, args[0], args[1], args[2], args[3]) < 0) {
                    printf("\nCould not execute command", stderr);
                }
                exit(0);
            }
        }
        else {
            // waiting for child to terminate
            wait(NULL);
            return;
        }
        
index = 0;

strcpy(b_path, "/bin/");

//clean(args); 

//execute(b_path, parsed); // <--- add ppath concatenation before function.

    } while (1);
}
*/



// BACKUP MOSTLY WORKING SHELL // 
/*
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>

///#include <unistd.h>
#define clear() printf("\033[H\033[J")

#define CMDLEN 100
#define ARGCNT 4
#define SPACE " "

int parse(char* cmd, char** parsed);
void execute(char* path, char** args); // <-- maybe leave this one. also add c commands.
//void clean(char* parsed[]);

int main() {
    int index = 0;
    char b_path[CMDLEN] = "/bin/";
    char c_path[CMDLEN] = ""; // <------ add path to non binary commands.
    char cmd[CMDLEN];
    char* parsed[ARGCNT];

    clear();
    do {
        printf("AdvShell> ");
        gets(cmd);
        if (!strcmp("Exit", cmd)) {
            break;
        }

        index = parse(cmd, parsed);
        parsed[index] = NULL;

        execute(b_path, parsed); // <--- add c_path concatenation before function.
        strcpy(b_path, "/bin/");

    } while (1);
}

int parse(char* cmd, char** parsed) {
    int i = 0, j = 0, index = 0;
    char temp[CMDLEN];
    for (i; i < strlen(cmd) + 1; i++) {
        if (cmd[i] == ' ' || cmd[i] == '\0') {
            temp[j] = '\0';
            parsed[index] = malloc(sizeof(char) * strlen(temp));
            strcpy(parsed[index], temp);
            index += 1;
            j = 0;
        }
        else if (index == ARGCNT + 1 || cmd[i] == '\0') {
            return index;
        }
        else {
            temp[j] = cmd[i];
            j += 1;
        }

    }
    return index;
}

void execute(char* path, char** args) {
    strcat(path, args[0]);

    
    pid_t pid = fork();

    if (pid == -1) {
        printf("\nFailed forking child", stderr);
        return;
    }
    else if (pid == 0) {
        if (execvp(path, args) < 0) {
            printf("\nCould not execute command", stderr);
        }
        exit(0);
    }
    else {
        // waiting for child to terminate
        wait(NULL);
        return;
    }
    
}
*/
