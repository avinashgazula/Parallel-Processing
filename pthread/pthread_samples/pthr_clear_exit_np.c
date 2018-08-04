
// pthread_clear_exit_np()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include <except.h>
#include <setjmp.h>
#include "check.h"
int threadStatus=1;

void cleanupHandler(void *p)
{
	jmp_buf *j = (jmp_buf *)p;

	/* Warning, it is quite possible that using combinations of */
	/* setjmp(), longjmp(), pthread_clear_exit_np(), and */
	/* pthread_setcancelstate() to handle thread exits or */
	/* cancellation could result in looping or non-cancelable */
	/* threads if done incorrectly. */

	printf("In cancellation cleanup handler. Handling the thread exit\n");
	longjmp(*j, 1);
	printf("The exit/cancellation was not stopped!\n");

	return;
}

void *threadfunc(void *parm)
{
	jmp_buf j;
	int rc, old;

	printf("Inside secondary thread\n");
	if (setjmp(j)) {
		/* Returned from longjmp after stopping the thread exit */
		/* Since longjmp was called from within the cancellation */
		/* cleanup handler, we must clear the exit state of the */
		/* thread and reset the cancelability state to what it was */
		/* before the cancellation cleanup handlers were invoked */
		/* (Cancellation cleanup handlers are invoked with */
		/* thread cancellation disabled) */

		printf("Stopped the thread exit, now clean up the states\n");
		printf("Clear exit state\n");
		rc = pthread_clear_exit_np();
		checkResults("pthread_clear_exit_np()\n", rc);

		printf("Restore cancel state\n");
		rc = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old);
		checkResults("pthread_setcancelstate()\n", rc);

		/* This example was successful */
		threadStatus = 0;
	}
	else {
		printf("Pushing cleanup handler that will stop the exit\n");
		pthread_cleanup_push(cleanupHandler, &j);
		/* This exit will be stopped by cleanupHandler2 and the */
		/* pthread_clear_exit_np() that is done above */
		pthread_exit(__VOID(threadStatus));

		printf("Did not expect to get here! Left status as 1.\n");
		pthread_cleanup_pop(0);
		pthread_clear_exit_np()--Clear Exit Status of Thread
	}

	pthread_exit(__VOID(threadStatus));
}

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc=0;
	char c;
	void *status;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create thread that will demonstrate handling an exit\n");
	rc = pthread_create(&thread, NULL, threadfunc, NULL);
	checkResults("pthread_create()\n", rc);

	rc = pthread_join(thread, &status);
	checkResults("pthread_join()\n", rc);

	if (__INT(status) != 0) {
		printf("Got an unexpected return status from the thread!\n");
		exit(1);
	}

	printf("Main completed\n");

	return 0;
}


// Output:
// Enter Testcase - 
// Create thread that will demonstrate handling an exit
// Inside secondary thread
// Pushing cleanup handler that will stop the exit
// In cancellation cleanup handler. Handling the thread exit
// Stopped the thread exit, now clean up the states
// Clear exit state
// Restore cancel state
// Main completed

