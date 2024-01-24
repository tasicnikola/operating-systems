#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    DIR* dp;
    struct dirent* dirp;
    int subDirs, regDirs, links;
    struct stat statbuf;
    int result;
    char path[255];

    if(argc != 2) {
        printf("One argument needed.\n");
        exit(1);
    }

    if((dp = opendir(argv[1])) == NULL) {
        printf("Error while opening directory. \n");
        exit(1);
    }

    subDirs = regDirs = links = 0;

    while((dirp = readdir(dp)) != NULL) {
        strcpy(path, argv[1]);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        if((result = stat(path, &statbuf)) == -1) {
            printf("Error while reading data of directory. \n");
            continue;
        }

        if(S_ISDIR(statbuf.st_mode)) {
            subDirs++;
        }

        if(S_ISREG(statbuf.st_mode)) {
            regDirs;
        }

        if(S_ISLNK(statbuf.st_mode)) {
            links;
        }
    }

    printf("Directory stats: %s\nSubfolders: %d\nRegular folders: %d\nLinks: %d\n", argv[1], subDirs, regDirs, links);
    closedir(dp);
}
