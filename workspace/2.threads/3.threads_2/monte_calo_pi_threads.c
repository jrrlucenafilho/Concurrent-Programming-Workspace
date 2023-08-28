//Monte Carlo method for pi, but now using threads!

/**
 * We can pass args thread funcs, as long as they're passed at the moment their threads ara initialized.
 * 
 * Every var that's initialized in the main thread WILL be on each thread. BUT will not be accessible
 * to one another, so you've got to put as global vars on as params to the thread funcs
 * 
 * An qeuivalent to getpi() for threads coms from sys/syscall: syscall(__NR_gettid);
 * 
 * Threads are easier on the pc rsrcs than using processes, once again
 * 
 * UNFINISHED
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/syscall.h>

#define MAX_POINTS 10000000

//Changed from ints to void* ing args and return type, to be able to use threads
void* monte_carlo_pi(void* p)
{
    long n = (long)p;
	unsigned int seed = time(NULL) + getpid();
	long points_inside = 0;
    pid_t tid = syscall(__NR_gettid);

    printf("[%d] monte_carlo_pi"); //unfinished here

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
	printf("[thread] pontos dentro: %ld\n", points_inside);
	return (void*)points_inside;
}

int main(int argc, char** argv)
{
	int qtd_threads = 0;
	pthread_t* threads;
	long total_points = 0;
	long points_per_thread = 0;
    long* thread_results;
    long total_points_inside = 0;

	time_t t;

	srand(time(&t));

	points_per_thread = rand() % MAX_POINTS + 1;
	//points_per_thread = 255;
	//LIMITACAO exit() = 1 byte

	if (argc != 2) {
		printf("Use %s <QTD_THREADS>\n", argv[0]);
		exit(0);
	} else {
		qtd_threads = atoi(argv[1]);
		printf("Criando %ld threads...\n", qtd_threads);
		printf("Pontos por thread: %ld\n", 
					points_per_thread);
		total_points = points_per_thread * qtd_threads;
		printf("Total de pontos: %ld\n", total_points);
		//ERRO ERA AQUI, TOTAL POINTS TAVA ZERO
	}

    threads = (pthread_t*) malloc(sizeof(pthread_t) * qtd_threads); //Dynamic allocs of arrays for C
    thread_results = (long*) malloc(sizeof(long) * qtd_threads);

    for(int i = 0; i < qtd_threads; i++){
        pthread_create(&threads[i], 0, monte_carlo_pi, (void*) points_per_thread);
        printf("Criei thread %ld\n", threads[i]);
    }

    for(int i = 0; i < qtd_threads; i++){
        //Args: thread itself, return value
        pthread_join(threads[i], (void**) &thread_results[i]);
        total_points_inside += thread_results[i]; //Add
    }

    printf("Total de pontos: %ld\n", total_points);

	return 0;
}
