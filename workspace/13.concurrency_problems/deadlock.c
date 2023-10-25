#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock1, lock2;

void* function_thread1(void* p) {
	printf("[t1] function_thread1()\n");
	pthread_mutex_lock(&lock1);
	printf("[t1] lock(lock1)\n");
	sleep(1);
	pthread_mutex_lock(&lock2);
	printf("[t1] lock(lock2)\n");
	sleep(1);

	pthread_mutex_unlock(&lock2);
	pthread_mutex_unlock(&lock1);

	return NULL;

}

void* function_thread2(void* p) {
        printf("[t2] function_thread2()\n");
        pthread_mutex_lock(&lock2);
        printf("[t2] lock(lock2)\n");
	sleep(1);
        pthread_mutex_lock(&lock1);
        printf("[t2] lock(lock1)\n");
	sleep(1);

        pthread_mutex_unlock(&lock1);
        pthread_mutex_unlock(&lock2);

        return NULL;

}

int main(void) {
	pthread_t thread1, thread2;

	pthread_mutex_init(&lock1, NULL);
	pthread_mutex_init(&lock2, NULL);

	pthread_create(&thread1, NULL, function_thread1, NULL);
	pthread_create(&thread2, NULL, function_thread2, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	pthread_mutex_destroy(&lock1);
	pthread_mutex_destroy(&lock2);

	return 0;
}

