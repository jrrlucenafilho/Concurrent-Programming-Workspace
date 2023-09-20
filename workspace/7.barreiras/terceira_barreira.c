#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 5
#define JOB_COUNT 3

int chegada[QTD_THREADS] = { 0 };
int saida[QTD_THREADS] = { 0 };

void* coord(void* p) {
	int count = 0;
	while (count < JOB_COUNT) {
		for (int i = 0; i < QTD_THREADS; i++) {
			while (chegada[i] != 1);
		}
		for (int i = 0; i < QTD_THREADS; i++) {
			chegada[i] = 0;
			saida[i] = 1;
		}
		count++;
	}
}

void* job(void* p) {
	int count = 0;
	while (count < JOB_COUNT) {
		long id = (long)p;
		printf("[%ld] job received\n", id);
		int _sleep = (int)(id + 1);
		sleep(_sleep);
		printf("[%ld] job done!\n", id);
		chegada[id] = 1;
		while (saida[id] != 1);
		printf("[%ld] passed the barrier\n",id);
		saida[id] = 0;
		count++;
	}
}

int main(void) {
	pthread_t threads[QTD_THREADS];
	pthread_t coord_thread;

	for (long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], 0, job, (void*)i);
	}

	pthread_create(&coord_thread, 0, coord, 0);

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], 0);
	}

	pthread_join(coord_thread, 0);

	return 0;
}
