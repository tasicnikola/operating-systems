#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_NAME 1024

void processdir(char* path, int* count, int* size);

int main(int argc, char* argv[]) {
    char dirname[MAX_NAME];
    char path[MAX_NAME];
    struct stat statbuf;
    DIR* dp;
    struct dirent* dirp;
    int count, size;

    if (argc < 2)
    {
        printf("Not enough arguments.\n");
        exit(-1);
    }

    strcpy(dirname, argv[1]);

    if(stat(dirname, &statbuf) < 0) {
        printf("Error while loading status of given path.\n");
        exit(-1);
    }

    if(!S_ISDIR(statbuf.st_mode)) {
        printf("Given argument is not path do directory.\n");
        exit(-1);
    }

    dp = opendir(dirname);
    if(dp == NULL) {
        printf("Error while opening directory.\n");
        exit(-1);
    }

    while((dirp = readdir(dp)) != NULL) {
        if(strcmp(dirp->d_name, ".") == 0
            || strcmp(dirp->d_name, "..") == 0)
            continue;

        strcpy(path, dirname);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        stat(path, &statbuf);

        if(S_ISDIR(statbuf.st_mode)) {
            processdir(path, &count, &size);
            printf("%s\t%d\t%d\n", dirp->d_name, count, size);
        }
    }

    closedir(dp);

    return 0;
}

void processdir(char* path, int* count, int* size) {
    DIR* dp;
    struct dirent* dirp;
    char path1[MAX_NAME];
    struct stat statbuf;

    dp = opendir(path);

    if(dp == NULL) {
        printf("Error while opening directory.\n");
        exit(-1);
    }

    *count = 0;
    *size = 0;

    while((dirp = readdir(dp)) != NULL) {
        strcpy(path1, path);
        strcat(path1, "/");
        strcat(path1, dirp->d_name);

        stat(path1, &statbuf);

        if(S_ISREG(statbuf.st_mode)) {
            *count = *count + 1;
            *size = *size + statbuf.st_size;
        }
    }

    closedir(dp);
}
