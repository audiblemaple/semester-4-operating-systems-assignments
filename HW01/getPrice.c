#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

// declare functions.
int openFile(int fd, char* fname);
int readFromFile(int fd, char* buff);


# define NAME 100
# define SIZE 10

// main function.
int main(int argc, char* argv[]) {
    if(argc < 3){
        printf("Bad argument count\n");
        exit(0);
    }

    //declaring variables
    char buff[2];
    char price[SIZE];
    int cnt = 0, fd = 0, index = 0;
    char fname[NAME];
    char temp[NAME];

    // initializing variables.
    strcpy(buff, "");
    strcpy(price, "");

    // assembling file name.
    strcpy(fname, argv[1]);

    strcpy(temp, argv[2]);
    if(argc > 3){
        strcat(temp, " ");
        strcat(temp, argv[3]);
    }

    strcat(fname, ".txt");

    fd = openFile(fd, fname);

    do {
        // searching for the dish character by character.
        if (buff[0] == temp[index])
            index += 1;

        else
            index = 0;
        // if we found the dish, then we search for the price.
        if (index == strlen(temp)) {
            readFromFile(fd, buff);
            readFromFile(fd, buff);
            if(buff[0] != '.')
                break;

            while (buff[0] != '\n' && buff[0] != EOF) {
                // reading from file.
                readFromFile(fd, buff);
                // if the character is a digit or it is a '.' AFTER we found a digit then we add the character to the price string.
                if ((48 <= buff[0] && buff[0] <= 57) || buff[0] == '\0' || (cnt != 0 && buff[0] == '.')) {
                    price[cnt] = buff[0];
                    price[cnt + 1] = '\0';
                    cnt += 1;
                }
            }
            break;
        }
    // read from file.
    } while (readFromFile(fd, buff));

    // if we havent found the price then we print dish not found.
    if (price[0] == '\0')
        printf("Dish not found!\n");
    // else we print the file.
    else
        printf("%sNIS\n", price);
}


// standard function to open file.
int openFile(int fd, char* fname) {
    if ((fd = open(fname, O_RDONLY)) == -1) {
        printf("Restaurant Not Found!\n");
        exit(-1);
    }
    return fd;
}

// standard function to read from file.
int readFromFile(int fd, char* buff) {
    int rbytes;
    if ((rbytes = read(fd, buff, 1)) == -1) {
        perror("Read form file");
        return -1;
    }
    return rbytes;
}