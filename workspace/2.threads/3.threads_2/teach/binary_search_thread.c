#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/syscall.h>

/*
	criar duas threads
	- uma thread sera responsavel por buscar um numero em uma lista ordenada
	- outra thread sera responsavel por buscar um outro numero nesta mesma lista
*/

#define QTD_THREADS 10

int list[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

typedef struct {
	int id;
	long search_number;
} thread_data_t;

void* search_list(void* p) {
	thread_data_t* my_data = (thread_data_t*) p;

	int list_size = sizeof(list) / sizeof(list[0]);

	int tid = syscall(__NR_gettid);

	printf("[%d/%d] buscando %ld...\n", my_data->id, tid, my_data->search_number);

	for (int i = 0; i < list_size; i++) {
		if (list[i] == my_data->search_number) {
			printf("[%d/%d] Numero %ld encontrado na posicao: %d\n",
					    my_data->id, tid, my_data->search_number, i);
			return 0;
		}
	}
	printf("[%d/%d] Numero %ld nao encontrado\n",
				my_data->id, tid, my_data->search_number);

	return 0;
}

int main(void) {
	pthread_t threads[QTD_THREADS];
	thread_data_t data[QTD_THREADS];
	time_t t;

	srand(time(&t));

	for (int i = 0; i < QTD_THREADS; i++) {
		data[i].id = i;
		data[i].search_number = rand() % 100;
	}

	printf("Busca binaria com %d threads\n", QTD_THREADS);

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], 0, search_list, (void*) &data[i]);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}
