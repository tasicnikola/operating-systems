#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file>

int main(int argc, char* argv[]) {
    int fd;
    fd = open(argv[2], O_CREAT | O_TRUNC | O_RDWR, 0777);
    dup2(fd,1);
    close(fd);
    execlp(argv[1], argv[1], NULL);
    printf("This shouldn't execute!");
}
