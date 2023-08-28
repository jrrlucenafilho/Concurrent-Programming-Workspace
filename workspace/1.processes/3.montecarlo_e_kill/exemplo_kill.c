#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#define QTD_PROC 10

void random_sleep() {
	time_t t;
	srand((time(&t) + getpid()));
	//int sleep_time = rand_r((time(&t) + i)) % 16;
	int sleep_time = rand() % 16;
	//sleep_time += (5 + i);
	printf("[%d] vou dormir %d...\n", getpid(), sleep_time);
	sleep(sleep_time);
}

int main(void) {
	int original_pid;
	int child_pids[QTD_PROC];
	int ccounter = -1;

	original_pid = getpid();

	for (int i = 0; i < QTD_PROC; i++) {
		if (getpid() == original_pid) {
			ccounter++;
			child_pids[ccounter] = fork();
			if (getpid() != original_pid) {
				break;
			} else {
				printf("[orig] criou %d\n",
				child_pids[ccounter]);
			}
		}
	}

	if (original_pid  == getpid()) {
		sleep(10);
		for (int i = 0; i < QTD_PROC; i++) {
			printf("[orig] finalizando %d\n",
					child_pids[i]);
			kill(child_pids[i], SIGHUP);
		}
	} else {
		random_sleep(ccounter);
		printf("[%d] saindo...\n", getpid());
		exit(0);
	}


	return 0;
}

