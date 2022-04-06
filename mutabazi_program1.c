#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <sys/types.h>

#define sizeOfInput 1000

// global variables
char* fileType;
char* fileName;
char* isFileFound;

// functions
char* getTypeToSearch(char* str);
char* getFileName(char* str);

int main() {

    // get current dir
    char * path = malloc(255*sizeof(char));
    getcwd(path, 255);

    // intialize dir
    DIR * dir = opendir(path);
    struct dirent * dirEntry = readdir(dir);

    //get user input
    char input1[sizeOfInput];
    fgets(input1, sizeOfInput, stdin);

    //intialize file
    FILE *file;
    
    // get what type to search
    fileType = getTypeToSearch(input1);

    // =====
    // case for -name
    // =====
    if (strcmp(fileType,"-name") == 0) {
        fileName = getFileName(input1);
        while (dirEntry != NULL){
            if (strcmp(fileName, dirEntry->d_name) == 0){
                printf("Directory found");
            } else {
                dirEntry = readdir(dir);
            }
        }
        printf("File not found \n");
    }

    // =====
    // case for -R
    // =====
    else if (strcmp(fileType,"-R") == 0) {
        fileName = getFileName(input1);
        while (dirEntry != NULL) {
            if (dirEntry->d_type == DT_DIR) {
                if ((strcmp(dirEntry->d_name,".") != 0) && (strcmp(dirEntry->d_name,"..") != 0)) {
                    char * dirName = malloc(255 * sizeof(char));
                    strcat(dirName, path);
                    strcat(dirName, dirEntry->d_name); 

                    // recursive
                    while (dirEntry != NULL) {
                        if (dirEntry->d_type == DT_DIR) {
                            if ((strcmp(dirEntry->d_name,".") != 0) && (strcmp(dirEntry->d_name,"..") != 0)) {
                                char * dirName = malloc(255 * sizeof(char));
                                strcat(dirName, path);
                                strcat(dirName, dirEntry->d_name); 
                            }
                        } 
                        else if (dirEntry->d_type == DT_REG) {
                            if (strcmp(fileName, dirEntry->d_name) == 0){
                                printf("File found");
                            }
                        }
                        dirEntry = readdir(dir);
                    }
                    printf("The file not found \n");
                }
            } 
            else if (dirEntry->d_type == DT_REG) {
                if (strcmp(fileName, dirEntry->d_name) == 0){
                    printf("File found");
                }
            }
            dirEntry = readdir(dir);
        }
        printf("The file not found \n");
    } 

    // =====
    // case for UID
    // =====
    else if (strcmp(fileType,"-uid") == 0) {
        fileName = getFileName(input1);
        
        // gettin file UID
        uid_t fileUID = getuid();
        
        while (dirEntry != NULL){
            uid_t fileUID;
        }
    } 

    // =====
    // case for type
    // =====
    else if (strcmp(fileType,"-type") == 0) {
        fileName = getFileName(input1);
        while (dirEntry != NULL) {
            if (strcmp(fileName, dirEntry->d_name) == 0){ // find if name given exists in the path
                printf("Directory/file found");
                if (dirEntry->d_type == DT_DIR) { // if name exists and is a folder
                    // print all dirs in the path
                    while (dirEntry->d_type == DT_DIR){
                        printf("%s \n", dirEntry->d_name);
                    } 
                } else if (dirEntry->d_type == DT_REG) { // if name exists and is a file
                    // print all files in the path
                    while (dirEntry->d_type == DT_REG){
                        printf("%s \n", dirEntry->d_name);
                    } 
                } else if (dirEntry->d_type == DT_BLK) { // if name exists and is a blk
                    // PRINT ALL BLK FILES
                    while (dirEntry->d_type == DT_BLK){
                        printf("%s \n", dirEntry->d_name);
                    } 
                } else if (dirEntry->d_type == DTF_HIDEW){
                    while (dirEntry->d_type == DTF_HIDEW){
                        printf("%s \n", dirEntry->d_name);
                    }
                } else {
                    printf("%s \n", fileName);
                }
            }
            dirEntry = readdir(dir);
        }

        printf("File or Folder doesn't exist \n");
    } 

    // =====
    // case for other
    // =====
    else {
        printf("please type '-name' or '-R' or '-uid' or '-type' before file name \n");
    }
}

char* getTypeToSearch(char* str) {
    int inputLength = strlen(str);
    int i = 0, j = 0;
    char* fileType = (char*)malloc(sizeof(str) + 1);
    while (i < inputLength && str[i] != ' ') {
        fileType[i] = str[i];
        ++i;
    }
    fileType[i] = '\0';
    return fileType;
}

char* getFileName(char* str) {
    int inputLength = strlen(str);
    int i = 0;
    char* filename = (char*)malloc(sizeof(str) + 1);
    while (i < inputLength && str[i] != ' ') {
        ++i;
    }
    // return NULL for no second word
    if (i == inputLength) {
        return NULL;
    }
    ++i;
    int j = 0;
    while (i < inputLength && str[i] != ' ') {
        filename[j] = str[i];
        ++i, ++j;
    }
    filename[j] = '\0';
    return filename;
}