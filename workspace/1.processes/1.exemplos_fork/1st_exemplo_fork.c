#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int contador = 0;

int main(void) {
	
	fork();

	int my_pid = getpid();

	while(1) {
		contador++;
		printf("[%d] %d\n", my_pid, contador);
		sleep(1);
	}

	return 0;
}
