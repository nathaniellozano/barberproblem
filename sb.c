#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

sem_t *customers; // 0
sem_t *barber; // 0
// mutex
sem_t *access_seats; // 1
int number_of_free_seats = 25;

int total_customers = 5;

// Prototypes
void barber_process();
void customer_process();

int main()
{
  // Init the semaphores
  customers = sem_open("customers", O_CREAT|O_EXCL, 0, 0);
  sem_unlink("customers");
  barber = sem_open("barber", O_CREAT|O_EXCL, 0, 0);
  sem_unlink("barber");
  access_seats = sem_open("access_seats", O_CREAT|O_EXCL, 0, 1);
  sem_unlink("access_seats");

  // start the randomness
  srand(time(NULL));

  // Create the customers
  int i;
  pid_t pid;
  if(fork() == 0)
    barber_process();
  for (i = 0 ;i < 10; i++) {
    sleep(rand() % 2);
    if((pid = fork()) < 0) {
      fprintf(stderr,"Error while forking process.\n");
      exit(1);
    }
    else if(pid == 0) {
        customer_process(i);
//	exit(0);
    }
  }
//  barber_process();

  return 0;
}

void barber_process()
{
  // runs in an infinite loop
  for (;;) {
    // tries to acquire a customer
    sem_wait(customers);
    // modify the number of available seats
    sem_wait(access_seats);
    // one chair gets free
    number_of_free_seats++;
    // the barber is ready to cut
    sleep(rand() % 5);
    sem_post(barber);
    // we don't need the lock on the chairs
    sem_post(access_seats);
    // here the barber is cutting hair
  }
}

void customer_process(int number)
{
  // runs in an infinite loop
  for (;;) {
    // tries to get access to the chairs
    sem_wait(access_seats);
    // if there are any free seats
    if (number_of_free_seats > 0) {
      // sitting down on a chair
      number_of_free_seats--;
      printf("[PID: %d] Customer %d sat down in waiting room.\n",
	     getpid(),number);
      // notifiy the barber, about a customer
      sem_post(customers);
      // don't need to lock the chairs anymore
      sem_post(access_seats);
      // customer wait if the barber is busy
      sem_wait(barber);
      printf("[PID: %d] Customer %d got his/her hair cut\n",
	     getpid(),number);
      exit(0);
    }
    // there are no free seats
    else {
      // but don't forget to release the seats
      sem_post(access_seats);
      // customer leaves without haircut
      printf("[PID %d] Customer %d left (no free seats).\n",
	     getpid(), number);
      exit(0);
    }
  }
}