#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

// declare functions.
int openFile(int fd, char* fname);
int readFromFile(int fd, char* buff);

int main(int argc, char*argv[]) {
    if(argc < 2){
        perror("bad argument count");
        exit(0);
    }
    // declare variables.
    int fd = 0, flg = 1;
    char* name = malloc(sizeof(char) * (strlen(argv[1]) + 5));
    strcpy(name, argv[1]);
    strcat(name, ".txt");
    char buff[2];
    buff[1] = '\0';
    // open file.
    fd = openFile(fd, name);
    
    // reading from file character by character until we reach the end of the file.
    while (flg != 0 && flg != -1) {
        flg = readFromFile(fd, buff);
        printf("%s", buff);
    }
}

// standard function to open file.
int openFile(int fd, char* fname) {
    if ((fd = open(fname, O_RDONLY)) == -1) {
        perror("open file");
        exit(1);
    }
    return fd;
}

// standard function for reading from file.
int readFromFile(int fd, char* buff) {
    int rbytes;
    if ((rbytes = read(fd, buff, 1)) == -1) {
        perror("Read file");
        return -1;
    }
    return rbytes;
}