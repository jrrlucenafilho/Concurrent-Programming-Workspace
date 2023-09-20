#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 5
#define JOB_COUNT 3

pthread_barrier_t barreira;

void* job(void* p) {
	int count = 0;
	while (count < JOB_COUNT) {
		long id = (long)p;
		printf("[%ld] job received\n", id);
		int _sleep = (int)(id + 1);
		sleep(_sleep);
		printf("[%ld] job done!\n", id);
		pthread_barrier_wait(&barreira);
		printf("[%ld] passed the barrier\n",id);
		count++;
	}
}

int main(void) {
	pthread_t threads[QTD_THREADS];

	pthread_barrier_init(&barreira, 0, QTD_THREADS);

	for (long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], 0, job, (void*)i);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], 0);
	}


	return 0;
}
