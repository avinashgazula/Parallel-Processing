
// pthread_is_initialthread_np()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

#define NUMTHREADS 1

void *function(void *parm)
{
	printf("Inside the function\n");

	if (pthread_is_initialthread_np()) {
		printf("In the initial thread\n");
	}
	else {
		printf("In a secondary thread\n");
	}

	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t thread[NUMTHREADS];
	int rc=0;
	int i=0;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create %d threads\n", NUMTHREADS);

	for (i=0; i<NUMTHREADS; ++i) {
		rc = pthread_create(&thread[i], NULL, function, NULL);
		checkResults("pthread_create()\n", rc);

		printf("Main: Currently %d threads\n",
		pthread_is_initialthread_np() + 1);
	}

	printf("Join to threads\n");
	for (i=0; i<NUMTHREADS; ++i) {
		rc = pthread_join(thread[i], NULL);
		checkResults("pthread_join()\n", rc);
	}

	function(NULL);
	printf("Main completed\n");

	return 0;
}

// Output:
// Enter Testcase - 
// Create 1 threads
// Join to threads
// Inside the function
// In a secondary thread
// Inside the function
// In the initial thread
// Main completed


