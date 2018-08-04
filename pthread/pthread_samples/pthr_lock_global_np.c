
// pthread_lock_global_np()-- Example
#include <pthread.h>
#include <stdio.h>
#include "check.h"

/*
This example shows the corruption that can result if no
serialization is done and also shows the use of
pthread_lock_global_np(). Call this test with no parameters
to use pthread_lock_gloabl_np() to protect the critical data,
between more than one (possibly unrelated) functions.
Use 1 or more parameters to skip locking and
show data corruption that occurs without locking.
*/

#define LOOPCONSTANT 50000
#define THREADS 10

int i,j,k,l;
int uselock=1;

void secondFunction(void)
{
	int rc;

	if (uselock) {
		rc = pthread_lock_global_np();
		checkResults("pthread_lock_global_np()\n", rc);
	}

	--i; --j; --k; --l;

	if (uselock) {
		rc = pthread_unlock_global_np();
		checkResults("pthread_unlock_global_np()\n", rc);
	}
}

void *threadfunc(void *parm)
{
	int loop = 0;
	int rc;

	for (loop=0; loop<LOOPCONSTANT; ++loop) {
		if (uselock) {
			rc = pthread_lock_global_np();
			checkResults("pthread_lock_global_np()\n", rc);
		}

		++i; ++j; ++k; ++l;

		secondFunction();

		++i; ++j; ++k; ++l;

		if (uselock) {
			rc = pthread_unlock_global_np();
			checkResults("pthread_unlock_global_np()\n", rc);
		}
	}

	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t threadid[THREADS];
	int rc=0;
	int loop=0;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Give any number of parameters to show data corruption\n");

	if (argc != 1) {
		printf("A parameter was specified, no serialization is being done!\n");
		uselock = 0;
	}

	if (uselock) {
		rc = pthread_lock_global_np();
		checkResults("pthread_lock_global_np() (main)\n", rc);
	}

	printf("Creating %d threads\n", THREADS);
	for (loop=0; loop<THREADS; ++loop) {
		rc = pthread_create(&threadid[loop], NULL, threadfunc, NULL);
		checkResults("pthread_create()\n", rc);
	}

	sleep(5);
	if (uselock) {
		rc = pthread_unlock_global_np();
		checkResults("pthread_unlock_global_np() (main)\n", rc);
	}

	printf("Wait for results\n");
	for (loop=0; loop<THREADS; ++loop) {
		rc = pthread_join(threadid[loop], NULL);
		checkResults("pthread_join()\n", rc);
	}

	printf("\nUsing %d threads and LOOPCONSTANT = %d\n",
	THREADS, LOOPCONSTANT);
	printf("Values are: (should be %d)\n", THREADS * LOOPCONSTANT);
	printf(" ==>%d, %d, %d, %d\n", i, j, k, l);
	printf("Main completed\n");

	return 0;
}

// Output:
// Enter Testcase - 
// Give any number of parameters to show data corruption
// Creating 10 threads
// Wait for results
// Using 10 threads and LOOPCONSTANT = 50000
// Values are: (should be 500000)
// ==>500000, 500000, 500000, 500000
// Main completed





