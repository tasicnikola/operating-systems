#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_NAME 1024

int main(int argc, char* argv[]) {
    char dirname[MAX_NAME];
    char path[MAX_NAME];
    char maxpath[MAX_NAME];
    int size;
    struct stat statbuf;
    DIR* dp;
    struct dirent* dirp;

    if (argc < 2) {
        printf("Not enough arguments.\n");
        exit(-1);
    }

    strcpy(dirname, argv[1]);

    if(stat(dirname, &statbuf) < 0) {
        printf("Error while loading status of given path.\n");
        exit(-1);
    }

    if(!S_ISDIR(statbuf.st_mode)) {
        printf("Argument is not path to directory.\n");
        exit(-1);
    }

    dp = opendir(dirname);
    if (dp == NULL)
    {
        printf("Error while opening directory.\n");
        exit(-1);
    }

    size = 0;
    while((dirp = readdir(dp)) != NULL) {
        strcpy(path, dirname);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        stat(path, &statbuf);
        if(S_ISREG(statbuf.st_mode)) {
            if(statbuf.st_size > size) {
                strcpy(maxpath, path);
                size = statbuf.st_size;
            }
        }
    }

    printf("directory %s has max size of %d\n", maxpath, size);
    closedir(dp);

    return 0;
}
