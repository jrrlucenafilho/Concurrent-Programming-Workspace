#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

	while(1) {
		printf("[pe] asc: %d meu: %d\n", getppid(),
				getpid());
		sleep(1);
	}

	return 0;
}
