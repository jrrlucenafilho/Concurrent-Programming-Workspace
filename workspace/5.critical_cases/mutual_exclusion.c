#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 5

typedef struct { 
    float position;
    float vel;
    float time;
} t_desc;

t_desc main_desc;

pthread_mutex_t mtx;
//All mutex ops we're gonna use are atomic at heart
//Gotta create this as a global var

int lock;

int process(t_desc desc)
{
    float s = desc.position + (desc.vel * desc.time);
    
    return s;
}

void* thread_func(void* p)
{
    long id = (long)p;
    while(__atomic_test_and_set(&lock, 1));
    //pthread_mutex_lock(&mtx);   //Blocking protocol
    //CRITICAL SESSION BEGIN
    main_desc.vel = id;
    main_desc.time = (id * 10) + 1;
    sleep((int)id);
    printf("[%f] process start! (%f, %f, %f)\n", main_desc.position, main_desc.vel, main_desc.time);

    float res = process(main_desc);
    main_desc.position = res;
    //CRITICAL SEESION END
    //pthread_mutex_unlock(&mtx); //Unblocking protocol
    //Gotta block the all other threads while doing this to kill inconsistencies (non-determinism)
    //Thread order may be different, but final result will be consistent
    //But if i want to use mutex withou using the mutex library, i can use a whil(true) loop with a 
    //shared var (but it isn't atomic so blerhg)
    //Instead: use the test and set logic from atomic
    //Usually we'll be using the mutex lib for pthreads (lik 99% of times), cuse the overhead is usually insignificant
    //BUT, in extreme cases like alarm systems, it'll be better to use occupied_waiting "with __atomic_test_and_set(&lock, 1)" (rare)

    printf("[%f] res = %f (%f, %f, %f)\n", res, main_desc.position, main_desc.vel, main_desc.time);
}

int main(void)
{
    pthread_t threads[QTD_THREADS];
    pthread_mutex_init(&mtx, 0); //Do this to be able to use it here

    for(int i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], 0, thread_func, 0);
    }

    for(int i = 0; i < QTD_THREADS; i++){
        pthread_join(&threads[i], 0);
    }
}