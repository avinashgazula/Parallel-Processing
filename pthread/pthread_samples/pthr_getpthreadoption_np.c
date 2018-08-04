
// pthread_getpthreadoption_np() - Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

#define NUMTHREADS 5

void *threadfunc(void *parm)
{
	printf("Inside the thread\n");
	return NULL;
}

void showCurrentSizeOfPool(void) {
	int rc;
	pthread_option_np_t opt;

	memset(&opt, 0, sizeof(opt));
	opt.option = PTHREAD_OPTION_POOL_CURRENT_NP;
	rc = pthread_getpthreadoption_np(&opt);
	checkResults("pthread_getpthreadoption_np()\n", rc);

	printf("Current number of thread structures in pool is %d\n", opt.optionValue);

	return;
}

int main(int argc, char **argv)
{
	pthread_t thread[NUMTHREADS];
	int rc=0;
	int i=0;
	pthread_option_np_t opt;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create threads and prime the storage pool\n");

	for (i=0; i<NUMTHREADS; ++i) {
		rc = pthread_create(&thread[i], NULL, threadfunc, NULL);
		checkResults("pthread_create(NULL)\n", rc);
	}

	printf("Joining all threads at once so thread n does not reuse\n"
		"thread n-1's data structures\n");

	for (i=0; i<NUMTHREADS; ++i) {
		rc = pthread_join(thread[i], NULL);
		checkResults("pthread_join()\n", rc);
	}

	showCurrentSizeOfPool();

	/* Set the maximum size of the storage pool to 0. I.e. No reuse of */
	/* pthread structures */
	printf("Set the max size of the storage pool to 0\n");
	memset(&opt, 0, sizeof(opt));
	opt.option = PTHREAD_OPTION_POOL_NP;
	opt.optionValue = 0;
	rc = pthread_setpthreadoption_np(&opt);
	checkResults("pthread_setpthreadoption_np()\n", rc);

	printf("Create some more threads. Each thread structure will come\n"
	"from the storage pool if it exists, but based on the max size of 0,\n"
	"the thread structure will not be allowed to be reused\n");

	for (i=0; i<NUMTHREADS; ++i) {
		rc = pthread_create(&thread[i], NULL, threadfunc, NULL);
		checkResults("pthread_create(NULL)\n", rc);

		showCurrentSizeOfPool();

		rc = pthread_join(thread[i], NULL);
		checkResults("pthread_join()\n", rc);
	}

	printf("Main completed\n");
	return 0;
}

// Output:
// Enter Testcase - 
// Create threads and prime the storage pool
// Joining all threads at once so thread n does not reuse
// thread n-1's data structures
// Inside the thread
// Inside the thread
// Inside the thread
// Inside the thread
// Inside the thread
// Current number of thread structures in pool is 5
// Set the max size of the storage pool to 0
// Create some more threads. Each thread structure will come
// from the storage pool if it exists, but based on the max size of 0,
// the thread structure will not be allowed to be reused
// Current number of thread structures in pool is 4
// Inside the thread
// Current number of thread structures in pool is 3
// Inside the thread
// Current number of thread structures in pool is 2
// Inside the thread
// Current number of thread structures in pool is 1
// Inside the thread
// Current number of thread structures in pool is 0
// Inside the thread
// Main completed

