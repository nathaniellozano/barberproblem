#include "sem.h"

void sem_init(Semaphore *s, int value)
{
  s = (Semaphore *) malloc(sizeof(Semaphore));

  if(pipe(s->fd) < 0) {
    fprintf(stderr, "Error creating pipe.\n");
  }
  int i;
  //  c = 'x';
  char c;
  c = 'x';
  for(i = 0; i < value; i++) {
    write(s->fd[1], &c, 1);
  }
  return;
}

void sem_close(Semaphore *s)
{
  close(s->fd[0]);
  close(s->fd[1]);
  return;
}

void sem_signal(Semaphore *s)
{
  char c;
  c = 'x';
  write(s->fd[1], &c, 1);
  return;
}

void sem_wait(Semaphore *s)
{
  char c;
  while(read(s->fd[0], &c, 1) < 0);
  return;
}