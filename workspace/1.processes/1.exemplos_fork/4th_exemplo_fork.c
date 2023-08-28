#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {

	int qtd_proc = 0;
	int pid_original = 0;

	pid_original = getpid();

	printf("Quantos processos deseja criar: ");
	scanf("%d", &qtd_proc);

	for (int i = 0; i < qtd_proc; i++) {
		if (getpid() == pid_original) {
			fork();
		} else {
			break;
		}
	}

	if (getpid() != pid_original) {
		printf("[%d] processo criado!\n", getpid());

		while(1);
	} else {
		printf("[%d] saindo...\n", getpid());
	}
	
	return 0;
}
