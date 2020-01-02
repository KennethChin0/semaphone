#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <unistd.h>

#define KEY 24601
#define KEY2 24602

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};

// int write();
int create();
int delete();
int view();
