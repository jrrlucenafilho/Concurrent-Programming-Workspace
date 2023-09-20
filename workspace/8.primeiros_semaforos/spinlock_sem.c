#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

//int lock = 0;
sem_t mutex_lock;

int var_global = 0;

#define QTD_THREADS 5
#define QTD_ITERATIONS 3

void* thread_func(void* p) {
	long id = (long)p;
	int counter = 0;
	while (counter < QTD_ITERATIONS) {
		printf("[%ld] Aguardando entrada na secao critica...\n", id);
		//while(__atomic_test_and_set(&lock, 1)); //PROTOCOLO DE ENTRADA
		sem_wait(&mutex_lock); //PROTOCOLO DE ENTRADA
		printf("[%ld] Entrou na secao critica.\n", id);
		int _sleep = rand() % QTD_THREADS;
		sleep(_sleep);
		var_global++;
		printf("[%ld] Job: %d\n", id, var_global);
		//lock = 0; //PROTOCOLO DE SAIDA
		sem_post(&mutex_lock); //PROTOCOLO DE SAIDA
		printf("[%ld] Saiu da secao critica\n", id);
		counter++;
	}

}

int main(void) {
	time_t t;
	pthread_t threads[QTD_THREADS];

	srand(time(&t));

	sem_init(&mutex_lock, 0, 1);

	for (long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], 0, thread_func, (void*)i);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], 0);
	}

	printf("[main] var_global = %d\n", var_global);

	return 0;

}
