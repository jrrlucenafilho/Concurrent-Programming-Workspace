#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void) {
	time_t t;

	srand(time(&t));

	int ret_fork;

	ret_fork = fork();

	if (ret_fork == 0) {
		//PROCESSO CLONADO
		printf("[%d] Gerando valor aleatorio...\n", getpid());
		int r = rand() % 100;
		printf("[%d] Gerei: %d\n", getpid(), r);
		sleep(3);
		exit(r);

	} else {
		//PROCESSO ORIGINAL
		int ret_clone;

		printf("[%d] Aguardando saida do processo %d\n", getpid(), ret_fork);
		waitpid(ret_fork, &ret_clone, 0); //FUNCAO BLOQUEANTE
		printf("[%d] Recebi do processo clonado: %d\n", getpid(), WEXITSTATUS(ret_clone));
	}

	return 0;
}

