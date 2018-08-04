
// pthread_mutex_unlock()-- Example
#include <pthread.h>
#include <stdio.h>
#include "check.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
	int rc=0;

	printf("Entering testcase\n");
	printf("Lock the mutex\n");
	rc = pthread_mutex_lock(&mutex);
	checkResults("pthread_mutex_lock()\n", rc);

	/* All other threads will be blocked from the resource here */
	printf("Unlock the mutex\n");
	rc = pthread_mutex_unlock(&mutex);
	checkResults("pthread_mutex_unlock()\n", rc);

	printf("Destroy the mutex\n");
	rc = pthread_mutex_destroy(&mutex);
	checkResults("pthread_mutex_destroy()\n", rc);

	printf("Main completed\n");
	return 0;
}

// Output:
// Entering testcase
// Lock the mutex
// Unlock the mutex
// Destroy the mutex
// Main completed




