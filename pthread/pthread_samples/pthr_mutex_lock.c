
// pthread_mutex_lock()-- Example
#include <pthread.h>
#include <stdio.h>
#include "check.h"

/*
This example shows the corruption that can result if no
serialization is done and also shows the use of
pthread_mutex_lock(). Call it with no parameters
to use pthread_mutex_lock() to protect the critical section,
or 1 or more parameters to show data corruption that occurs
without locking.
*/

#define LOOPCONSTANT 100000
#define THREADS 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int i,j,k,l;
int uselock=1;

void *threadfunc(void *parm)
{
	int loop = 0;
	int rc;

	for (loop=0; loop<LOOPCONSTANT; ++loop) {
		if (uselock) {
			rc = pthread_mutex_lock(&mutex);
			checkResults("pthread_mutex_lock()\n", rc);
		}

		++i; ++j; ++k; ++l;

		if (uselock) {
			rc = pthread_mutex_unlock(&mutex);
			checkResults("pthread_mutex_unlock()\n", rc);
		}
	}

	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t threadid[THREADS];
	int rc=0;
	int loop=0;
	pthread_attr_t pta;

	printf("Entering testcase\n");
	printf("Give any number of parameters to show data corruption\n");

	if (argc != 1) {
		printf("A parameter was specified, no serialization is being done!\n");
		uselock = 0;
	}

	pthread_attr_init(&pta);
	pthread_attr_setdetachstate(&pta, PTHREAD_CREATE_JOINABLE);

	printf("Creating %d threads\n", THREADS);
	for (loop=0; loop<THREADS; ++loop) {
		rc = pthread_create(&threadid[loop], &pta, threadfunc, NULL);
		checkResults("pthread_create()\n", rc);
	}

	printf("Wait for results\n");
	for (loop=0; loop<THREADS; ++loop) {
		rc = pthread_join(threadid[loop], NULL);
		checkResults("pthread_join()\n", rc);
	}

	printf("Cleanup and show results\n");
	pthread_attr_destroy(&pta);
	pthread_mutex_destroy(&mutex);

	printf("\nUsing %d threads and LOOPCONSTANT = %d\n", THREADS, LOOPCONSTANT);
	printf("Values are: (should be %d)\n", THREADS * LOOPCONSTANT);
	printf(" ==>%d, %d, %d, %d\n", i, j, k, l);
	printf("Main completed\n");
	return 0;
}

// Output:
// Entering testcase
// Give any number of parameters to show data corruption
// Creating 10 threads
// Wait for results
// Cleanup and show results
// Using 10 threads and LOOPCONSTANT = 100000
// Values are: (should be 1000000)
// ==>1000000, 1000000, 1000000, 1000000
// Main completed
// 
// Output:
// (data corruption without locking example)
// Entering testcase
// Give any number of parameters to show data corruption
// A parameter was specified, no serialization is being done!
// Creating 10 threads
// Wait for results
// Cleanup and show results
// Using 10 threads and LOOPCONSTANT = 100000
// Values are: (should be 1000000)
// ==>883380, 834630, 725131, 931883
// Main completed


