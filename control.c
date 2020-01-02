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


int main(int argc, char * argv[]){
  se.val = 1;
  if (argc > 1){
    if (strcmp(argv[1], "-c") == 0){
      create();
    }
    if (strcmp(argv[1], "-v") == 0){
      view();
    }
    if (strcmp(argv[1], "-r") == 0){
      delete();
    }

  }
  return 0;
}

int create(){
  // printf("create\n");
  int semd = semget(KEY, 1, IPC_CREAT | 0644);
  int shmd, fd;
  if (semd < 0){
    printf("%s\n", strerror(errno));
    return -1;
  }
  semctl(semd, 0, SETVAL, se);
  shmd = shmget(KEY2, sizeof(char *), IPC_CREAT | 0644);
  if (shmd < 0){
    printf("%s\n", strerror(errno));
    return -1;
  }
  printf("Shared Memory Created\n");
  fd = open("story.txt", O_CREAT | O_RDWR, 0644);
  if (fd < 0) {
    printf("%s\n", strerror(errno));
    return -1;
  }
  printf("File Created\n");
  close(fd);
  return 0;
}

int view(){
  int fd = open("story.txt", O_RDONLY);
  if (fd < 0){
    printf("%s\n", strerror(errno) );
    return -1;
  }
  FILE * f = fopen("story", "r");
  printf("The Story So Far:\n");
  char buff[1024];
  buff[0] = '\0';
  if (strlen(buff) != 0){
    *(strrchr(buff, '\n') + 1) = '\0';
  }
   printf("%s\n", buff);
   close(fd);
   return 0;
}

int delete(){
  // printf("remove\n");
  struct sembuf sb;
  int semd = semget(KEY, 1, 0);
  if (semd < 0){
    printf("%s\n", strerror(errno) );
    return -1;
  }
  printf("Trying to get in\n");
  semop(semd,&sb, 1);
  int shmd = shmget(KEY2, sizeof(char *), 0);
  if (shmd < 0){
    printf("%s\n", strerror(errno));
    return -1;
  }
  view();
  shmctl(shmd, IPC_RMID, 0);
  printf("Shared memory removed\n");
  remove("file.txt");
  printf("File Removed\n");
  semctl(semd, IPC_RMID, 0);
  printf("Semaphore removed\n");
  return 0;
}
