/**
 * This one creates 10 threads in an array and finishes them.
 * 
 * Thread identifiers aren't often used, since we'll be using vars to refer to threads and manip them.
 * 
 * Once the main() thread ends, it's entire context gets freed from memory, it also means all threads
 * derived from main() will die as well.
 * 
 * Thread finalization will return stuff (much higher information-density, like returning structs and passing them
 * to threads, is possible now with this)
 * 
 * pthread_join() is equivalent to wait() for processes, but being for threads
 * 
 * Problem fits: Not all problems benefit from parallelism, communication time may make it worse than if it was procedural.
 * Obs: If the operations have interdependence between 2 or more instructions, it'll be better to run this set
 * of instrs. in the same thread
 * 
 * Global vars over here share the same context (mem) among different threads, unlike in processes
 * 
 * !!: Incrementing with "++" is NOT an atomic instruction in posix-compliant systems!!!
 * So if we use that without care, it goes to asm as a comb. of "LOAD ADD STORE", but we can lose the 
 * proper incrementation
 * 
 * "atomic" means it translates to a single instruction down-in-the-assembly, which makes it so
 * the scheduler doesn't really have to care about it. Thus, not adding non-determinism.
 * 
 * Turns out that we can make it so that a section of code disables (temporarily) concurrency
 * And as such, once one of the threads reach this block of code, all other threads get blocked and wait
 * until that one thread finishes that code section
 * It's name is "mutex", menaning "mutual exclusion"
 * 
 * The cost of instantiating a thread is lower than instatiating a process
 * (and it's easier to communicate between them).
 * But, like in google chrome, processes have it's uses where they're better
 * 
 * Occupied wait: uses a heavy while loop to test continuously
 * 
 * END: Manipulating global vars is easy for reading-only, but as for writing and changing them: we'll have to follow
 * specific input and output protocols (we won't only be incrementing lol)
 * 
 * At max, only 1 thread goes at a time (sequentially) if using the mutex tehcnique (mutual exclusion)
 * with one of the 2 algos. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 1000
#define COUNT_LIMIT 1000

//Global variables like this one reside in shared memory (same context) and can be used by all threads
//BUT: watch out for edge cases and implicit info: most of this course will regard problems accessing shared context
int shared_var = 0;

pthread_mutex_t m;

//This func can be staggered independently from the main() func
//Though, in this program, all of them execute the same func: thread_func()
void* thread_func(void* p)
{
    long id = (long)p;

    //printf("[%ld] Thread iniciada!\n", id);
    //If i don't use sleep() to control the sequence of finalized, the scheduler does it, optimizing
    //processing time. (so it's prob non-deterministic)
    //Obs: Even after a thread dies, it's return value is still accessible since it's saved in the same
    //memory context, and can be acquired by using the pthread_join() return value.
    //sleep(1);
    for(int i = 0; i < COUNT_LIMIT; i++){
        pthread_mutex_lock(&m); //Mutex_block begin!!! (every other threads gets blocked)
        //And now, each addition is stopping every other thread once it reaches this block between
        //mutex_lock and mutex_unlock.
        //But the order here is still being define by the scheduler
        //AND in this manner, since every thread is stopping every other thread once it reaches this block
        //it is way less efficient than the previous method of using atomic incrementation
        //(Well, different solutions for different ways i'd want to solve this problem)
        //2 solutions: Using mutual exclusion (mutex) or atomic increments
        shared_var++; //Now each thread increments +1000 to shared_var (much bigger increment loss if not cared for!!!)
        pthread_mutex_unlock(&m);   //Mutex_block end!!! (every other threads gets unblocked)

        //But the GNU compilers do have atomic incrementation!!
        //__atomic_fetch_add(&shared_var, 1, 0);
        //Now it works properly, cuz this line compiles to 1 instruction down in the asm
    }

    //The code line below shows that that shared var is common among threads
    //BUT the shared/global var access level is read-only among each other!!!!
    printf("[%ld] shared_var = %d\n", id, shared_var);
    //printf("[%ld] Thread finalizada!\n", id);

    return (void*)(id + 1);
}

int main(void)
{
    pthread_t threads[QTD_THREADS]; //A thread type in and of itself, unlike in processes
    int thread_results[QTD_THREADS];

    for(long i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], 0, thread_func, (void*)i);
    }

    for(int i = 0; i < QTD_THREADS; i++){
        //Joins funcs (and their threads) together, joined thread becomes sequential with this one
        //Joined thread comes to the main thread's context (!!)
        pthread_join(threads[i], &thread_results[i]);

        //printf("[main] recebi %d da thread #%d\n", thread_results[i], i);
    }

    //With so many threads, a lot of them are getting outdated shared_var increments, cuz the processor
    //is staggering based on whichever it chooses first (optimizing ofc)
    printf("[main] shared_var = %d\n", shared_var);

    return 0;
}