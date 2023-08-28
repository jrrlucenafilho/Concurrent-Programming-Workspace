#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	int ret_fork;
	int outra_var = 1000;

	ret_fork = fork();
	
	printf("[%d] %d\n", getpid(), ret_fork);

	if (ret_fork == 0) {
		//PROCESSO CLONADO
		outra_var = 999;
		printf("[%d] outra var = %d\n", getpid(), outra_var);

	} else {
		//PROCESSO ORIGINAL
		outra_var = 1001;
		printf("[%d] outra var = %d\n", getpid(), outra_var);
	}


	return 0;
}
