#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int randmax = 0;

// Toda funcao para thread deve ter
// retorno de void *
// argumento do tipo void *
void* thread_rand(void* p) {
  int result = rand() % randmax;
  return (void*)result;
}

int proc_rand() {
  int result = rand() % randmax;
  return result;
}

/**
 * int main() {}
 * int main(int argc, char* argv[]) {}
 *
 * argc é um inteiro que guarda a quantidade de argumentos (contando com o
 * executavel do programa) argv é um array de string com os valores dos
 * argumentos
 */
int main(int argc, char** argv) {
  /**
   * DECLARACAO DE VARIAVEL
   */
  clock_t th_start, th_end, pr_start, pr_end;
  double th_cpu_time_used, pr_cpu_time_used;
  time_t t;
  int num;
  pthread_t* threads;
  int* thread_results;
  // Process ID
  int* child_pids;
  int* proc_results;

  srand(time(&t));

  if (argc != 3) {
    printf("usage: %s <NUM THREADS & PROCESSES> <MAX RANDOM VALUE>\n", argv[0]);
    exit(0);
  } else {
    // alpha to integer
    // Converter string para inteiro
    num = atoi(argv[1]);
    randmax = atoi(argv[2]);
    printf("Numero de threads/processos: %d\n", num);
    printf("Max random: %d\n", randmax);
  }

  // THREADS BEGIN
  th_start = clock();
  // memory allocation
  // 5
  // 8
  // qtdElementos * espacoMemoriaDoTipo
  threads = (pthread_t*)malloc(sizeof(pthread_t) * num);
  thread_results = (int*)malloc(sizeof(int) * num);

  for (int i = 0; i < num; i++) {
    pthread_create(&threads[i], 0, thread_rand, NULL);
  }

  int th_soma = 0;

  for (int i = 0; i < num; i++) {
	// O primeiro argumento espera o thread id
	// O segundo argumento espera onde vai ser guardado o retorno da thread
    pthread_join(threads[i], (void**)&thread_results[i]);
    th_soma += thread_results[i];
  }

  float th_media = (float)th_soma / (float)num;
  printf("Media threads: %f\n", th_media);
  th_end = clock();
  th_cpu_time_used = ((double)(th_end - th_start)) / CLOCKS_PER_SEC;
  printf("Tempo threads: %f\n", th_cpu_time_used);
  // THREADS END

  // PROCESS BEGIN
  pr_start = clock();
  child_pids = (int*)malloc(sizeof(int) * num);
  proc_results = (int*)malloc(sizeof(int) * num);
  int original_pid = getpid();

  for (int i = 0; i < num; i++) {
    if (getpid() == original_pid) {
      // PROCESSO ORIGINAL
      child_pids[i] = fork();
      if (child_pids[i] == 0) {
        // PROCESSO CLONADO
        exit(proc_rand());
      }
    }
  }

  int pr_soma = 0;

  // PROCESSO ORIGINAL
  for (int i = 0; i < num; i++) {
	// Primeiro argumento e o pid do processo
	// Ponteiro da variavel que vai receber o retorno
    waitpid(child_pids[i], &proc_results[i], 0);
    pr_soma += WEXITSTATUS(proc_results[i]);
  }

  double pr_media = (double)pr_soma / (double)num;
  printf("Media procs: %f\n", pr_media);
  pr_end = clock();
  pr_cpu_time_used = ((double)(pr_end - pr_start)) / CLOCKS_PER_SEC;
  printf("Tempo procs: %f\n", pr_cpu_time_used);

  // PROCESS END

  return 0;
}
