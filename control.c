#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/shm.h>
#include "semaphone.h"

union semun se;
struct sembuf sb;

int main(int argc, char* argv[]){
  sb.sem_num = 0;
  sb.sem_op = -1;
  se.val = 1;
  if (argc > 1){
    if (strcmp(argv[1], "-c") == 0){
      create();
    }
    else if (strcmp(argv[1], "-v") == 0){
      view();
    }
    else if (strcmp(argv[1], "-r") == 0){
      delete();
    }
  }
  return 0;
}


int create(){
  // printf("create\n");
  int semid = semget(KEY, 1, IPC_CREAT | 0644);
  if (semid < 0){
    printf("%s\n", strerror(errno));
    return -1;
  }
  printf("Semaphore Created\n");
  semctl(semid, 0, SETVAL, se);
  int shmid = shmget(KEY2, sizeof(char *),IPC_CREAT | 0644);
  if (shmid < 0){
    printf("%s\n", strerror(errno));
    return -1;
  }
  printf("Shared Memory Created\n");
  int fd = open("story.txt", O_CREAT | O_RDWR, 0644);
  if (fd < 0){
    printf("%s\n", strerror(errno));
    return -1;
  }
  printf("Story Created\n");
  close(fd);
  return 0;
}

int view(){
  // printf("viewn");
  int fd = open("story.txt", O_RDONLY);
  if (fd < 0){
    printf("%s\n", strerror(errno));
    return -1;
  }
  char buff[1024];
  buff[0] = '\0';
  read(fd, buff, 1024);
  if (strlen(buff) != 0){
    *(strrchr(buff, '\n') + 1) = '\0';
  }
  printf("The Story So Far:\n");
  printf("%s\n", buff);
  close(fd);
  return 0;
}

int delete(){
  // printf("remove\n");
  int semid = semget(KEY, 1, 0);
  if (semid < 0){
    printf("%s\n", strerror(errno));
    return 1;
  }
  printf("Trying to get in\n");
  semop(semid,&sb, 1);
  int shmid = shmget(KEY2, sizeof(char *), 0);
  if (shmid < 0){
    printf("%s\n", strerror(errno));
    return -1;
  }
  view();
  shmctl(shmid, IPC_RMID, 0);
  printf("Shared Memory Removed\n");
  remove("story.txt");
  printf("Story Removed\n");
  semctl(semid, IPC_RMID, 0);
  printf("Semaphore Removed\n");
  return 0;
}
