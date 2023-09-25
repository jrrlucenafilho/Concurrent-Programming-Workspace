#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define QTD_PRINTERS 3
#define QTD_JOBS 10
#define SHARED_PRINTING_BUFFER_SIZE 10

//3 printers have to do 10 jobs, with a shared printing buffer between the printers, since only 3 can be used at a time
//Putting here since they'll be shared between threads

int shared_printing_buffer[SHARED_PRINTING_BUFFER_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};    //Especifica tamanha da linha de impressão
pthread_mutex_t mutex_job;   //Mutex for controlling job adding to buffer
pthread_barrier_t printing_barrier; //Barrier to control printing

//Just executes the job and sleeps for abit to simulate it
void* job(void* p)
{
    long id = (long)p;

    printf("job [%ld] is being added to shared buffer...\n", id);
    sleep(1);

    pthread_mutex_lock(&mutex_job); //CRITICAL SESSION BEGIN

    //Checks the buffer array until it finds em ampty spot
    for(int i = 0; i < SHARED_PRINTING_BUFFER_SIZE; i++){
        //Adds to the shared buffer in the free spot
        if(shared_printing_buffer[i] == 0){
            shared_printing_buffer[i] = id;
            printf("job [%ld] succesfully added to shared buffer\n", id);
        }
    }

    pthread_mutex_unlock(&mutex_job);   //CRITICAL SESSION END

    return;
}

void* printers_printing(void* p)
{
    long id = (long)p;
    int counter = 0;

    //Guarantees that the next printing spree only begins after printing all current 3 jobs
    while(counter < 3){
        printf("Printers are currently doing their jobs...\n");

        long _sleep = (rand() % 3) + 1; //Random wait time
        sleep(_sleep);

        pthread_barrier_wait(&printing_barrier);    //"Waiting for other threads to finish" point

        printf("[%ld] job done!\n", id);
		counter++;
    }
    printf("[%ld] barrier passed\n",id); //Only passes barrier after printers finish all 3 jobs

    return;
}

int main(void)
{
    //Initializing control data
	pthread_mutex_init(&mutex_job, NULL);
    pthread_barrier_init(&printing_barrier, NULL, 3); //for 3 printers, meaning the barrier will await for 

    //Criação das impressoras e jobs
    pthread_t printers[QTD_PRINTERS];
    pthread_t jobs[QTD_JOBS];

    //Initializing printers
    for(long i = 0; i < QTD_JOBS; i++){
        pthread_create(&jobs[i], 0, job, (void*)i);
    }

    //Initializing jobs
    for(long i = 0; i < QTD_PRINTERS; i++){
        pthread_create(&printers[i], 0, printers_printing, (void*)i);
    }

    //Now waiting for them to finish executing
    for (int i = 0; i < QTD_JOBS; i++) {
		pthread_join(jobs[i], 0);
	}
	for (int i = 0; i < QTD_PRINTERS; i++) {
		pthread_join(printers[i], 0);
	}

    return 0;
}