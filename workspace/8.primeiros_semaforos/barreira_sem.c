#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define QTD_ITERATIONS 3

sem_t arrive1;
sem_t arrive2;


void* job1(void* p) {
	int counter = 0;
	while(counter < QTD_ITERATIONS) {
		//JOB
		int _sleep = (rand() % 5) + 1;
		printf("[t1] job() - %d\n", _sleep);
		sleep(_sleep);
		printf("[t1] barrier\n");
		sem_post(&arrive1);
		sem_wait(&arrive2);
		printf("[t1] passou da barreira (%d)\n", counter);
		//BARRIER
		counter++;
	}

}

void* job2(void* p) {
        int counter = 0;
        while(counter < QTD_ITERATIONS) {
		//JOB
		int _sleep = (rand() % 5) + 1;
		printf("[t2] job() - %d\n", _sleep);
		sleep(_sleep);
		printf("[t2] barrier\n");
		sem_post(&arrive2);
		sem_wait(&arrive1);
		printf("[t2] passou da barreira (%d)\n", counter);
		//BARRIER
                counter++;
        }
}

int main(void) {
	pthread_t thread1, thread2;
	time_t t;

	sem_init(&arrive1, 0, 0);
	sem_init(&arrive2, 0, 0);

	srand(time(&t));

	pthread_create(&thread1, 0, job1, 0);
	pthread_create(&thread2, 0, job2, 0);

	pthread_join(thread1, 0);
	pthread_join(thread2, 0);

	return 0;
}
