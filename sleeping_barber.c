/*
Not sure if we had too make our own semaphore header so I used the default header.
I tried making my own but it did not work completely.
*/
#include <pthread.h>
//header for semaphores
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//creating all semaphores needed
sem_t WaitingRoom;
sem_t BarberChair;
sem_t SleepBarber;
sem_t CustomerWait;
//will be used to check if done with a task
int isTaskComplete = 0;
// limites the max amount of customers to be inputed
#define MAX_NUMBER_CUSTOMERS 25
// creates random wait time
void RandomWaitTime (int seconds) 
{
    int time;
    time = (int) ( ( drand48() * seconds ) + 1);
    sleep(time);
}
//customer function, is the process the customer runs through
void *Customer (void *number) 
{
    int id = *(int *)number;
    //creates time for the customer to get to the barber shop
    printf("'Customer %d' on the way to the barber shop.\n", id);
    RandomWaitTime(5); 
    //waits for a seat in the waiting room
    printf("'Customer %d' entering the barber shop.\n", id);
    sem_wait(&WaitingRoom);
    // waits for barber to be ready
    printf("'Customer %d' in the waiting room.\n", id);
    sem_wait(&BarberChair);
    //creates open seat in waiting room
    sem_post(&WaitingRoom);
    // waits up the customer
    printf("'Customer %d' is waking up the barber.\n", id);
    sem_post(&SleepBarber);
    //gets a haircut
    printf("'Customer %d' is getting haircut.\n", id);
    sem_wait(&CustomerWait);
    //is done so leaves barber shop
    sem_post(&BarberChair);
    printf("'Customer %d' exiting the barber shop.\n", id);
}
//barber function, is the process the barber will go through
void *Barber ()
{
    while (!isTaskComplete) 
    {
        //barber sleeps 
        printf("The barber sleeping.\n");
        sem_wait(&SleepBarber);
        if (!isTaskComplete)
        {
            //barber cuts hair
            printf("The barber is cutting a customers hair.\n");
            RandomWaitTime(3);
            //finishes haircut
            printf("The barber is done cutting one customer's hair.\n");
            sem_post(&CustomerWait);
        }
        else 
        {
            // finishes with customers
            printf("The barber is done with all customers.\n");
            
        }
        
    }

}

int main() 
{
    //initialize variables and threads 
    int numberOfCustomers;
    int numberOfChairs;
    int i;
    pthread_t barberThreadInit;
    pthread_t customerThreadInit[MAX_NUMBER_CUSTOMERS];
    int randomNumberSeed;
    int Numbers[MAX_NUMBER_CUSTOMERS];
    printf("\n");
    //grab the customers, chairs and random seed from user  
    printf("Input the amount of customers getting a haircut: ");
    scanf("%d", &numberOfCustomers);
    if (numberOfCustomers > MAX_NUMBER_CUSTOMERS) 
    {
        printf("\nThe maximum number of customers is: %d\n", MAX_NUMBER_CUSTOMERS);
        printf("Please input a small number. \n\n");
        exit(-1);
        
    }
    printf("\nInput total amount chairs in waiting room: ");
    scanf("%d", &numberOfChairs);
    printf("\nInput a number for random seed: ");
    scanf("%d", &randomNumberSeed);
    printf("\n");
    srand48(randomNumberSeed);
    for (i = 0; i < MAX_NUMBER_CUSTOMERS; i++) 
    {
        Numbers[i] = i;
    }
    // initialize semaphores to avoid deadlock
    sem_init(&WaitingRoom, 0, numberOfChairs);
    sem_init(&BarberChair, 0, 1);
    sem_init(&SleepBarber, 0, 0);
    sem_init(&CustomerWait, 0, 0);
    //uses threads
    pthread_create(&barberThreadInit, NULL, Barber, NULL);
    for (i = 0; i < numberOfCustomers; i++) 
    {
        pthread_create(&customerThreadInit[i], NULL, Customer, (void *)&Numbers[i]);
    }
    for (i = 0; i < numberOfCustomers; i++) 
    {
        pthread_join(customerThreadInit[i],NULL);
    }

    isTaskComplete = 1;
    sem_post(&SleepBarber);
    pthread_join(barberThreadInit,NULL);
    //code done so ends program
}
