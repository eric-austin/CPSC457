/// counts number of primes from standard input
///
/// compile with:
///   $ gcc findPrimes.c -O2 -o count -lm
///
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>

//structure for tracking a threads search space
struct search_struct {
  int64_t first;
  int64_t last;
  int isPrime;
  int64_t num;
};

/// primality test, if n is prime, return 1, else return 0
int isPrimeSingle(int64_t n)
{
     if( n <= 1) return 0; // small numbers are not primes
     if( n <= 3) return 1; // 2 and 3 are prime
     if( n % 2 == 0 || n % 3 == 0) return 0; // multiples of 2 and 3 are not primes
     int64_t i = 5;
     int64_t max = sqrt(n);
     while( i <= max) {
         if (n % i == 0 || n % (i+2) == 0) return 0;
         i += 6;
     }
     return 1;
}

void* isPrimeMulti(void* arg) {
  struct search_struct *arg_struct = (struct search_struct*) arg;
  int64_t i = arg_struct->first;
  int64_t max = arg_struct->last;
  int64_t n = arg_struct->num;
  while(i <= max) {
    if(n % i == 0 || n % (i+2) == 0) {
      arg_struct->isPrime = 0;
      break;
    }
    i += 6;
  }
}

int main( int argc, char ** argv)
{
    /// parse command line arguments
    int nThreads = 1;
    if( argc != 1 && argc != 2) {
        printf("Usage: countPrimes [nThreads]\n");
        exit(-1);
    }
    if( argc == 2) nThreads = atoi( argv[1]);

    /// handle invalid arguments
    if( nThreads < 1 || nThreads > 256) {
        printf("Bad arguments. 1 <= nThreads <= 256!\n");
        exit(-1);
    }
    //declare count variable
    int64_t count = 0;
    //check whether running multi or single threaded
    if( nThreads != 1) {
      //set up threads
      pthread_t threads[nThreads];
      struct search_struct ranges[nThreads];
      //loop through all numbers in file
      while(1) {
        //read in one number
        int64_t num;
        if(1 != scanf("%ld", & num)) break;
        //if number is small skip the multithreading
        if(num < 1000) {
          if(isPrimeSingle(num))
            count++;
            continue;
        }
        //if number large, do multi threading
        int64_t range = (sqrt(num)) / nThreads + 1;
        int64_t perRange = range / 6 + 1;
        //set up ranges for each threads
        for(int i = 0; i < nThreads; i++) {
          //calculate search ranges
          ranges[i].first = i * perRange * 6 + 5;
          ranges[i].last = (i + 1) * perRange * 6 + 5;
          ranges[i].isPrime = 1;
          ranges[i].num = num;
          //start the threads
          pthread_create(&threads[i], NULL, isPrimeMulti, &ranges[i]);
        }
        for(int i = 0; i < nThreads; i++) {
          pthread_join(threads[i], NULL);
        }
        int isPrime = 1;
        for(int i = 0; i < nThreads; i++) {
          if(ranges[i].isPrime == 0)
            isPrime = 0;
        }
        count += isPrime;
      }
      printf("Counting primes using %d thread%s.\n",
          nThreads, nThreads == 1 ? "s" : "");

    } else {
        /// single thread
        printf("Counting primes using %d thread%s.\n",
            nThreads, nThreads == 1 ? "s" : "");
        while( 1) {
            int64_t num;
            if( 1 != scanf("%ld", & num)) break;
            if( isPrimeSingle(num)) count ++;
        }
    }


    /// report results
    printf("Found %ld primes.\n", count);

    return 0;
}
