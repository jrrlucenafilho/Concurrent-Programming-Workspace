#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define QTD_THREADS 5

int number = 0;
int next = 0;
int turn[QTD_THREADS] = { 0 };

int requests = 0;

/*

//ALGORITMO DO TICKET - SOLUCAO JUSTA

int number = 1;
int next = 1;
int turn[N] = {0};

process CS [i = 1 to N ] {
        while(true) {
                <turn[i] = number; number++;>
                <await (turn[i] == next);> //PROTOCOLO DE ENTRADA
                //SECAO CRITICA
                <next++;> //PROTOCOLO DE SAIDA
                //SECAO NAO CRITICA
        }
}

*/

//__atomic_fetch_add(&var, 1, __ATOMIC_ACQ_REL);

void* process_request(void* p) {
	long id = (long)p;
	int _sleep = (rand() % 5) + 1;
	printf("[%ld] recebeu request... (sleep: %d, r: %d)\n", id, _sleep, requests);
	turn[id] = __atomic_fetch_add(&number, 1, 0); //PROT. ENTRADA
	while (turn[id] != next); //ESPERA OCUPADA (PROT. ENTRADA)
	//SECAO CRITICA
	printf("[%ld] processando request...\n", id);
	sleep(_sleep);
	requests++;
	//SECAO CRITICA
	printf("[%ld] processou request: %d\n", id, requests);
	__atomic_fetch_add(&next, 1, 0);
}
int main(void) {
	pthread_t threads[QTD_THREADS];
	time_t t;

	srand(time(&t));

	for (long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], 0, process_request, (void*)i);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Total requests: %d\n", requests);

	return 0;
}

