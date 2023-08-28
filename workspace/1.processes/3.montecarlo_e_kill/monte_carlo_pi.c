#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_POINTS 10000000


int monte_carlo_pi(int n) {
	printf("[%d] monte_carlo_pi(%d)\n", getpid(), n);
	unsigned int seed = time(NULL) + getpid();
	int points_inside = 0;


	for (int j = 0; j < n; ++j) {
		double x = (double)rand_r(&seed) / RAND_MAX + 2 - 1;
		double y = (double)rand_r(&seed) / RAND_MAX + 2 - 1 ;
		//printf("[%d] Gerei: %f %f\n", getpid(), x, y);
		x -= 1;
		y -= 1;
		if (x*x + y*y <= 1) {
			points_inside++;
		}
	}
	printf("[%d] pontos dentro: %d\n", getpid(), points_inside);
	return points_inside;
}

int main(int argc, char** argv) {
	int qtd_proc = 0;
	int* child_pids;
	int original_pid;
	int total_points = 0;
	int points_per_process = 0;
	time_t t;

	srand(time(&t));

	//points_per_process = rand() % MAX_POINTS + 1;
	points_per_process = 255;
	//LIMITACAO exit() = 1 byte

	if (argc != 2) {
		printf("Use %s <QTD_PROCESSOS>\n", argv[0]);
		exit(0);
	} else {
		qtd_proc = atoi(argv[1]);
		printf("Criando %d processos...\n", qtd_proc);
		printf("Pontos por processo: %d\n", 
					points_per_process);
		total_points = points_per_process * qtd_proc;
		printf("Total de pontos: %d\n", total_points);
		//ERRO ERA AQUI, TOTAL POINTS TAVA ZERO
	}

	original_pid = getpid();
	child_pids = (int*) malloc(sizeof(int) * qtd_proc);

	for (int i = 0; i < qtd_proc; i++) {
		if (original_pid == getpid()) {
			child_pids[i] = fork();
			if (getpid() != original_pid) {
				//free(child_pids);
				break;
			} else {
				printf("Criei %d\n", child_pids[i]);
			}
		}
	}

	if (getpid() != original_pid) {
		printf("%d calculando pontos...\n", getpid());
		int result = 
			monte_carlo_pi(points_per_process);
		exit(result);
	}
	if (getpid() == original_pid) {
		int* child_results;
		int total_points_inside = 0;

		child_results = (int*) 
			malloc(sizeof(int) * qtd_proc);

		for (int i = 0; i < qtd_proc; i++) {
			waitpid(child_pids[i],
				&child_results[i], 0);
			total_points_inside += 
				WEXITSTATUS(child_results[i]);
		}

		printf("Total de pontos: %d\n", total_points);
		printf("Total de pontos dentro: %d\n", 
				total_points_inside);

		double estimated_pi = 
		4.0 * ((double)total_points_inside / 
				(double)total_points);
		printf("Valor estimado de pi: %lf\n", 
				estimated_pi);
		free(child_pids);
		free(child_results);
	}


	return 0;
}
