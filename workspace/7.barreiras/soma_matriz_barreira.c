#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

/* calculo da soma de uma matriz grande */

/* cada thread deve calcular a soma de uma
 * linha da matriz no final de cada iteracao,
 * todas as threads devem esperar pelas outras
 * usando uma barreira de posix threads */

#define QTD_THREADS 4
#define MATRIX_SIZE 4
#define LOOP_COUNT 3

int matrix[MATRIX_SIZE][MATRIX_SIZE];
int partial_sum[QTD_THREADS];
pthread_barrier_t barreira_soma;
pthread_barrier_t barreira_total;

void* matrix_sum(void* p) {
	long id = (long)p;
	int sum = 0;
	printf("[%ld] recebeu job!\n", id);

	for (int i = 0; i < MATRIX_SIZE; i++) {
		sum += matrix[id][i];
	}
	sleep((int)id);
	partial_sum[id] = sum;
	printf("[%ld] terminou job! (%d)\n", id, sum);
	pthread_barrier_wait(&barreira_soma);
	printf("[%ld] passou da barreira!\n", id);
	pthread_barrier_wait(&barreira_total);
}

int main(void) {
	pthread_t threads[QTD_THREADS];
	time_t t;
	int loop_count = 0;

	srand(time(&t));

	pthread_barrier_init(&barreira_soma, 0, QTD_THREADS+1); //+1 = thread main
	pthread_barrier_init(&barreira_total, 0, QTD_THREADS+1);

	while(loop_count != LOOP_COUNT) {

		for (int i = 0; i < MATRIX_SIZE; i++) {
			for (int j = 0; j < MATRIX_SIZE; j++) {
				matrix[i][j] = (rand() % 100);
				printf("matrix[%d][%d] = %d\n", i, j, matrix[i][j]);
			}
		}

		printf("Matriz populada! Iniciando soma...\n");

		for (long i = 0; i < QTD_THREADS; i++) {
			pthread_create(&threads[i], 0, matrix_sum, (void*)i);
		}

		pthread_barrier_wait(&barreira_soma);

		printf("[main] realizando soma total...\n");
		int total = 0;

		for (int i = 0; i < QTD_THREADS; i++) {
			total += partial_sum[i];
		}

		printf("Valor total da soma da matriz: %d\n", total);

		pthread_barrier_wait(&barreira_total);

		loop_count++;
	}

	return 0;
}
