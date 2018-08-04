
// pthread_is_multithreaded_np()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

#define NUMTHREADS 3

void *threadfunc(void *parm)
{
	int myHiId;
	int myId;
	pthread_t me = pthread_self();

	printf("Inside the New Thread\n");
	sleep(2); /* Sleep is not a very robust way to serialize threads */

	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t thread[NUMTHREADS];
	int rc=0;
	int theHiId=0;
	int theId=0;
	int i=0;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create %d threads\n", NUMTHREADS);

	for (i=0; i<NUMTHREADS; ++i) {
		rc = pthread_create(&thread[i], NULL, threadfunc, NULL);
		checkResults("pthread_create()\n", rc);

		printf("Main: Currently %d threads\n",
		pthread_is_multithreaded_np() + 1);
	}

	printf("Join to threads\n");

	for (i=0; i<NUMTHREADS; ++i) {
		rc = pthread_join(thread[i], NULL);
		checkResults("pthread_join()\n", rc);
	}

	if (rc = pthread_is_multithreaded_np()) {
		printf("Error: %d Threads still exist!\n", rc+1);
		exit(1);
	}

	printf("Main completed\n");
	return 0;
}

// Output:
// Enter Testcase - 
// Create 3 threads
// Main: Currently 2 threads
// Main: Currently 3 threads
// Main: Currently 4 threads
// Join to threads
// Inside the New Thread
// Inside the New Thread
// Inside the New Thread
// Main completed



