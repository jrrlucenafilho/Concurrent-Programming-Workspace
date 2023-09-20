#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define QTD_THREADS 5

typedef struct {
	int contador;
	pthread_mutex_t mutex;
} t_monitor;

void mon_inicializa(t_monitor* m) {
	m->contador = 0;
	pthread_mutex_init(&m->mutex, NULL);
}

void mon_incrementa(t_monitor* m) {
	pthread_mutex_lock(&m->mutex);
	m->contador++;
	pthread_mutex_unlock(&m->mutex);
}

int mon_get_valor(t_monitor* m) {
	pthread_mutex_lock(&m->mutex);
	int result = m->contador;
	pthread_mutex_unlock(&m->mutex);
	return result;
}

void mon_finaliza(t_monitor* m) {
	pthread_mutex_destroy(&m->mutex);
}

void* thread_func(void* p) {
	t_monitor* mon = (t_monitor*)p;

	int qtd = (rand() % 100) + 1;
	printf("[thread] incrementa: %d\n", qtd);

	for (int i = 0; i < qtd; i++) {
		mon_incrementa(mon);
		usleep(10000);
	}
}

int main(void) {
	pthread_t threads[QTD_THREADS];
	t_monitor monitor;
        time_t t;

        srand(time(&t));
	mon_inicializa(&monitor);

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], NULL, thread_func, (void*)&monitor);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Valor final do contador do monitor: %d\n",
						mon_get_valor(&monitor));


	return 0;

}
