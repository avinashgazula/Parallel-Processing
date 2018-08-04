
// pthread_mutex_init()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2;
pthread_mutex_t mutex3;

int main(int argc, char **argv)
{
	int rc=0;
	pthread_mutexattr_t mta;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create a default mutex attribute\n");
	rc = pthread_mutexattr_init(&mta);
	checkResults("pthread_mutexattr_init\n", rc);

	printf("Create the mutexes using the default mutex attributes\n");
	printf("First mutex created via static PTHREAD_MUTEX_INITIALIZER\n");
	printf("Create the mutex using the NULL attributes (default)\n");

	rc = pthread_mutex_init(&mutex3, NULL);
	checkResults("pthread_mutex_init(NULL)\n", rc);

	printf("Create the mutex using a mutex attributes object\n");
	rc = pthread_mutex_init(&mutex2, &mta);
	checkResults("pthread_mutex_init(mta)\n", rc);

	printf("- At this point, all mutexes can be used with their\n");
	printf("- default attributes from any threads that want to\n");
	printf("- use them\n");

	printf("Destroy all mutexes\n");
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex2);
	pthread_mutex_destroy(&mutex3);

	printf("Main completed\n");
	return 0;
}

// Output:
// Enter Testcase - 
// Create a default mutex attribute
// Create the mutexes using the default mutex attributes
// First mutex created via static PTHREAD_MUTEX_INITIALIZER
// Create the mutex using the NULL attributes (default)
// Create the mutex using a mutex attributes object
// - At this point, all mutexes can be used with their
// - default attributes from any threads that want to
// - use them
// Destroy all mutexes
// Main completed



