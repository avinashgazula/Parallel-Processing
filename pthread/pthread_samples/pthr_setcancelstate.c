
// pthread_setcancelstate()-- Example
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "check.h"

void *threadfunc(void *parm)
{
	int i = 0;

	printf("Entered secondary thread\n");
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

	while (1) {
		printf("Secondary thread is looping\n");
		pthread_testcancel();
		sleep(1);

		if (++i == 5) {
			/* Since default cancel type is deferred, changing the state */
			/* will allow the next cancellation point to cancel the thread */
			printf("Cancel state set to ENABLE\n");
			pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		}
	} /* infinite */

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
	sleep(3);
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
// Secondary thread is looping
// Cancel the thread
// Secondary thread is looping
// Secondary thread is looping
// Cancel state set to ENABLE
// Main completed



