#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define QTD_ITERA 30
#define TAM_BUF 10

int buffer[TAM_BUF] = { 0 };
sem_t empty, full;
int front, rear;


void* produtor(void* p) {
	int counter = 0;
	while(counter < QTD_ITERA) {
		printf("[prod] vou produzir...\n");
		int result = (rand() % 100);
		sem_wait(&empty);
		buffer[rear] = result;
		printf("[prod] produziu: %d\n", result);
		rear = ((rear + 1) % TAM_BUF);
		sleep(1);
		sem_post(&full);
		counter++;
	}
}

void* consumidor(void* p) {
        int counter = 0;
        while(counter < QTD_ITERA) {
		printf("[cons] vou consumir...\n");
		sem_wait(&full);
		int result = buffer[front];
		printf("[cons] consumi: %d\n", result);
		front = ((front + 1) % TAM_BUF);
		sem_post(&empty);
                counter++;
        }
}

int main(void) {
	pthread_t prod, cons;
	time_t t;

	srand(time(&t));

	front = 0;
	rear = 0;
	sem_init(&empty, 0, TAM_BUF);
	sem_init(&full, 0, 0);

	pthread_create(&prod, 0, produtor, 0);
	pthread_create(&cons, 0, consumidor, 0);

	pthread_join(prod, 0);
	pthread_join(cons, 0);

	return 0;
}
