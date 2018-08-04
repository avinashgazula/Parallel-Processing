// pthread_delay_np()--Delay Thread for Requested
Example
#define _MULTI_THREADED
#include <stdio.h>
#include <qp0z1170.h>
#include <time.h>
#include <pthread.h>
#include "check.h"

#define NTHREADS 5

void *threadfunc(void *parm)
{
	int rc;
	struct timespec ts = {0, 0};

	/* 5 and 1/2 seconds */
	ts.tv_sec = 5;
	ts.tv_nsec = 500000000;

	printf("Thread blocked\n");
	rc = pthread_delay_np(&ts);

	if (rc != 0) {
		printf("pthread_delay_np() - return code %d\n", rc);
		return (void*)&rc;
	}

	printf("Wait timed out!\n");

	return NULL;
}

int main(int argc, char **argv)
{
	int rc=0;
	int i;
	pthread_t threadid[NTHREADS];
	void *status;
	int fail=0;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create %d threads\n", NTHREADS);

	for(i=0; i<NTHREADS; ++i) {
		rc = pthread_create(&threadid[i], NULL, threadfunc, NULL);
		checkResults("pthread_create()\n", rc);
	}

	printf("Wait for threads and cleanup\n");

	for (i=0; i<NTHREADS; ++i) {
		rc = pthread_join(threadid[i], &status);
		checkResults("pthread_join()\n", rc);

		if (status != NULL) {
			fail = 1;
		}
	}

	if (fail) {
		printf("At least one thread failed!\n");
		exit(1);
	}

	printf("Main completed\n");
	return 0;
}

// Output:
// Enter Testcase - 
// Create 5 threads
// Thread blocked
// Thread blocked
// Thread blocked
// Thread blocked
// Wait for threads and cleanup
// Thread blocked
// Wait timed out!
// Wait timed out!
// Wait timed out!
// Wait timed out!
// Wait timed out!
// Main completed

