
// pthread_mutex_timedlock_np()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *threadFunc(void *parm)
{
	int rc;
	int i;
	struct timespec deltatime;

	deltatime.tv_sec = 5;
	deltatime.tv_nsec = 0;

	printf("Timed lock the mutex from a secondary thread\n");
	rc = pthread_mutex_timedlock_np(&mutex, &deltatime);

	if (rc != EBUSY) {
		printf("Got an incorrect return code from
		pthread_mutex_timedlock_np\n");
	}

	printf("Thread mutex timeout\n");
	return 0;
}

int main(int argc, char **argv)
{
	int rc=0;
	pthread_t thread;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Acquire the mutex in the initial thread\n");
	rc = pthread_mutex_lock(&mutex);
	checkResults("pthread_mutex_lock()\n", rc),

	printf("Create a thread\n");
	rc = pthread_create(&thread, NULL, threadFunc, NULL);
	checkResults("pthread_create()\n", rc);

	printf("Join to the thread\n");
	rc = pthread_join(thread, NULL);
	checkResults("pthread_join()\n", rc);

	printf("Destroy mutex\n");
	pthread_mutex_destroy(&mutex);
	printf("Main completed\n");

	return 0;
}

// Output:
// Enter Testcase - 
// Acquire the mutex in the initial thread
// Create a thread
// Join to the thread
// Timed lock the mutex from a secondary thread
// Thread mutex timeout
// Destroy mutex
// Main completed



