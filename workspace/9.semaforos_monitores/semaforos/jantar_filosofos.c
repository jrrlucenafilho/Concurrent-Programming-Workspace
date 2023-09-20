#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define QTD_FILOSOFOS 5
#define QTD_GARFOS 5
#define QTD_ITERA 3

sem_t garfos[QTD_GARFOS];

void comer(long id) {
        int _sleep = (rand() % 3);
        sleep(_sleep);
	printf("[filosofo-%ld] comendo...\n", id);
}

void* filosofo(void* p) {
	long id = (long)p;
	int counter = 0;
	int garfo_esq = (int)id;
	int garfo_dir = ((id + 1) % QTD_FILOSOFOS);

	while (counter < QTD_ITERA) {
		long id = (long)p;
		printf("[filosofo-%ld] pensando...\n", id);
		int _sleep = (rand() % 3);
		sleep(_sleep);
		printf("[filosofo-%ld] tenta pegar os garfos...\n", id);
		sem_wait(&garfos[garfo_esq]);
		sem_wait(&garfos[garfo_dir]);
		printf("[filosofo-%ld] pegou os garfos\n", id);
		//capturar os garfos
		comer(id);
		sem_post(&garfos[garfo_esq]);
		sem_post(&garfos[garfo_dir]);
		//libera seus garfos]
		printf("[filosofo-%ld] se alimentou\n", id);
		counter++;
	}
}

int main(void) {
	pthread_t filosofos[QTD_FILOSOFOS];
	time_t t;

	srand(time(&t));

	for (int i = 0; i < QTD_FILOSOFOS; i++) {
		sem_init(&garfos[i], 0, 1);
	}

	for (long i = 0; i < QTD_FILOSOFOS; i++) {
		pthread_create(&filosofos[i], 0, filosofo, (void*)i);
	}

	for (int i = 0; i < QTD_FILOSOFOS; i++) {
		pthread_join(filosofos[i], 0);
	}

}
