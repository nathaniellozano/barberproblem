  
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
// struct for semaphore 
// will be used for signalling and waiting
typedef struct {
  int fd[2];
} Semaphore;
//functions used in sem.c
void sem_init(Semaphore *s, int value);
void sem_close(Semaphore *s);
void sem_wait(Semaphore *s);
void sem_signal(Semaphore *s);
