
// pthread_equal()-- Example
#define _MULTI_THREADED
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

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create thread using default attributes\n");
	rc = pthread_create(&thread, NULL, threadfunc, NULL);
	checkResults("pthread_create()\n", rc);

	/* sleep() is not a very robust way to wait for the thread */
	sleep(5);

	printf("Check if global vs local pthread_t are equal\n");
	if (!pthread_equal(thread, theThread)) {
		printf("Unexpected results on pthread_equal()!\n");
		exit(1);
	}

	printf("pthread_equal returns true\n");
	printf("Main completed\n");

	return 0;
}

// Output:
// Enter Testcase - 
// Create thread using default attributes
// Inside secondary thread
// Check if global vs local pthread_t are equal
// pthread_equal returns true
// Main completed


