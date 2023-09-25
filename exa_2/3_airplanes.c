#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdbool.h>

#define PLANES_QTD 10

//Semaphore that'll control access to the landing road of an airport
//1 means free, 0 means occupied
//Plane tries to get it, and frees it after use
//Randomly choose if it's gonna land or takeoff
sem_t access_sem;

void* ask_for_permission(void* p)
{
    long id = (long)p;
    bool is_landing;    //true if plane has decided to land. False if it's taking off

    //Airplanes ask for permission to use the road
    printf("Airplane [%ld] asking for permission to use the road...\n", id);

    sem_wait(&access_sem);  //From here on, one single airplane has permitted usage of the road
    printf("Road use permitted for plane [%ld]\n", id);

    //Choosing wether to land or takeoff (randomly choosing a number from 0 to 30, and landing if higher than 17)
    int choice = (rand() % 30);
    if(choice > 17){
        is_landing = true;  //Plane has randomly chosen to land
        printf("Avião [%ld] pousando (Plane landing)\n", id);
    }else{
        is_landing = false; //Plane has randomly chosen to take off
        printf("Avião [%ld] decolando (Plane taking off)\n", id);
    }

    //Sleeping to simulate landing or taking off (random wait timebetween 1s and 5s)
    long sleep_time = (rand() % 5) + 1;
    sleep(sleep_time);

    //Now after waiting (sleeping) for takeoff or landing time, frees up the landing road
    //(semaphore goes back to 1, sinalizing that the road is now free again for another plane to take it)
    sem_post(&access_sem);
}

int main(void)
{
    //Creating specified threads
    pthread_t airplanes[PLANES_QTD];

    sem_init(&access_sem, 0, 1);

    //Init'ing planes' threads, each asking for permission to use the road
    for(long i = 0; i < PLANES_QTD; i++){
        pthread_create(&airplanes[i], 0, ask_for_permission, (void*)i);
    }

    //Joining threads (waiting for planes to land/takeoff on their leisure)
    for(long i = 0; i < PLANES_QTD; i++){
        pthread_join(airplanes[i], 0);
    }
}