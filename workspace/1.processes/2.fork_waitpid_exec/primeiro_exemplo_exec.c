#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
	printf("Programa original - antes do exec()\n");
	printf("meu pid: %d\n", getpid());
	char *args[]={"./prog-externo",NULL};
	if (fork() == 0) {
	        execvp(args[0],args);
	}

	while(1) {
		printf("prog-original!\n");
		sleep(1);
	}
	

	return 0;
}
