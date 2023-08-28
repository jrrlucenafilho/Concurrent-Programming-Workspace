#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

/**
 * Create 2 threads
 *  - 1 responsible for searching for a number in an ordered list
 *  - The other one is responsible for searching for another number
 * 
 * //---------------------------------------------------------------
 * random.org: good for generating randomly-filled data structures
*/

#define QTD_THREADS 10

//List needs to be shared between threads (same context / memory)
int list[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

typedef struct{
    int id;
    long search_number;
} thread_data_t;

void* search_list(void* p)
{
    long search_number = (long)p;
    int list_size = sizeof(list) / sizeof(list[0]); //Pointer arithmetic to get size of array
    thread_data_t data[QTD_THREADS];

    time_t t = srand(time(&t));

    for(int i = 0; i < QTD_THREADS; i++){
        data[i].id = i;
        data[i].search_number = rand() % 100; //Random num from 0 to 99
    }
    
    int tid = syscall(__NR_gettid);

    printf("Buscando %ld...\n", search_number);

    for(int i = 0; i < list_size; i++){
        if(list[i] == search_number){
            printf("[%d] Numero %ld encontrado na posicao %d\n", tid, search_number, i);
            return 0;
        }
    }
    printf("[%d] Numero %ld nao encontrado\n", tid, search_number);

    return 0;
}

int main(void)
{
    pthread_t threads[QTD_THREADS];

    long search_numbers[QTD_THREADS] = {5, 25, 44, 34, 4, 6, 77, 73, 4, 76};

    printf("Busca binaria com %d threads\n", QTD_THREADS);

    //Both these for loops are necessary, otherwise it'd be sequential, not concurrent, cuz of add
    for(int i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], 0, search_list, (void*) search_numbers[i]);
    }

    for(int i = 0; i < QTD_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}