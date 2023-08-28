#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/syscall.h>

#define MAX_POINTS 1000000

void* monte_carlo_pi(void* p) {
	long n = (long)p;

	unsigned int seed = time(NULL) + getpid();
	long points_inside = 0;
	pid_t tid = syscall(__NR_gettid);

	printf("[%d] monte_carlo_pi(%ld)\n", tid, n);

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
	printf("[%d] pontos dentro: %ld\n", tid, points_inside);
	return (void*)points_inside;
}

int main(int argc, char** argv) {
	int qtd_threads = 0;
	pthread_t* threads;

	long total_points = 0;
	long points_per_thread = 0;
	long total_points_inside = 0;

	long* thread_results;

	time_t t;

	srand(time(&t));

	points_per_thread = rand() % MAX_POINTS + 1;

	if (argc != 2) {
		printf("Use %s <QTD_THREADS>\n", argv[0]);
		exit(0);
	} else {
		qtd_threads = atoi(argv[1]);
		printf("Criando %d threads...\n", qtd_threads);
		printf("Pontos por thread: %ld\n",
					points_per_thread);
		total_points = points_per_thread * qtd_threads;
		printf("Total de pontos: %ld\n", total_points);
	}

	threads = (pthread_t*) malloc(sizeof(pthread_t) * qtd_threads);
	thread_results = (long*) malloc(sizeof(long) * qtd_threads);

	for (int i = 0; i < qtd_threads; i++) {
		pthread_create(&threads[i], 0, monte_carlo_pi, (void*) points_per_thread);
		printf("Criei thread: %ld\n", threads[i]);
	}

	for (int i = 0; i < qtd_threads; i++) {
		pthread_join(threads[i], (void**)&thread_results[i]);
		total_points_inside += thread_results[i];
	}


	printf("Total de pontos: %ld\n", total_points);
	printf("Total de pontos dentro: %ld\n",
				total_points_inside);

	double estimated_pi =
		4.0 * ((double)total_points_inside /
				(double)total_points);
	printf("Valor estimado de pi: %lf\n",
				estimated_pi);
	free(threads);
	free(thread_results);

	return 0;
}
