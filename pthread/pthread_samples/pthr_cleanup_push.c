
// pthread_cleanup_push()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

void cleanupHandler(void *arg)
{
	printf("In the cleanup handler\n");
}

void *threadfunc(void *parm)
{
	printf("Entered secondary thread\n");
	pthread_cleanup_push(cleanupHandler, NULL);

	while (1) {
		pthread_testcancel();
		sleep(1);
	}

	pthread_cleanup_pop(0);
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc=0;

	printf("Enter Testcase - %s\n", argv[0]);

	/* Create a thread using default attributes */
	printf("Create thread using the NULL attributes\n");
	rc = pthread_create(&thread, NULL, threadfunc, NULL);
	checkResults("pthread_create(NULL)\n", rc);

	/* sleep() is not a very robust way to wait for the thread */
	sleep(2);

	printf("Cancel the thread\n");
	rc = pthread_cancel(thread);
	checkResults("pthread_cancel()\n", rc);

	/* sleep() is not a very robust way to wait for the thread */
	sleep(3);
	printf("Main completed\n");

	return 0;
}

// Output:
// Enter Testcase - 
// Create thread using the NULL attributes
// Entered secondary thread
// Cancel the thread
// In the cleanup handler
// Main completed


