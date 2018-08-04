
// pthread_getcancelstate_np()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include <except.h>
#include <setjmp.h>
#include "check.h"

void showCancelState(void);

int threadStatus=42;

void showCancelState(void)
{
	int state, rc;

	rc = pthread_getcancelstate_np(&state);
	checkResults("pthread_getcancelstate_np()\n", rc);
	printf("current cancel state is %d\n", state);
}

void cleanupHandler2(void *p)
{
	printf("In cancellation cleanup handler\n");
	showCancelState();
	return;
}

void *threadfunc(void *parm)
{
	int rc, old;

	printf("Inside secondary thread\n");
	showCancelState();

	pthread_cleanup_push(cleanupHandler2, NULL);
	threadStatus = 0;
	printf("Calling pthread_exit() will allow cancellation cleanup handlers to run\n");
	pthread_exit(__VOID(threadStatus));
	pthread_cleanup_pop(0);

	return __VOID(-1);
}

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc=0;
	char c;
	void *status;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create thread that will demonstrate
	pthread_getcancelstate_np()\n");
	rc = pthread_create(&thread, NULL, threadfunc, NULL);
	checkResults("pthread_create()\n", rc);

	rc = pthread_join(thread, &status);
	checkResults("pthread_join()\n", rc);

	if (__INT(status) != threadStatus) {
		printf("Got an unexpected return status from the thread!\n");
		exit(1);
	}

	printf("Main completed\n");
	return 0;
}


// Output:
// Enter Testcase - 
// Create thread that will demonstrate pthread_getcancelstate_np()
// Inside secondary thread
// current cancel state is 0
// Calling pthread_exit() will allow cancellation cleanup handlers to run
// In cancellation cleanup handler
// current cancel state is 1
// Main completed


