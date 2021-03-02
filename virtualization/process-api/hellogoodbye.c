#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int rc = fork();
    if (rc < 0) {
      printf("Fork failed: % d\n", rc);
      exit(1);
    } else if (rc == 0) {
      printf("hello %d\n", rc);
      int ret = waitpid(10);
    } else {
      int ret = wait(NULL);
      printf("Ret: %d\n", ret);
      printf("goodbye %d\n", rc);
    }
    return 0;
}
