#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

#define EXE_LIMIT 10

typedef struct {
	float temp;
	float pres;

} t_status;

t_status status;

void* execute(void* p) {
	printf("(E) execute() - start\n");
	printf("(E) considering %f - %f\n", status.temp, status.pres);
	float exe_param = 0;
	for (int i = 0; i < EXE_LIMIT; i++) {
		exe_param = (status.temp * 10) + (status.pres * status.pres);
		sleep(1);
		printf("(E) exe_param = %f (%f,%f)\n", exe_param,
					status.temp, status.pres);
	}
	sleep(2);
}

void* manage(void* p) {
	printf("(M) manage() - start\n");
	printf("(M) considering %f - %f\n", status.temp, status.pres);
	for (int i = 0; i < EXE_LIMIT; i++) {
		float t = status.temp;
		float p = status.pres;
		t = t + 10.5;
		status.temp = t;
		sleep(1);
		p = p + 0.01;
		status.pres = p;
		printf("(M) changed: %f %f\n", status.temp, status.pres);
		sleep(1);
	}
	sleep(2);
}

int main(void) {
	pthread_t thread_exe, thread_man1, thread_man2;

	status.temp = 100;
	status.pres = 1;

	pthread_create(&thread_exe, 0, execute, 0);
	pthread_create(&thread_man1, 0, manage, 0);
	pthread_create(&thread_man2, 0, manage, 0);

	pthread_join(thread_exe, 0);
	pthread_join(thread_man1, 0);
	pthread_join(thread_man2, 0);

	printf("(main) saindo...\n");




}
