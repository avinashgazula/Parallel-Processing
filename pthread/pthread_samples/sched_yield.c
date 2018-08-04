// sched_yield()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include "check.h"

#define LOOPCONSTANT 1000
#define THREADS 3

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int i,j,k,l;

void *threadfunc(void *parm)
{
	int loop = 0;
	int localProcessingCompleted = 0;
	int numberOfLocalProcessingBursts = 0;
	int processingCompletedThisBurst = 0;
	int rc;

	printf("Entered secondary thread\n");
	for (loop=0; loop<LOOPCONSTANT; ++loop) {
		rc = pthread_mutex_lock(&mutex);
		checkResults("pthread_mutex_lock()\n", rc);

		/* Perform some not so important processing */
		i++, j++, k++, l++;
		rc = pthread_mutex_unlock(&mutex);
		checkResults("pthread_mutex_unlock()\n", rc);

		/* This work is not too important. Also, we just released a lock
		and would like to ensure that other threads get a chance in
		a more co-operative manner. This is an admittedly contrived
		example with no real purpose for doing the sched_yield().
		*/
		sched_yield();
	}

	printf("Finished secondary thread\n");
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t threadid[THREADS];
	int rc=0;
	int loop=0;

	printf("Enter Testcase - %s\n", argv[0]);
	rc = pthread_mutex_lock(&mutex);
	checkResults("pthread_mutex_lock()\n", rc);

	printf("Creating %d threads\n", THREADS);
	for (loop=0; loop<THREADS; ++loop) {
		rc = pthread_create(&threadid[loop], NULL, threadfunc, NULL);
		checkResults("pthread_create()\n", rc);
	}

	sleep(1);
	rc = pthread_mutex_unlock(&mutex);
	checkResults("pthread_mutex_unlock()\n", rc);

	printf("Wait for results\n");
	for (loop=0; loop<THREADS; ++loop) {
		rc = pthread_join(threadid[loop], NULL);
		checkResults("pthread_join()\n", rc);
	}

	pthread_mutex_destroy(&mutex);
	printf("Main completed\n");

	return 0;
}

// Output:
// Enter Testcase - 
// Creating 3 threads
// Entered secondary thread
// Entered secondary thread
// Entered secondary thread
// Wait for results
// Finished secondary thread
// Finished secondary thread
// Finished secondary thread
// Main completed




