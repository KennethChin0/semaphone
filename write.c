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


int main(){
  int semid;
  semid = semget(KEY, 1, 0);
  if (semid < 0){
    printf("%s\n", strerror(errno) );
    return -1;
  }
  struct sembuf sem;
  sem.sem_num = 0;
  sem.sem_op = -1;
  semop(semid, &sem, 1);
  int shmid = shmget(KEY2, sizeof(char *), 0);
  if (shmid < 0){
    printf("%s\n", strerror(errno));
    return -1;
  }
  int fd = open("story.txt", O_WRONLY | O_APPEND, 0644);
  printf("trying to get in\n");

  if (fd < 0){
    printf("error opening file: %s\n", strerror(errno));
    sem.sem_op = 1;
    semop(semid, &sem, 1);
    return -1;
  }
  char *last = shmat(shmid,0,0);
  printf("Last addition: %s\n", last);
  char new[1024];
  printf("Your addition:");
  fgets(new, 1024, stdin);
  write(fd, new, strlen(new));
  close(fd);
  strcpy(last,new);
  shmdt(last);
  sem.sem_op = 1;
  semop(semid, &sem, 1);
  return 0;

}
