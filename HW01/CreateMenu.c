#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 256
#define FORMATLEN 10

// declaring functions
int openFile(int fd, char* fname);
int formatDish(char* dish, int file);
int printToFile(char str[], int fd);

// main function. 
int main(int argc, char* argv[]) {
    // check arguments.
    if (argc < 3) {
        perror("bad argument count");
        exit(1);
    }

    // declaring and initializing variables.
    int file, length, i = 0, cnt, dircreate;
    file = 0;
    char dish[SIZE], fname[SIZE], buff[SIZE], dirname[SIZE], ch[4], tmp[SIZE];
    strcpy(dirname, "");
    strcpy(dish, "");
    strcpy(fname, "");
    int tst;
    
    ch[1] = '.';
    ch[2] = ' ';
    ch[3] = '\0';
    
    // assembling file name.
    strcpy(fname, argv[1]);
    strcat(fname, ".txt");
    file = openFile(file, fname);

    // printing menu header.
    strcpy(buff, argv[1]);
    strcat(buff, " Menu");
    printToFile(buff, file);
    printToFile(" \n\n", file);

    // while loop for printing the menu itself.
    for (i; i < atoi(argv[2]); i++) {
        // printing the letter near each dish type and getting input{
        ch[0] = 'a' + i;

        printToFile(ch, file);
        printf("Insert dish type %c:\n", ch[0]);
        strcpy(dish, "");
        //gets(dish);
        fgets(dish, 256, stdin);
        dish[strlen(dish) - 1] = '\0';
        printToFile(dish, file);
        printToFile(" \n", file);
        // }
        cnt = 1;
        // loop for printing the dishes.
        while (strcmp("stop", dish)) {
            // asking for input.
            printf("Insert dish name %d:\n", cnt);
            fgets(dish, 256, stdin);
            dish[strlen(dish) - 1] = '\0';
            // formating the dish also check for "stop".
            if (!formatDish(dish, file)) {
                printToFile(" \n", file);
                continue;
            }
            cnt += 1;
        }
    }
    // printing bon apetit.
    printToFile("       Bon Apetit", file);

    // assembling directory name.
    strcat(dirname, argv[1]);
    strcat(dirname, "_");
    strcat(dirname, "Order");
    // creating a directory.
    dircreate = mkdir(dirname, 0777);
    printf("Successfuly created // %s.txt created, %s dir created.\n", fname, dirname);
}

// standard function to open a file.
int openFile(int fd, char* fname) {
    if ((fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1) {
        perror("Open file");
        return -1;
    }
    return fd;
}

// standard function to write to a file.
int printToFile(char str[], int fd) {
    int wbytes, rbytes;
    rbytes = strlen(str);

    if ((wbytes = write(fd, str, rbytes)) == -1) {
        perror("Write to file");
        return -1;
    }
    // check if write is successful.
    if (wbytes != rbytes) {
        perror("Bad write");
        return -1;
    }
}

// formating the dish. to something like that: "dish ....... 10NIS.
int formatDish(char* dish, int file){
    int i, cnt = 0, index;
    char num[FORMATLEN], tmp;
    if (!strcmp("stop", dish)) {
        return 0;
    }
    strcpy(num, "");
    i = strlen(dish) - 1;
    // searching for the price.
    for (i; i > 0; --i) {
        if ((48 <= dish[i] && dish[i] <= 57) || dish[i] == '\0' || dish[i] == '.') {
            num[cnt + 1] = '\0';
            num[cnt] = dish[i];
            cnt += 1;
        }
    }

    // printing the indentation.
    printToFile("  ", file);
    // cutting off the price from input.
    index = strlen(dish) - strlen(num);
    dish[index] = '\0';
    // printing dish to file.
    printToFile(dish, file);
    i = 0;
    // printing the dots.
    for (i; i < (25 - index - 6 - strlen(num)); i ++) {
        printToFile(".", file);
    }
    
    // printinf the space after dots
    printToFile(" ", file);
    
    // if price not specified we dont print it.
    if ( !strcmp(num, ""))
        printToFile("", file);
    // else we reverse the price because we read from the end of the line.
    else {
        i = (strlen(num) - 1);
        for (i; i > (strlen(num)) / 2; i--) {
            tmp = num[i];
            num[i] = num[((strlen(num) - 1) - i)];
            num[((strlen(num) - 1) - i)] = tmp;
        }
        // and print it to file.
        printToFile(num, file);
    }
    // print NIS and new line for next iteration or program end.
    printToFile("NIS", file);
    printToFile(" \n", file);
    return 1;
}

/*BACKUP WORKING FUNCTION.
// formating the dish. to something like that: "dish ....... 10NIS.
int formatDish(char* dish){
    int i = 0, cnt = 0, index;
    char num[FORMATLEN];
    if (!strcmp("stop", dish)) {
        return 0;
    }
    // searching for the price.
    for (i; i < strlen(dish); i++) {
        if ((48 <= dish[i] && dish[i] <= 57) || dish[i] == '\0' || dish[i] == '.') { // check the '.' condition
            num[cnt] = dish[i];
            num[cnt + 1] = '\0';
            cnt += 1;
        }
    }
    // extending the dish string with '.' up to 23 characters, then price and " NIS"
    index = strlen(dish) - strlen(num);
    i = index;
    for (i; i < FORMATLEN; i++) {
        if (FORMATLEN - i <= strlen(num) + 4) {
            strcat(dish, " ");
            strcat(num, "NIS");
            strcat(dish, num);
            break;
        }
        dish[i + 1] = '\0';
        dish[i] = '.';
    }
    return 1;
}
*/

/*
// main function. int argc, char* argv[]
int main() {
    // check arguments.
    /*
        if (argc < 3) {
        perror("bad argument count");
        exit(1);
    }
    
char* argv[] = { " ", "BBB", "1" };

// declaring and initializing variables.
int file, length, i = 0, cnt, dircreate;
file = 0;
char dish[SIZE], fname[SIZE], buff[SIZE], dirname[SIZE], ch[4];
strcpy(dirname, "");
strcpy(dish, "");
strcpy(fname, "");

ch[1] = '.';
ch[2] = ' ';
ch[3] = '\0';

// assembling file name.
strcpy(fname, argv[1]);
strcat(fname, ".txt");
file = openFile(file, fname);

// printing menu header.
strcpy(buff, argv[1]);
strcat(buff, " Menu");
printToFile(buff, file);
printToFile(" \n\n", file);

// while loop for printing the menu itself.
for (i; i < atoi(argv[2]); i++) {
    // printing the letter near each dish type and getting input{
    ch[0] = 'a' + i;

    printToFile(ch, file);
    printf("Insert dish type %c:\n", ch[0]);
    strcpy(dish, "");
    gets(dish);
    printToFile(dish, file);
    printToFile(" \n", file);
    // }
    cnt = 1;
    // loop for printing the dishes.
    while (strcmp("stop", dish)) {
        // asking for input.
        printf("Insert dish name %d:\n", cnt);
        gets(dish);
        // formating the dish also check for "stop".
        if (!formatDish(dish)) {
            printToFile(" \n", file);
            continue;
        }
        // printing the indentation, dish and new line for the next dish.
        printToFile("  ", file);
        printToFile(dish, file);
        printToFile(" \n", file);
        cnt += 1;
    }
}
// printing bon apetit.
printToFile("         Bon Apetit", file);

// assembling directory name.
strcat(dirname, argv[1]);
strcat(dirname, "_");
strcat(dirname, "Order");
// creating a directory.
dircreate = mkdir(dirname, 0777);
printf("Successfuly created // %s.txt created, %s dir created.\n", fname, dirname);
}
*/