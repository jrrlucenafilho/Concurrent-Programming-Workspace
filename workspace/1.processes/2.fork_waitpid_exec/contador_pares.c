#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define QTD_PROC 10

int count_even(char* filename) {
	FILE *fd;
	char ch;
	int count = 0;

	fd = fopen(filename, "r");

	if (fd == NULL) {
		fprintf(stderr, "Error loading file");
		return -1;
	}

	while((ch = fgetc(fd))!=EOF) {
		if (ch == '\n') {
			continue;
		} else {
			int n = ch - 48;
			if ((n % 2) == 0) count++;
		}
	}

	fclose(fd);

	return count;
}

int main() {
	int pid_clones[QTD_PROC];
	int res_clones[QTD_PROC];

	int pid_original = getpid();
	int clone_id;
	int count = 0;

	for (int i = 0; i < QTD_PROC; i++) {
		if (getpid() == pid_original) {
			pid_clones[i] = fork();
			if (getpid() != pid_original) {
				//CLONE
				clone_id = i;
				break;
			}
		}
	}
	
	if (getpid() != pid_original) {
		//CLONADO
		char filename[8];
		int result = 0;
		sprintf(filename, "%d.txt", clone_id);
		result = count_even(filename);
		printf("[%d] count = %d\n", clone_id, result);
		sleep(clone_id+1);
		exit(result);
	}

	if (getpid() == pid_original) {
		//ORIGINAL
		for (int i = 0; i < QTD_PROC; i++) {
			waitpid(pid_clones[i], 
					&res_clones[i], 0);
			count += WEXITSTATUS(res_clones[i]);
		}

		printf("Quantidade de pares: %d\n", count);
	}

	return 0;
}
