#include <iostream>
#include <pthread.h>
#include <cstdlib>

using namespace std;

//Max size of array
#define max 100000000

//Max number of threads to create
#define thread_max 4

long a[max];
long key = 99999990;

//Flag to indicate if key is found in a[] or not
long f = -1;

int current_thread = 0;

//Linear search function which will run for all the threads
void* ThreadSearch(void* args) {
	int num = current_thread++;

	for(long i = num * (max / thread_max); i < ((num + 1) * (max / thread_max)); i++) {
		if(a[i] == key)
			f = i;
	}
}

int main() {
	for(long j = 0; j < max; j++)
		a[j] = j;

	pthread_t thread[thread_max];

	for(int i = 0; i < thread_max; i++) {
		pthread_create(&thread[i], NULL, ThreadSearch, (void*)NULL);
	}

	for(int i = 0; i < thread_max; i++) {
		pthread_join(thread[i], NULL);
	}

	if(f == -1)
		cout << "Key not present" << endl;
	else
		cout << "Key element found in index " << f << endl;

	return 0;
}
