#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>

// declaring a struct for the dish.
struct Node {
    char* dish;
    float price;
    int amount;
};

// declaring functions.
void closeFile(int fd);
int orderProcess(char* order);
int printToFile(char str[], int fd);
float getPrice(char* order, int fdr);
int openFile(int fd, char* fname, int rw);
int getInput(struct Node** dishes, int fd);
int readFromFile(int fd, char* buff, int len);
void createOrder(struct Node* dishes, int fd, char** argv);

// main function.
# define SIZE 100 // constant for order input.

int main(int argc, char* argv[]) {
        if(argc < 3){
        printf("bad parameter count\n");
        exit(0);
    }
    
    // declaring variables.
    struct Node* orderList;
    if ((orderList = malloc(sizeof(struct Node))) == NULL) {
        printf("memmory allocation error");
        free(orderList);
        exit(1);
    }

    char buff[2];
    strcpy(buff, "");
    char order[SIZE];
    char* price;
    int fdr = 0, fdw = 0, flg = 1;

    char* fname = malloc(sizeof(char) * (strlen(argv[2]) + 5));
    char* name = malloc(sizeof(char) * (strlen(argv[1]) + strlen(argv[2]) + 12)); // change

    if (name == NULL || fname == NULL) {
        printf("memmory allocation error");
        free(fname);
        free(name);
        exit(1);
    }

    // build and open write file name.
    strcpy(name, argv[1]);
    strcat(name, "_Order");
    strcat(name, "/");
    strcat(name, argv[2]);
    strcat(name, ".txt");
    fdw = openFile(fdw, name, 2); // open file for write.

    // build and open read file name.
    strcpy(fname, argv[1]);
    strcat(fname, ".txt");

     fdr = openFile(fdr, fname, 1); // open file for read.

    // fill the orderlist array.
    if (getInput(&orderList, fdr)) {

        // creating the file.
        createOrder(orderList, fdw, argv);

        // changing mode to read only and cancelling files.
        chmod(name, 00444); // S_IRUSR | S_IRGRP | S_IROTH
    }

    free(fname);
    free(name);
    free(orderList);
    printf("Order created! // Tiran.txt Created in BBB_Order Dir with Read Mode\n");

    closeFile(fdw);
    closeFile(fdr);
}

// a function to print all the dishes ordered to a file.
void createOrder(struct Node* dishes, int fd, char** argv) {
    // declare variables.
    int i = 0;
    float sum = 0;
    char* printer;
    if ((printer = malloc(sizeof(char) * (strlen(argv[1]) + 1))) == NULL) {
        printf("memmory allocation error");
        free(printer);
        exit(1);
    }

    // declare time variables.
    int day, month, year;
    time_t now;
    time(&now);
    struct tm* local = localtime(&now);

    day = local->tm_mday;
    month = local->tm_mon + 1;
    year = local->tm_year + 1900;

    // printing the first part of file.
    printToFile(argv[1], fd);
    printToFile(" Order", fd);
    printToFile(" \n\n", fd);

    // calculating the overall price of the order.
    while (strcmp(dishes[i].dish, "Finish")) {
        if (dishes[i].amount > 0 && dishes[i].price > 0) {
            sum += dishes[i].price * dishes[i].amount;
            printToFile(dishes[i].dish, fd);
            printToFile(" \n", fd);
            i += 1;
        }
        else
            i += 1;
    }
    // printing "Total price: ##NIS.
    printToFile("Total Price: ", fd);
    gcvt(sum, 10, printer);

    //sprintf(printer, "%f", sum);
    printToFile(printer, fd);
    printToFile("NIS", fd);

    printToFile(" \n", fd);
    printToFile(" \n", fd);

    // printing date.
    sprintf(printer, "%02d", day);
    printToFile(printer, fd);

    printToFile("/", fd);

    sprintf(printer, "%02d", month);
    printToFile(printer, fd);

    printToFile("/", fd);

    sprintf(printer, "%d", year);
    printToFile(printer, fd);
}

int getInput(struct Node** dishes, int fd) {
    // declare variables.
    char order[SIZE], *buff;
    int i = 0, j = 0;
    float sum = 0;

    printf("Enter dish order:\n");
    while (1) { // thinks of some condition
    
        // get user input.
        strcpy(order, "");
        fgets(order, 100, stdin);
        order[strlen(order) - 1] = '\0';

        // CHECK IF FINISH CONFIRM OR CANCEL
        if ( ! strcmp(order, "Finish")) {

            // add finish to array so we know its "length".
            if (((*dishes + i)->dish = malloc(sizeof(char) * (strlen(order) + 1))) == NULL) {
                printf("memmory allocation error");
                free((*dishes + i)->dish);
                exit(1);
            }
            strcpy((*dishes + i)->dish, order);

            for (j; j < i; j++) {
                if ((*dishes + j)->price > 0) {
                    sum += (*dishes + j)->price * (*dishes + j)->amount;
                }
            }

            // printing total price and asking to confirm or cancel.
            printf("Total Price: %0.2f NIS (Confirm to approve/ else to cancel.)\n", sum);
        fgets(order, 100, stdin);
        order[strlen(order) - 1] = '\0';

            // if confirm we continue.
            if ( !strcmp(order, "Confirm"))
                return 1;

            // or we just not write to the file.
            else {
                printf("Order Canceled.");
                return 0;
            }
        }

        // add dish to order.
        if (((*dishes + i)->dish = malloc(sizeof(char) * (strlen(order) + 1))) == NULL) {
            printf("memmory allocation error");
            free((*dishes + i)->dish);
            exit(1);
        }
        strcpy((*dishes + i)->dish, order);

        // getting order amount.
        (*dishes + i)->amount = orderProcess(order);

        // getting price of ordered dish.
        (*dishes + i)->price = getPrice(order, fd);

        // extending the array.
        if ((*dishes = realloc(*dishes, sizeof(struct Node) * (i + 2))) == NULL) {
            printf("memmory reallocation error");
            free(*dishes);
            exit(1);
        }
        i += 1;
    }
}



