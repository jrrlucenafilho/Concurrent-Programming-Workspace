#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> //We'll learn how these go differently in cpp later

//Shared vars (global) only NOW change between threads
//(if i change it on a thread, it'll be chnaged on another one
//which is good, but causes some problems)
int shared_var = 0;

#define QTD_THREADS 10

//In C, this function NEEDS to return "void*" and can only receive ONE void* arg (not in cpp)
void* thread_func(void* p)
{
    long id = (long)p;

    printf("[%ld] Thread Initialized\n", id);
    sleep(id+1);
    printf("[%ld] Thread Finalized\n", id);
    
    //This exit() would kill everything
    //exit(0);
    return 0;
}

int main(void)
{
    pthread_t threads[QTD_THREADS];

    for(long i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], 0, thread_func, (void*)i);  //tid = thread id (not as used as pid)
    }

    sleep(10);
    //Even with this sleep(), we couldn't see the last thread being finalizea, we'll have to use an equivalent
    //to waitpid()

    return 0;
}