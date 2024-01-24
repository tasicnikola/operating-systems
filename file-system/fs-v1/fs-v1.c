#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char* argv[]) {
    DIR* dp;
    struct dirent* dirp;

    if(argc != 3) {
        printf("Two arguments needed. \n");
        exit(1);
    }

    if((dp = opendir(argv[1])) == NULL) {
        printf("Error while opening directory. \n");
        exit(1);
    }

    while((dirp = readdir(dp)) != NULL) {
        if(strstr(dirp->d_name, argv[2]) != NULL) {
            printf("%s\n", dirp->d_name);
        }
    }

    closedir(dp);
}
