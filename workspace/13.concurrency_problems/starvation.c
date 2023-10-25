#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

pthread_mutex_t lock;
int resource = 0;

void job() {
	int rand_sleep = rand() % 5;
	sleep(rand_sleep);
}

void* high_priority_function(void* p) {
	for (int i = 0; i < 5; i++) {
		pthread_mutex_lock(&lock);
		//job();
		resource++;
		printf("[high] Resource = %d\n", resource);
		pthread_mutex_unlock(&lock);
	}

}

void* low_priority_function(void* p) {
        for (int i = 0; i < 5; i++) {
                pthread_mutex_lock(&lock);
                job();
                resource--;
                printf("[low] Resource = %d\n", resource);
                pthread_mutex_unlock(&lock);
        }
}

int main(void) {
	pthread_t thread1, thread2;
	time_t t;

	srand(time(&t));

	pthread_mutex_init(&lock, NULL);

	pthread_create(&thread1, NULL, high_priority_function, NULL);
	pthread_create(&thread2, NULL, low_priority_function, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	pthread_mutex_destroy(&lock);

	return 0;
}
