#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int fd = open("file.txt", O_WRONLY | O_APPEND);
    int rc = fork();
    if (rc < 0) {
      printf("Fork failed: % d\n", rc);
      exit(1);
    } else if (rc == 0) {
      write(fd, "child\n", strlen("child\n"));
    } else {
      write(fd, "parent\n", strlen("parent\n"));
    }
    close(fd);
    return 0;
}
