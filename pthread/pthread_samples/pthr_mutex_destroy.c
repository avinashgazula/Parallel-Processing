
// pthread_mutex_destroy()-- Example
#include <pthread.h>
#include <stdio.h>
#include "check.h"

pthread_mutex_t mutex;

int main(int argc, char **argv)
{
	int rc=0;
	pthread_mutexattr_t mta;

	printf("Entering testcase\n");
	printf("Create the mutex using the NULL attributes (default)\n");
	rc = pthread_mutex_init(&mutex, NULL);
	checkResults("pthread_mutex_init(NULL)\n", rc);

	printf("Destroy all mutexes\n");
	pthread_mutex_destroy(&mutex);
	checkResults("pthread_mutex_destroy()\n", rc);

	printf("Main completed\n");
	return 0;
}

// Output:
// Entering testcase
// Create the mutex using the NULL attributes (default)
// Destroy all mutexes
// Main completed




