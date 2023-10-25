#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t lock;

void* increment(void* p) {
	for (int i = 0; i < 1000000; i++) {
		pthread_mutex_lock(&lock);
		counter++;
		pthread_mutex_unlock(&lock);
	}
	return NULL;

}

int main(void) {
	pthread_t thread1, thread2;

	pthread_create(&thread1, NULL, increment, NULL);
	pthread_create(&thread2, NULL, increment, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("Counter final value: %d\n", counter);

	pthread_mutex_destroy(&lock);

	return 0;
}
