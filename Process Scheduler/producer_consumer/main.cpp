/**
 * Name: Kevyn Higbee, Preston Roberts
 * 
 * Date: April 23, 2019
 * Assignment: Program 4
 * File: Main File Implementation
 **/

#include <unistd.h>
#include "PBuffer.h"
#include <stdlib.h>
#include <pthread.h>

// variables
PBuffer bndBuff;
pthread_mutex_t mutex;

// function prototypes
void *producer(void *param);
void *consumer(void *param);

/**
 * Main - Handles command line arguments, creates threads
 */
int main(int argc, char *argv[])
{
    // handle command line arguments
    if (argc != 4)
    {
        std::cout << "Usage: " << argv[0] << " <sleep time> <# producer threads> <# consumer threads>" << std::endl;
        exit(1);
    }
    int sleepTime = atoi(argv[1]);
    int producerThreads = atoi(argv[2]);
    int consumerThreads = atoi(argv[3]);
    
    // initializations
    pthread_mutex_init(&mutex, NULL);   // create and initialize mutex lock
    srand(time(NULL));                  // initilize rand()

    // create producer threads
    pthread_t pThreads[producerThreads];
    for (int i = 0; i < producerThreads; i++)
        pthread_create(&pThreads[i], NULL, producer, NULL);

    // create consumer threads
    pthread_t cThreads[consumerThreads];
    for (int z = 0; z < consumerThreads; z++)
        pthread_create(&cThreads[z], NULL, consumer, NULL);

    // sleep for period of time
    sleep(sleepTime);
    
    // done
    std::cout << "Finished execution. Program Termintating ... " << std::endl;

    return 0;
}

// FUNCTION DEFINITIONS

/**
 * Producer - producer thread routine, waits for random amount of time, then attempts to insert into buffer
 */
void *producer(void *param)
{
    bItm item;

    while(true)
    {
        // sleep for random period of time
        sleep(rand()%2);
        
        // generate random number
        item = rand() % 200;

        pthread_mutex_lock(&mutex);

        if(bndBuff.insertItem(item) != 0)
            std::cout << "Error: Producer unable to produce " << item << ". \n";
        else
            std::cout << "Producer produced " << item << ". \n";

        pthread_mutex_unlock(&mutex);
    }
}

/**
 * Consumer - consumer thread routine, waits for random amount of time, then attempts to remove rear of buffer
 */
void *consumer(void *param)
{
    bItm item;

    while(true)
    {
        // sleep for random period of time
        sleep(rand()%2);
        
        pthread_mutex_lock(&mutex);

        if(bndBuff.removeItem(&item) != 0)
            std::cout << "Error: Consumer unable to consume. \n";
        else
            std::cout << "Consumer consumed " << item << ". \n";

        pthread_mutex_unlock(&mutex);
    }
}