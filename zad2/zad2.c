#define _XOPEN_SOURCE 500
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ftw.h>

int sizeGlobal;



char *addToPath(const char *oldPath, char *fileName);

void printFileInfo(const char *filePath, struct stat *fileStat);



static int dirHandler(const char *filePath, const struct stat *fileStat, int typeFlag, struct FTW *ftwbuf) {
    if (typeFlag == FTW_F) {
        if (stat(filePath, fileStat) >= 0 && (long int) fileStat->st_size <= sizeGlobal
            && (long int) fileStat->st_size > 0) {
            printFileInfo(filePath, fileStat);
        }
    }
    return 0;
}

void searchDirectory_nftw(char *dirPath) {
    if (nftw(dirPath, dirHandler, 30, FTW_DEPTH | FTW_PHYS) == -1) {
        exit(4);
    }
}

void searchDirectory_stat(const char *dirPath) {
    DIR *dir = opendir(dirPath);
    if (dir == NULL) {
        exit(3);
    }
    struct dirent *dirent;
    struct stat *fileStat = malloc(sizeof(struct stat));
    while ((dirent = readdir(dir)) != NULL) {
        if (dirent->d_type == DT_REG) {
            char *filePath = addToPath(dirPath, dirent->d_name);
            if (stat(filePath, fileStat) >= 0 && (long int) fileStat->st_size <= sizeGlobal
                && (long int) fileStat->st_size > 0) {
                printFileInfo(filePath, fileStat);
            }
        }
        if ((dirent->d_type == DT_DIR) && strcmp(dirent->d_name, ".") != 0
            && strcmp(dirent->d_name, "..") != 0) {
            const char *nextDirPath = addToPath(dirPath, dirent->d_name);
            searchDirectory_stat(nextDirPath);
        }
    }
    free(fileStat);
    closedir(dir);
}

char *addToPath(const char *oldPath, char *fileName) {
    char *newPath = malloc(strlen(oldPath) + 1 + strlen(fileName) + 1);
    strcpy(newPath, oldPath);
    strcat(newPath, "/");
    strcat(newPath, fileName);
    return newPath;
}

void printFileInfo(const char *filePath, struct stat *fileStat) {
    //print permissions:
    char *permissions = malloc(11);
    for (int i = 0; i < 10; i++) permissions[i] = '-';
    permissions[10] = '\0';
    if (S_ISDIR(fileStat->st_mode)) permissions[0] = 'd';
    if (fileStat->st_mode & S_IRUSR) permissions[1] = 'r';
    if (fileStat->st_mode & S_IWUSR) permissions[2] = 'w';
    if (fileStat->st_mode & S_IXUSR) permissions[3] = 'x';
    if (fileStat->st_mode & S_IRGRP) permissions[4] = 'r';
    if (fileStat->st_mode & S_IWGRP) permissions[5] = 'w';
    if (fileStat->st_mode & S_IXGRP) permissions[6] = 'x';
    if (fileStat->st_mode & S_IROTH) permissions[7] = 'r';
    if (fileStat->st_mode & S_IWOTH) permissions[8] = 'w';
    if (fileStat->st_mode & S_IXOTH) permissions[9] = 'x';
    printf("%s\t", permissions);
    free(permissions);

    //print size
    printf("%10ldus\t", fileStat->st_size);

    //print date:
    char *buffer = malloc(20);
    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S",
             localtime(&fileStat->st_mtime));
    printf("%s\t", buffer);

    //print path:
    char absolutePath[1000];
    realpath(filePath, absolutePath);
    printf(" path: %s\n", absolutePath);
}


int main(int argc, char **argv) {

    if (argc == 2) {
        if (strcmp(argv[1], (char *) "-h") == 0 || strcmp(argv[1], (char *) "-help") == 0) {
            printf("program_name -nftw|-stat dir_path size_in_bytes\n");
            exit(0);
        }
    }
    if (argc != 4) {
        printf("Wrong number of arguments\n");
        exit(1);
    }

    char *dirPath = argv[2];
    sizeGlobal = atoi(argv[3]);

    if (strcmp(argv[1], (char *) "-nftw") == 0) {
        searchDirectory_nftw(dirPath);
    } else if (strcmp(argv[1], (char *) "-stat") == 0) {
        searchDirectory_stat(dirPath);
    } else {
        printf("Option not recognized\n");
        exit(2);
    }

    return 0;
}