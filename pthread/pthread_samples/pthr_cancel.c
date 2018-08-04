
// pthread_cancel()-- Example
#include <pthread.h>
#include <stdio.h>
#include "check.h"

void *threadfunc(void *parm)
{
	printf("Entered secondary thread\n");

	while (1) {
		printf("Secondary thread is looping\n");
		pthread_testcancel();
		sleep(1);
	}

	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc=0;

	printf("Entering testcase\n");
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
// Entering testcase
// Create thread using the NULL attributes
// Entered secondary thread
// Secondary thread is looping
// Secondary thread is looping
// Cancel the thread
// Main completed



