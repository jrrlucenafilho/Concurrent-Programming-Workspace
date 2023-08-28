#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	fork();
	fork();
	//QUATRO PROCESSOS SERAO GERADOS

	while(1);

	return 0;
}
