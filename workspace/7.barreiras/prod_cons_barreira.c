#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define BUFFER_SIZE 10
#define NUM_PROD 1
#define NUM_CONS 1

pthread_barrier_t barreira;
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

void* producer(void* p) {
	long id = (long)p;
	while (1) {
		int item = (rand() % 100);
		buffer[in] = item;
		pthread_barrier_wait(&barreira);
		in = (in + 1) % BUFFER_SIZE;
		printf("[prod-%ld] produziu: %d\n", id, item);
		sleep(1);
	}
}

void* consumer(void* p) {
	long id = (long)p;
	int item = 0;
	while (1) {
		pthread_barrier_wait(&barreira);
		item = buffer[out];
		out = (out + 1) % BUFFER_SIZE;
		printf("[cons-%ld] consumiu: %d\n", id, item);
		sleep(1);
	}
}

int main(void) {
	pthread_t producers[NUM_PROD];
	pthread_t consumers[NUM_CONS];
	time_t t;

	srand(time(&t));

	pthread_barrier_init(&barreira, NULL, (NUM_PROD+NUM_CONS));

	for (long i = 0; i < NUM_PROD; i++) {
		pthread_create(&producers[i], NULL, producer, (void*)i);
	}

	for (long i = 0; i < NUM_CONS; i++) {
		pthread_create(&consumers[i], NULL, consumer, (void*)i);
	}

	for (int i = 0; i < NUM_PROD; i++) {
		pthread_join(producers[i], 0);
	}

	for (int i = 0; i < NUM_CONS; i++) {
		pthread_join(consumers[i], 0);
	}

	pthread_barrier_destroy(&barreira);

	return 0;
}
