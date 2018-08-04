
// pthread_extendedjoin_np()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "check.h"

static void *thread(void *parm)
{
	printf("Entered thread\n");
	sleep(10);

	printf("Ending thread\n");
	return __VOID(42);
}

int main (int argc, char *argv[])
{
	pthread_joinoption_np_t joinoption;
	void *status;
	int rc;
	pthread_t t;

	printf("Entering testcase %s\n", argv[0]);
	printf("Create thread using attributes that allow join\n");
	rc = pthread_create(&t, NULL, thread, NULL);
	checkResults("pthread_create()\n", rc);

	memset(&joinoption, 0, sizeof(pthread_joinoption_np_t));
	joinoption.deltatime.tv_sec = 3;
	joinoption.leaveThreadAllocated = 1;

	printf("Join to the thread, timeout in 3 seconds, no implicit detach\n");
	rc = pthread_extendedjoin_np(t, &status, &joinoption);

	if (rc != ETIMEDOUT) {
		printf("Join did not timeout as expected! rc=%d\n", rc);
		exit(1);
	}

	/* Call pthread_extendedjoin_np the same as a normal */
	/* pthread_join() call. */
	/* i.e. Implicit Detach is done, and Infinite wait */
	printf("Normal join to the thread\n");
	rc = pthread_extendedjoin_np(t, &status, NULL);
	checkResults("pthread_extendedjoin_np(no-options)\n", rc);

	if (__INT(status) != 42) {
		printf("Got the incorrect thread status!\n");
		exit(1);
	}
	printf("Main completed\n");

	return(0);
}

// Output
// Entering testcase QP0WTEST/TPJOINE0
// Create thread using attributes that allow join
// Join to the thread, timeout in 3 seconds, no implicit detach
// Entered thread
// Normal join to the thread
// Ending thread
// Main completed


