#include <stdio.h>
#include <pthread.h>

#define num_loops 20000000

long long sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* counting(void *arg) {
	int offset = *(int *) arg;
	for(long long i = 0; i < num_loops; i++) {
		pthread_mutex_lock(&mutex);
		sum += offset;
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

int main() {
	pthread_t id1;
	int offset1 = 1;
	pthread_create(&id1, NULL, counting, &offset1);

	pthread_t id2;
	int offset2 = -1;
	pthread_create(&id2, NULL, counting, &offset2);

	//wait for threads to finish
	pthread_join(id1, NULL);
	pthread_join(id2, NULL);

	printf("Sum = %lld\n", sum);

	return 0;
}