// processing the order 
int orderProcess(char* order) {

    // declare variables.
    int i = 0, len = 2, index = 0, num;
    char* amount;

    if ((amount = (char*)malloc(sizeof(char) * len)) == NULL) {
        printf("memmory allocation error");
        free(amount);
        exit(1);
    }
    amount[0] = '\0';

    // getting the amount of the ordered dish.
    while (order[i] != '\0') {
        if (48 <= order[i] && order[i] <= 57) {
            if ((amount = realloc(amount, len)) == NULL) {
                printf("memmory reallocation error");
                free(amount);
                exit(1);
            }

            amount[index + 1] = '\0';
            amount[index] = order[i];
            len += 1;
            index += 1;
        }
        i += 1;
    }

    order[strlen(order) - (index + 1)] = '\0';
    num = atoi(amount);
    free(amount);
    return num;
}

// getting price function.

float getPrice(char* order, int fdr) {

    // declare variables.
    int cnt = 0, flg = 0, fptr = 0;
    float num;
    char* price = malloc(sizeof(char)*2);
    char* buff = malloc(sizeof(char) * (strlen(order) + 1));

    if (price == NULL || buff == NULL) {

        printf("memmory allocation error");
        free(buff);
        free(price);
        exit(1);
    }
    buff[strlen(order)] = '\0';

    // search for Dish

    while (readFromFile(fdr, buff, strlen(order))) {
        if (!strcmp(buff, order)) {
            //printf("%s", order);
            readFromFile(fdr, buff, 1);
            readFromFile(fdr, buff, 1);
            if(buff[0] != '.')
                break;
            flg = 1;

            // getting the price similar to the get price program.
            while (buff[0] != '\n' && buff[0] != EOF) {
                if ((buff = realloc(buff, sizeof(char) * 2)) == NULL) {
                    printf("memmory reallocation error");
                    free(buff);
                    exit(1);
                }
                buff[1] = '\0';

                // reading character by character until we encounter digits then adding them to the price string.
                readFromFile(fdr, buff, 1);
                if ((48 <= buff[0] && buff[0] <= 57) || buff[0] == '\0' || (cnt != 0 && buff[0] == '.')) {
                    if ((price = realloc(price, sizeof(char) * (strlen(price) + 1))) == NULL) {
                        printf("memmory reallocation error");
                        free(buff);
                        exit(1);
                    }

                    price[cnt + 1] = '\0';
                    price[cnt] = buff[0];
                    cnt += 1;
                }
            }
            break;
        }
        fptr += 1;

        lseek(fdr, fptr, 0);
    }

    // if dish not found then we empty price string.
    if (flg == 0) {
        printf("Dish not found!\n");
        lseek(fdr, 0, 0);
        return -1;
    }

    num = atof(price);
    free(buff);

    // return to start of file and return price.
    lseek(fdr, 0, 0);
    return num;
}

// standard open file functions read write.
int openFile(int fd, char* fname, int rw) {
    if (rw == 1) {
        if ((fd = open(fname, O_RDONLY)) == -1) {
            printf("Restaurant Not Found!\n");
            exit(1);
        }
    }

    if (rw == 2) {
        chmod(fname, 00666); // S_IRUSR | S_IRGRP | S_IROTH
        if ((fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1) {
            printf("Restaurant Not Found!\n");
            exit(1);
        }
    }
    return fd;
}

// standard print to file function.
int printToFile(char str[], int fd) {
    int wbytes, rbytes;
    rbytes = strlen(str);

    if ((wbytes = write(fd, str, rbytes)) == -1) {
        perror("Write to file");
        return -1;
    }

    if (wbytes != rbytes) {
        perror("Bad write");
        return -1;
    }
}

// standard read from file.
int readFromFile(int fd, char* buff, int len) {
    int rbytes;

    if ((rbytes = read(fd, buff, len)) == -1) {
        perror("read from file");
        return -1;
    }
    return rbytes;
}

// standard function for closing a file.
void closeFile(int fd) {
    if ((close(fd)) != 0)
        perror("File close");
}