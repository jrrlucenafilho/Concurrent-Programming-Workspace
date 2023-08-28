#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 1000
#define COUNT_LIMIT 1000

int shared_var = 0;
pthread_mutex_t m;

void* thread_func(void* p) {
	long id = (long)p;

//	printf("[%ld] Thread iniciada!\n", id);
	sleep(1);
	for (int i = 0; i < COUNT_LIMIT; i++) {

		pthread_mutex_lock(&m); //protocolo de entrada
		shared_var++; //secao critica
		pthread_mutex_unlock(&m); //protocolo de saida

		//__atomic_fetch_add(&shared_var, 1, 0); //incremento atomico
	}

	printf("[%ld] shared_var = %d\n",id, shared_var);
//	printf("[%ld] Thread finalizada\n", id);
	return (void*)(id+1);
}

int main(void) {
	pthread_t threads[QTD_THREADS];
	int thread_results[QTD_THREADS];

	for (long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], 0, thread_func, (void*)i);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], &thread_results[i]);
//		printf("[main] recebi %d da thread #%d\n",thread_results[i],i);
	}

	printf("[main] shared_var = %d\n", shared_var);

	return 0;
}
