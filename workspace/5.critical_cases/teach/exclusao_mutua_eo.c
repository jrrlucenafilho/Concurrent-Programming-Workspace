#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 5

typedef struct {
	float pos;
	float vel;
	float time;
} t_desc;

t_desc main_desc;

int lock = 0;

int process(t_desc desc) {
	float s = desc.pos + (desc.vel * desc.time);
	return s;
}

void* thread_func(void* p) {
	long id = (long)p;
	while(__atomic_test_and_set(&lock, 1)); //PROTOCOLO DE ENTRADA
	//SECAO CRITICA
	main_desc.vel = id;
	main_desc.time = (id*10)+1;
	sleep((int)id);
	printf("[%ld] process start (%f, %f, %f)\n",
			id, main_desc.pos, main_desc.vel, main_desc.time);

	float res = process(main_desc);
	main_desc.pos = res;
	//SECAO CRITICA
	lock = 0;	//PROTOCOLO DE SAIDA
	printf("[%ld] res = %f (%f %f %f)\n",
			id, res, main_desc.pos, main_desc.vel, main_desc.time);
}

int main(void) {
	pthread_t threads[QTD_THREADS];

	for(long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], 0, thread_func, (void*)i);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], 0);
	}

	printf("(main) exiting...\n");

}
