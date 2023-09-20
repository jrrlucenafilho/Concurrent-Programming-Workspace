#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define QTD_ITERA 10
#define TAM_BUF 10
#define QTD_PROD 3
#define QTD_CONS 6


int buffer[TAM_BUF] = { 0 };
sem_t empty, full;
sem_t mutex_p, mutex_c;
int front, rear;


void* produtor(void* p) {
	long id = (long)p;
	int counter = 0;
	while(counter < QTD_ITERA) {
		printf("[prod-%ld] vou produzir...\n",id);
		int result = (rand() % 100);
		sem_wait(&empty);
		sem_wait(&mutex_p);
		buffer[rear] = result;
		printf("[prod-%ld] produziu: %d\n", id, result);
		rear = ((rear + 1) % TAM_BUF);
		sleep(1);
		sem_post(&mutex_p);
		sem_post(&full);
		counter++;
	}
}

void* consumidor(void* p) {
	long id = (long)p;
        int counter = 0;
        while(counter < QTD_ITERA) {
		printf("[cons-%ld] vou consumir...\n", id);
		sem_wait(&full);
		sem_wait(&mutex_c);
		int result = buffer[front];
		printf("[cons-%ld] consumi: %d\n", id, result);
		front = ((front + 1) % TAM_BUF);
		sem_post(&mutex_c);
		sem_post(&empty);
                counter++;
        }
}

int main(void) {
	pthread_t prod[QTD_PROD], cons[QTD_CONS];
	time_t t;

	srand(time(&t));

	front = 0;
	rear = 0;
	sem_init(&empty, 0, TAM_BUF);
	sem_init(&full, 0, 0);
	sem_init(&mutex_p, 0, 1);
	sem_init(&mutex_c, 0, 1);

	for (long i = 0; i < QTD_PROD; i++) {
		pthread_create(&prod[i], 0, produtor, (void*)i);
	}
	for (long i = 0; i < QTD_CONS; i++) {
		pthread_create(&cons[i], 0, consumidor, (void*)i);
	}

	for (int i = 0; i < QTD_PROD; i++) {
		pthread_join(prod[i], 0);
	}
	for (int i = 0; i < QTD_CONS; i++) {
		pthread_join(cons[i], 0);
	}

	return 0;
}
