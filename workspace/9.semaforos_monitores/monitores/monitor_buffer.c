#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define TAM_BUFFER 10
#define MULT 3

typedef struct {
	int buffer[TAM_BUFFER];
	int indice_insercao;
	int indice_remocao;
	pthread_mutex_t mutex;
	sem_t buffer_cheio;
	sem_t buffer_vazio;
} t_monitor;

void inicializar(t_monitor* m) {
	m->indice_insercao = 0;
	m->indice_remocao = 0;
	pthread_mutex_init(&m->mutex, NULL);
	sem_init(&m->buffer_cheio, 0, 0);
	sem_init(&m->buffer_vazio, 0, TAM_BUFFER);
}

void inserir_item(t_monitor* m, int item) {
	pthread_mutex_lock(&m->mutex);
	sem_wait(&m->buffer_vazio);
	m->buffer[m->indice_insercao] = item;
	m->indice_insercao = (m->indice_insercao + 1) % TAM_BUFFER;
	sem_post(&m->buffer_cheio);
	pthread_mutex_unlock(&m->mutex);
}

int remover_item(t_monitor* m) {
	sem_wait(&m->buffer_cheio);
	pthread_mutex_lock(&m->mutex);
	int item = m->buffer[m->indice_remocao];
	m->indice_remocao = (m->indice_remocao + 1) % TAM_BUFFER;
	pthread_mutex_unlock(&m->mutex);
	sem_post(&m->buffer_vazio);
	return item;
}

void* produtor(void* p) {
	t_monitor* mon = (t_monitor*)p;

	for (int i = 0; i < TAM_BUFFER*MULT; i++) {
		printf("produtor produziu: %d\n", (i+1));
		inserir_item(mon, (i+1));
		sleep(1);
	}

	return 0;

}

void* consumidor(void* p) {
	t_monitor* mon = (t_monitor*)p;

	for (int i = 0; i < TAM_BUFFER*MULT; i++) {
		int item = remover_item(mon);
		printf("consumidor consumiu: %d\n", item);
		sleep(1);
	}

}

int main(void) {
	t_monitor m;
	pthread_t t_prod, t_cons;

	inicializar(&m);

	pthread_create(&t_prod, 0, produtor, (void*)&m);
	pthread_create(&t_cons, 0, consumidor, (void*)&m);

	pthread_join(t_prod, 0);
	pthread_join(t_cons, 0);


	return 0;
}
