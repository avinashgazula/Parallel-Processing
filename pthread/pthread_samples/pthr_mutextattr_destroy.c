
// pthread_mutexattr_destroy()-- Example
#include <pthread.h>
#include <stdio.h>
#include "check.h"

pthread_mutex_t mutex;

int main(int argc, char **argv)
{
	int rc=0;
	pthread_mutexattr_t mta;

	printf("Entering testcase\n");
	printf("Create a default mutex attribute\n");
	rc = pthread_mutexattr_init(&mta);
	checkResults("pthread_mutexattr_init\n", rc);

	printf("Create the mutex using a mutex attributes object\n");
	rc = pthread_mutex_init(&mutex, &mta);
	checkResults("pthread_mutex_init(mta)\n", rc);

	printf("- At this point, the mutex with its default attributes\n");
	printf("- Can be used from any threads that want to use it\n");
	printf("Destroy mutex attribute\n");
	rc = pthread_mutexattr_destroy(&mta);
	checkResults("pthread_mutexattr_destroy()\n", rc);

	printf("Destroy mutex\n");
	rc = pthread_mutex_destroy(&mutex);
	checkResults("pthread_mutex_destroy()\n", rc);

	printf("Main completed\n");
	return 0;
}

// Output:
// Entering testcase
// Create a default mutex attribute
// Create the mutex using a mutex attributes object
// - At this point, the mutex with its default attributes
// - Can be used from any threads that want to use it
// Destroy mutex attribute
// Destroy mutex
// Main completed





