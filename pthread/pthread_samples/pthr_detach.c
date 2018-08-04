// pthread_detach()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "check.h"

void *threadfunc(void *parm)
{
	printf("Inside secondary thread\n");
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc=0;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create thread using attributes that allow join or detach\n");

	rc = pthread_create(&thread, NULL, threadfunc, NULL);
	checkResults("pthread_create()\n", rc);

	sleep(5);
	printf("Detach the thread after it terminates\n");
	rc = pthread_detach(thread);
	checkResults("pthread_detach()\n", rc);

	printf("Detach the thread again (expect ESRCH)\n");
	rc = pthread_detach(thread);

	if (rc != ESRCH) {
		printf("Got an unexpected result! rc=%d\n", rc);
		exit(1);
	}

	printf("Second detach fails correctly\n");
	/* sleep() is not a very robust way to wait for the thread */
	sleep(5);
	printf("Main completed\n");

	return 0;
}


// Output:
// Enter Testcase - 
// Create thread using attributes that allow join or detach
// Inside secondary thread
// Detach the thread after it terminates
// // Detach the thread again (expect ESRCH)
// Second detach fails correctly
// Main completed

