#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define QTD_THREADS 5

int requests = 0;
pthread_mutex_t mutex;

void* process_request(void* p) {
	long id = (long)p;
	int _sleep = 0;
	int reqs = 0;
	reqs = (rand() % 3) + 1;
	printf("[%ld] requisicoes recebidas: %d\n", id, reqs);
	for (int i = 0; i < reqs; i++) {
		pthread_mutex_lock(&mutex);
		printf("[%ld] requisicao em processamento... (%d)\n", id, requests);
		_sleep = (rand() % 5) + 1;
		printf("[%ld] time: %d\n", id, _sleep);
		sleep(_sleep);
		requests++;
		pthread_mutex_unlock(&mutex);
		printf("[%ld] requisicao processada (%d)\n", id, requests);
	}
}

int main(void) {
	time_t t;
	pthread_t threads[QTD_THREADS];

	srand(time(&t));

	pthread_mutex_init(&mutex, NULL);

	for (long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], 0, process_request, (void*)i);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], 0);
	}

	printf("Total requests: %d\n", requests);

	pthread_mutex_destroy(&mutex);

	return 0;
}
