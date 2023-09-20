#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 5
#define JOB_COUNT 3

int barrier_count;

void* job(void* p) {
	int count = 0;
	while (count < JOB_COUNT) {
		long id = (long)p;
		printf("[%ld] job received\n", id);
		int _sleep = (int)(id + 1);
		sleep(_sleep);
		printf("[%ld] job done! (%d)\n", id, barrier_count);
		__atomic_fetch_add(&barrier_count, 1, 0);
		while (barrier_count != QTD_THREADS);
		printf("[%ld] passed the barrier (%d)\n",id, barrier_count);
		if (id == 0)
			barrier_count = 0;
		else
			while (barrier_count != 0);

		count++;
	}
}

int main(void) {
	pthread_t threads[QTD_THREADS];

	barrier_count = 0;

	for (long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], 0, job, (void*)i);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], 0);
	}


	return 0;
}
