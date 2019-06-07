/// counts number of primes from standard input
///
/// compile with:
///   $ gcc findPrimes.c -O2 -o count -lm
///
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <thread>

using namespace std;

/// primality test, if n is prime, return 1, else return 0
int isPrime(long long n, long long first, long long last)
{
     if( n <= 1) return 0; // small numbers are not primes
     if( n <= 3) return 1; // 2 and 3 are prime
     if( n % 2 == 0 || n % 3 == 0) return 0; // multiples of 2 and 3 are not primes
     long long i;
     if(first < 5) {
       i = 5;
     } else {
       i = first;
     }
     while( i <= last) {
         if (n % i == 0 || n % (i+2) == 0) return 0;
         i += 6;
     }
     return 1;
}

void testFunction(long long i) {
  printf("thread %lld starting", i);
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
    long long count = 0;
    //check whether running multi or single threaded
    if( nThreads != 1) {
      thread threads[nThreads];
      printf("I would solve with %d threads, were I working.\n", nThreads);
      for(int i = 0; i < nThreads; i++) {
        threads[i] = thread(testFunction, (long long) i);
      }
      for(int i = 0; i < nThreads; i++) {
        threads[i].join();
      }
    } else {
        /// single thread
        printf("Counting primes using %d thread%s.\n",
            nThreads, nThreads == 1 ? "s" : "");
        while( 1) {
            long long num;
            if( 1 != scanf("%lld", & num)) break;
            if( isPrime(num, 1, sqrt(num))) count ++;
        }
    }


    /// report results
    printf("Found %lld primes.\n", count);

    return 0;
}
