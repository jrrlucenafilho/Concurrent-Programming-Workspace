#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define QTD_ITERA 5

int buffer;

sem_t empty, full;

void* produtor(void* p) {
	int counter = 0;
	while (counter < QTD_ITERA) {
		printf("[prod] vai produzir...\n");
		int r = (rand() % 100) + 1;
		sem_wait(&empty);
		buffer = r;
		printf("[prod] produziu: %d\n", r);
		sleep(1);
		sem_post(&full);
		counter++;
	}
}

void* consumidor(void* p) {
	int counter = 0;
        while (counter < QTD_ITERA) {
		printf("[cons] vai consumir...\n");
		sem_wait(&full);
		int result = buffer;
		printf("[cons] consumiu: %d\n", result);
		sem_post(&empty);
                counter++;
        }
}

int main(void) {
	pthread_t cons, prod;
	time_t t;

	srand(time(&t));

	sem_init(&empty, 0 , 1); //empty = 1
	sem_init(&full, 0, 0);  //full = 0

	pthread_create(&cons, 0, consumidor, 0);
	pthread_create(&prod, 0, produtor, 0);

	pthread_join(cons, 0);
	pthread_join(prod, 0);

	return 0;
}

