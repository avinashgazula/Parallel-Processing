
// pthread_setcanceltype()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void cleanupHandler(void *parm)
{
	int rc;

	printf("Inside cleanup handler, unlock mutex\n");
	rc = pthread_mutex_unlock((pthread_mutex_t *)parm);
	checkResults("pthread_mutex_unlock\n", rc);
}

void *threadfunc(void *parm)
{
	int rc;
	int oldtype;

	printf("Entered secondary thread, lock mutex\n");
	rc = pthread_mutex_lock(&mutex);
	checkResults("pthread_mutex_lock()\n", rc);

	pthread_cleanup_push(cleanupHandler, &mutex);

	/* We must assume there is a good reason for async. cancellability */
	/* and also, we must assume that if we get interrupted, it is */
	/* appropriate to unlock the mutex. More than likely it is not */
	/* because we will have left some data structures in a strange state */
	/* if we are async. interrupted while holding the mutex */

	rc = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
	checkResults("pthread_setcanceltype()\n", rc);

	printf("Secondary thread is now looping\n");
	while (1) { sleep(1); }

	printf("Unexpectedly got out of loop!\n");
	pthread_cleanup_pop(0);

	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc=0;
	void *status;

	printf("Enter Testcase - %s\n", argv[0]);

	/* Create a thread using default attributes */
	printf("Create thread using the NULL attributes\n");
	rc = pthread_create(&thread, NULL, threadfunc, NULL);
	checkResults("pthread_create(NULL)\n", rc);

	/* sleep() is not a very robust way to wait for the thread */
	sleep(1);
	printf("Cancel the thread\n");
	rc = pthread_cancel(thread);
	checkResults("pthread_cancel()\n", rc);

	rc = pthread_join(thread, &status);
	if (status != PTHREAD_CANCELED) {
		printf("Unexpected thread status\n");
		exit(1);
	}

	printf("Main completed\n");
	return 0;
}

// Output:
// Enter Testcase - 
// Create thread using the NULL attributes
// Entered secondary thread, lock mutex
// Secondary thread is now looping
// Cancel the thread
// Inside cleanup handler, unlock mutex
// Main completed


