#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

const int N_ITERATIONS = 10000000;

int counter;
pthread_mutex_t my_mutex;

void thread1_start() {
	for (int i = 0; i < N_ITERATIONS; i++) {
		pthread_mutex_lock(&my_mutex);
		counter = counter + 1;
		pthread_mutex_unlock(&my_mutex);
	}
	pthread_exit(0);
}

void thread2_start() {
	for (int i = 0; i < N_ITERATIONS; i++) {
		pthread_mutex_lock(&my_mutex);
		counter = counter - 1;
		pthread_mutex_unlock(&my_mutex);
	}
	pthread_exit(0);
}

int main() {
	pthread_mutex_init(&my_mutex, NULL);
	counter = 0;
	pthread_t thread1, thread2;
	pthread_create(&thread1, NULL, thread1_start, 0);
	pthread_create(&thread2, NULL, thread2_start, 0);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	pthread_mutex_destroy(&my_mutex);
	printf("Counter = %d\n", counter);
	exit(0);
}
