#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <dirent.h>

#define PLEN 100

// main function
int main(int argc, char* argv[]){
    // check for valid argument count.
    if(argc < 2){
        printf("Invalid parameter count\n");
        exit(0);
    }
    // declare variables.
    DIR *ptr;  
    struct dirent *ent;
    int cnt=0, i = 0;;
    char dir[PLEN];

    // assemble directory path
    strcpy(dir, argv[1]);
    strcat(dir, "_Order");

    // opening directory.
    ptr = opendir(dir);
    while((ent = readdir(ptr)) != NULL) {
        // on successful open we loop through all the entries.
        while(ent->d_name[i] != '\0' && i < strlen(ent->d_name) -2){
            // searching for files with .txt extentions.
            if(ent->d_name[i] == 't' && ent->d_name[i+1] == 'x' && ent->d_name[i+2] == 't'){
                cnt += 1;
                i += 1;
            }
            i += 1;
        }
        i = 0;
        
    }

    // print amount of files found
    printf("%d orders\n", cnt);
}


/*BACKUP
            if(ent -> d_type != DT_DIR || ){ 
                n++;
            } 
*/

/* IMPORTANT
           struct dirent {
               ino_t          d_ino;       /* inode number 
               off_t          d_off;       /* not an offset; see NOTES 
               unsigned short d_reclen;    /* length of this record 
               unsigned char  d_type;      /* type of file; not supported
                                              by all file system types 
               char           d_name[256]; 
           };
*/


/*
int file_count = 0;
DIR * dirp;
struct dirent * entry;
printf("\n%s\n", path);

if((dirp = opendir(path)) == NULL){ // <-- change here
    perror("error opening directory");
}
printf("open\n");

while ((entry = readdir(dirp)) != NULL) {
    printf("%s\n", entry->d_type);
    if (entry->d_type == DT_REG) {  If the entry is a regular file 
        printf("increased\n");
         file_count++;
    }
}

printf("%d", file_count);
closedir(dirp);
*/