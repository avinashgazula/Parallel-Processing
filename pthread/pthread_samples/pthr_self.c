
// pthread_self()-- Example
#include <pthread.h>
#include <pthread.h>
#include <stdio.h>
#include "check.h"

pthread_t theThread;

void *threadfunc(void *parm)
{
	printf("Inside secondary thread\n");
	theThread = pthread_self();
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc=0;

	printf("Entering testcase\n");
	printf("Create thread using default attributes\n");
	rc = pthread_create(&thread, NULL, threadfunc, NULL);
	checkResults("pthread_create()\n", rc);

	/* sleep() is not a very robust way to wait for the thread */
	sleep(5);
	printf("Check if the thread got its thread handle\n");
	if (!pthread_equal(thread, theThread)) {
		printf("Unexpected results on pthread_equal()!\n");
		exit(1);
	}

	printf("pthread_self() returned the thread handle\n");
	printf("Main completed\n");

	return 0;
}

// Output:
// Entering testcase
// Create thread using default attributes
// Inside secondary thread
// Check if the thread got its thread handle
// pthread_self() returned the thread handle
// Main completed



