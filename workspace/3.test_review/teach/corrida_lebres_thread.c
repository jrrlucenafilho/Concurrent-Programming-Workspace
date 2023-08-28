#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define TRACK_LEN 1000
#define MAX_JUMP 10
#define QTD 5

void* lebre(void* p) {
	long id = (long)p;
	int total = 0;
	int my_jump = 0;
	while (total < TRACK_LEN) {
		my_jump = ((rand() % MAX_JUMP) + 1);
		printf("[%ld] pulou: %d (%d)\n", id, my_jump, total);
		total += my_jump;
		if (total >= TRACK_LEN) {
			printf("[%ld] VENCEU!\n", id);
			exit(0);
		}
		usleep(100000);
	}
}

/*
Simulador de corrida de lebres
*/

int main(void) {
	pthread_t threads[QTD];
	time_t t;

	srand(time(&t));

	printf("Tamanho pista: %d\n", TRACK_LEN);
	printf("Quantidade de lebres: %d\n", QTD);

	for (long i = 0; i < QTD; i++) {
		pthread_create(&threads[i], 0, lebre, (void*)i);
	}

	for (int i = 0; i < QTD; i++) {
		pthread_join(threads[i], 0);
	}

	return 0;
}


