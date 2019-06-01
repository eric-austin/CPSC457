#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 5

int counter;

void incr() {
	int x = counter;
	x = x + 1;
	counter = x;
}

int main() {
	counter = 0;
	pthread_t t1;
	pthread_t t2;
	pthread_create(&t1, NULL, incr);
	pthread_create(&t2, NULL, incr);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	printf("%d\n", counter);
}
