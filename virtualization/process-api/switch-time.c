#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUF_SIZE 1024
#define SHM_KEY 0x1234

const char *semName = "mutt";
const char *semName2 = "bubb";

int main(int argc, char *argv[])
{
   //int unlink = sem_unlink(semName);
   //if (unlink != 0) {
   //  printf("Error with unlink: %d\n", unlink);
   //}

   //unlink = sem_unlink(semName2);
   //if (unlink != 0) {
   //  printf("Error with unlink: %d\n", unlink);
   //}

   int shmid = shmget(SHM_KEY, 1024, 0644|IPC_CREAT);
   int *val = shmat(shmid, NULL, 0);
   *val = 0;

    pid_t pid;
    pid = fork();

    if (pid < 0) {
      perror("fork");
      exit(1);
    }

    sem_t *sem = sem_open(semName, O_CREAT, 0600, 0);
    if (sem == SEM_FAILED) {
      perror("Sem open: [sem_open] Failed\n");
      exit(1);
    }

    sem_t *sem2 = sem_open(semName2, O_CREAT, 0600, 0);
    if (sem2 == SEM_FAILED) {
      perror("Sem2 open: [sem_open] Failed\n");
      exit(1);
    }

    while (1 == 1) {
      printf("%d waiting for sem1\n", pid);
      int r = sem_wait(sem);
      printf("%d got sem1\n", pid);
      if (r != 0) {
        printf("Error: %d\n", r);
        exit(1);
      }

      printf("%d waiting for sem2\n", pid);
      r = sem_wait(sem2);
      printf("%d got sem2\n", pid);
      if (r != 0) {
        printf("Error: %d\n", r);
        exit(1);
      }

      // Release the first lock so the other process gets it.
      r = sem_post(sem);
      if (r != 0) {
        printf("Error: %d\n", r);
        exit(1);
      }
      printf("%d released sem1\n", pid);

      int shmid = shmget(SHM_KEY, 1024, 0644|IPC_CREAT);
      int *val = shmat(shmid, NULL, 0);

      printf("Read val: %d\n", *val);
      if (*val >= 5) {
        exit(0);
      }
      *val = *val + 1;

      // Release the second lock to the other process updates shmem.
      r = sem_post(sem2);
      printf("%d released sem2\n", pid);
      if (r != 0) {
        printf("Error: %d\n", r);
        exit(1);
      }
    }
    sem_close(sem);
    sem_close(sem2);
    return 0;
}

