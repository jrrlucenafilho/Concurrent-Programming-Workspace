#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define QTD_CORREDORES 5
#define QTD_VOLTAS 4

pthread_barrier_t barreira;

void* correr(void* p) {
	long id = (long)p;
	int counter = 0;
	while (counter < QTD_VOLTAS) {
		printf("Corredor %ld inicia corrida...\n", id);
		int _sleep = (rand() % 5) + 1;
		printf("Corredor %ld correndo (%d)...\n", id, _sleep);
		sleep(_sleep);
		printf("Corredor %ld chegou na barreira...\n", id);
		pthread_barrier_wait(&barreira);
		counter++;
		printf("Corredor %ld passou da barreira! (%d)\n", id, counter);
	}
}

int main(void) {
	pthread_t corredores[QTD_CORREDORES];
	time_t t;

	srand(time(&t));

	printf("Corrida com %d voltas\n", QTD_VOLTAS);

	pthread_barrier_init(&barreira, NULL, QTD_CORREDORES);

	for (long i = 0; i < QTD_CORREDORES; i++) {
		pthread_create(&corredores[i], 0, correr, (void*)i);
	}

	for (int i = 0; i < QTD_CORREDORES; i++) {
		pthread_join(corredores[i], 0);
	}

	printf("Fim da corrida!\n");

	pthread_barrier_destroy(&barreira);

	return 0;
}
