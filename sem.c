#include "sem.h"
//function for initializing semaphore
void sem_init(Semaphore *s, int value)
{
  s = (Semaphore *) malloc(sizeof(Semaphore));

  if(pipe(s->fd) < 0) {
    fprintf(stderr, "Error creating pipe.\n");
  }
  int i;
  //  c = '.';
  char c;
  c = '.';
  for(i = 0; i < value; i++) {
    write(s->fd[1], &c, 1);
  }
  return;
}
//function for closing a semaphore
void sem_close(Semaphore *s)
{
  close(s->fd[0]);
  close(s->fd[1]);
  return;
}
//function for signalling
void sem_signal(Semaphore *s)
{
  char c;
  c = '.';
  write(s->fd[1], &c, 1);
  return;
}
//function for waiting
void sem_wait(Semaphore *s)
{
  char c;
  while(read(s->fd[0], &c, 1) < 0);
//printf("hmm..\n");
  return;
}
