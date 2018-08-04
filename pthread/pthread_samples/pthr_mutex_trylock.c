
// pthread_mutex_trylock()-- Example
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include "check.h"

/*
This example simulates a number of threads working on a parallel
problem. The threads use pthread_mutex_trylock() so that
they do not spend time blocking on a mutex and instead spend more
of the time making progress towards the final solution. When
trylock fails, the processing is done locally, eventually to
be merged with the final parallel solution.
This example should complete faster than the example for
pthread_mutex_lock() in which threads solve the same parallel
problem but spend more time waiting in resource contention.
*/

#define LOOPCONSTANT 100000
#define THREADS 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int i,j,k,l;

void *threadfunc(void *parm)
{
	int loop = 0;
	int localProcessingCompleted = 0;
	int numberOfLocalProcessingBursts = 0;
	int processingCompletedThisBurst = 0;
	int rc;

	for (loop=0; loop<LOOPCONSTANT; ++loop) {
		rc = pthread_mutex_trylock(&mutex);

		if (rc == EBUSY) {
			/* Process continue processing the part of the problem */
			/* that we can without the lock. We do not want to waste */
			/* time blocking. Instead, we'll count locally. */

			++localProcessingCompleted;
			++numberOfLocalProcessingBursts;
			continue;
		}

		/* We acquired the lock, so this part of the can be global*/
		checkResults("pthread_mutex_trylock()\n", rc);

		/* Processing completed consist of last local processing */
		/* plus the 1 unit of processing this time through */
		processingCompletedThisBurst = 1 + localProcessingCompleted;
		localProcessingCompleted = 0;
		i+=processingCompletedThisBurst; j+=processingCompletedThisBurst;
		k+=processingCompletedThisBurst; l+=processingCompletedThisBurst;
		rc = pthread_mutex_unlock(&mutex);
		checkResults("pthread_mutex_unlock()\n", rc);
	}

	/* If any local processing remains, merge it with the global*/
	/* problem so our part of the solution is accounted for */
	if (localProcessingCompleted) {
		rc = pthread_mutex_lock(&mutex);
		checkResults("final pthread_mutex_lock()\n", rc);

		i+=localProcessingCompleted; j+=localProcessingCompleted;
		k+=localProcessingCompleted; l+=localProcessingCompleted;

		rc = pthread_mutex_unlock(&mutex);
		checkResults("final pthread_mutex_unlock()\n", rc);
	}

	printf("Thread processed about %d%% of the problem locally\n",
	(numberOfLocalProcessingBursts * 100) / LOOPCONSTANT);
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t threadid[THREADS];
	int rc=0;
	int loop=0;
	pthread_attr_t pta;

	printf("Entering testcase\n");
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
// Creating 10 threads
// Wait for results
// Thread processed about 100% of the problem locally
// Thread processed about 90% of the problem locally
// Thread processed about 88% of the problem locally
// Thread processed about 94% of the problem locally
// Thread processed about 93% of the problem locally
// Thread processed about 96% of the problem locally
// Thread processed about 90% of the problem locally
// Thread processed about 91% of the problem locally
// Thread processed about 81% of the problem locally
// Thread processed about 76% of the problem locally
// Cleanup and show results
// Using 10 threads and LOOPCONSTANT = 100000
// Values are: (should be 1000000)
// ==>1000000, 1000000, 1000000, 1000000
// Main completed



