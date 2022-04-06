#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <sys/wait.h>

void printDirectoryContents(char * path);

int main() {
    // get current dir
    char * path = malloc(255*sizeof(char));
    getcwd(path, 255);

    // intialize dir
    DIR * dir = opendir(path);
    struct dirent * dirEntry = readdir(dir);

    printf("Contents of %s\n",path);

    printDirectoryContents(path);
}

void printDirectoryContents(char * path) {
    DIR * dir = opendir(path);
    struct dirent * dirEntry = readdir(dir);

    int status = 0;
    int count = 0;

    pid_t pid = fork();

    while (dirEntry != NULL) {
        if (dirEntry->d_type == DT_DIR) {
            if ((strcmp(dirEntry->d_name,".") != 0) && (strcmp(dirEntry->d_name,"..") != 0)) {
                char * dirName = malloc(255 * sizeof(char));
                strcat(dirName, path);
                strcat(dirName, dirEntry->d_name);  
                printf("Contents of %s\n",dirName);

                pid_t pid = fork();

                // recursive
                while (dirEntry != NULL) {
                    if (dirEntry->d_type == DT_DIR) {
                        if ((strcmp(dirEntry->d_name,".") != 0) && (strcmp(dirEntry->d_name,"..") != 0)) {
                            char * dirName = malloc(255 * sizeof(char));
                            strcat(dirName, path);
                            strcat(dirName, dirEntry->d_name);  
                            printf("Contents of %s\n",dirName);
                        } 
                    } else if (dirEntry->d_type == DT_REG) {
                        if (pid == 0) {       
                            sleep(3);
                            exit(0);
                        } else {
                            sleep(3);
                            wait(&status);
                            if (WIFEXITED(status)) {
                                printf("Child Exited Normally\n");
                            }
                        }
                        printf("File %d: %s\n", count+1, dirEntry->d_name);
                    }
                    dirEntry = readdir(dir);
                }
            }
        } else if (dirEntry->d_type == DT_REG) {

            if (pid == 0) {       
                sleep(3);
                exit(0);
            } else {
                sleep(3);
                wait(&status);
                if (WIFEXITED(status)) {
                    printf("Child Exited Normally\n");
                }   

            }
            printf("File %d: %s\n",count, dirEntry->d_name);
        }
        dirEntry = readdir(dir);
    }
}