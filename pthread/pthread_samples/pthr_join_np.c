
// pthread_join_np()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

int okStatus = 12;

void *threadfunc(void *parm)
{
	printf("Inside secondary thread\n");
	return __VOID(okStatus);
}

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc=0;
	void *status;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create thread using attributes that allow join\n");
	rc = pthread_create(&thread, NULL, threadfunc, NULL);
	checkResults("pthread_create()\n", rc);

	printf("Wait for the thread to exit\n");
	rc = pthread_join_np(thread, &status);
	checkResults("pthread_join_np()\n", rc);

	if (__INT(status) != okStatus) {
		printf("Secondary thread failed\n");
		exit(1);
	}

	printf("With pthread_join_np(), we can join repeatedly\n");
	rc = pthread_join_np(thread, &status);
	checkResults("pthread_join_np()\n", rc);

	if (__INT(status) != okStatus) {
		printf("Secondary thread failed\n");
		exit(1);
	}

	printf("Got secondary thread status as expected\n");
	/* Eventually, we should use pthread_join() or pthread_detach() */
	rc = pthread_detach(thread);
	checkResults("pthread_detach()\n", rc);

	printf("Main completed\n");
	return 0;
}

// Output:
// Enter Testcase - 
// Create thread using attributes that allow join
// Wait for the thread to exit
// Inside secondary thread
// With pthread_join_np(), we can join repeatedly
// Got secondary thread status as expected
// Main completed


