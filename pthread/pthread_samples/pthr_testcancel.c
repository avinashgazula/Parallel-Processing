
// pthread_testcancel()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

void cleanupHandler(void *parm) {
	printf("Inside cancellation cleanup handler\n");
}

void *threadfunc(void *parm)
{
	unsigned int i=0;
	int rc=0, oldState=0;

	printf("Entered secondary thread\n");
	pthread_cleanup_push(cleanupHandler, NULL);
	rc = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldState);
	checkResults("pthread_setcancelstate()\n", rc);

	/* Allow cancel to be pending on this thread */
	sleep(2);

	while (1) {
		printf("Secondary thread is now looping\n");
		++i;
		sleep(1);

		/* pthread_testcancel() has no effect until cancelability is enabled.*/
		/* At that time, a call to pthread_testcancel() should result in the */
		/* pending cancel being acted upon */

		pthread_testcancel();

		if (i == 5) {
			printf("Cancel state set to ENABLE\n");
			rc = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,&oldState);
			checkResults("pthread_setcancelstate(2)\n", rc);

			/* Now, cancellation points will allow pending cancels
			to get through to this thread */
		}
	} /* infinite */

	pthread_cleanup_pop(0);
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc=0;
	void *status=NULL;

	printf("Enter Testcase - %s\n", argv[0]);
	/* Create a thread using default attributes */
	printf("Create thread using the NULL attributes\n");
	rc = pthread_create(&thread, NULL, threadfunc, NULL);
	checkResults("pthread_create(NULL)\n", rc);

	sleep(1);
	printf("Cancel the thread\n");
	rc = pthread_cancel(thread);
	checkResults("pthread_cancel()\n", rc);

	rc = pthread_join(thread, &status);
	if (status != PTHREAD_CANCELED) {
		printf("Thread returned unexpected result!\n");
		exit(1);
	}

	printf("Main completed\n");
	return 0;
}

// Output:
// Enter Testcase - 
// Create thread using the NULL attributes
// Entered secondary thread
// Cancel the thread
// Secondary thread is now looping
// Secondary thread is now looping
// Secondary thread is now looping
// Secondary thread is now looping
// Secondary thread is now looping
// Cancel state set to ENABLE
// Secondary thread is now looping
// Inside cancellation cleanup handler
// Main completed


