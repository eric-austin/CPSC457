#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int x;

void * do_something( ) {
	x = 11;
	pthread_exit(0);
}

int main() {
	x = 10;

	pthread_t tid;
	pthread_create(&tid, NULL, do_something, NULL);

	pthread_join(tid, NULL);
	printf("x=%d\n", x);
}
