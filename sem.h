  
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
  int fd[2];
} Semaphore;

void sem_init(Semaphore *s, int value);
void sem_close(Semaphore *s);
void sem_wait(Semaphore *s);
void sem_signal(Semaphore *s);