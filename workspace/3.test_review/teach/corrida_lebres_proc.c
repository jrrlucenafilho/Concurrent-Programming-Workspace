#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#define TRACK_LEN 1000
#define MAX_JUMP 10
#define QTD 5

void lebre(int id) {
	int total = 0;
	int my_jump = 0;
	time_t t;
	srand(time(&t) + id);
	while (total < TRACK_LEN) {
		my_jump = ((rand() % MAX_JUMP) + 1);
		printf("[%d] pulou: %d (%d)\n", id, my_jump, total);
		total += my_jump;
		if (total >= TRACK_LEN) {
			printf("[%d] VENCEU!\n", id);
			exit(0);
		}
		usleep(50000);
	}
}

/*
Simulador de corrida de lebres
*/

int main(void) {
	int* child_pids;
	int original_pid;

	original_pid = getpid();
	child_pids = (int*)malloc(sizeof(int) * QTD);

	printf("Tamanho pista: %d\n", TRACK_LEN);
	printf("Quantidade de lebres: %d\n", QTD);

	for (int i = 0; i < QTD; i++) {
		child_pids[i] = fork();
		if (child_pids[i] == 0) {
			//PROCESSO CLONADO
			lebre(i);
		}
	}

	int retval;

	wait(&retval);

	for (int i = 0; i < QTD; i++) {
		kill(child_pids[i], SIGHUP);
	}

	return 0;
}


