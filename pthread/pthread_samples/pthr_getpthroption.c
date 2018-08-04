
// pthread_getpthreadoption_np()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

void *threadfunc(void *parm)
{
	printf("Inside the thread\n");
	return NULL;
}

void showCurrentSizeOfPool(void)
{
	int rc;
	pthread_option_np_t opt;

	memset(&opt, 0, sizeof(opt));
	opt.option = PTHREAD_OPTION_POOL_CURRENT_NP;
	rc = pthread_getpthreadoption_np(&opt);
	checkResults("pthread_getpthreadoption_np()\n", rc);

	printf("Current number of thread structures in pool is %d\n",
	opt.optionValue);
	return;
}

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc=0;
	pthread_option_np_t opt;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create thread using the NULL attributes\n");
	rc = pthread_create(&thread, NULL, threadfunc, NULL);
	checkResults("pthread_create(NULL)\n", rc);

	memset(&opt, 0, sizeof(opt));
	opt.option = PTHREAD_OPTION_POOL_NP;
	rc = pthread_getpthreadoption_np(&opt);
	checkResults("pthread_getpthreadoption_np()\n", rc);

	printf("Current maximum pool size is %d thread structures\n",
	opt.optionValue);
	showCurrentSizeOfPool();

	printf("Joining to the thread may it to the storage pool\n");
	rc = pthread_join(thread, NULL);
	checkResults("pthread_join()\n", rc);

	showCurrentSizeOfPool();

	printf("Main completed\n");
	return 0;
}

// Output:
// Enter Testcase - 
// Create thread using the NULL attributes
// Current maximum pool size is 512 thread structures
// Current number of thread structures in pool is 0
// Joining to the thread may it to the storage pool
// Inside the thread
// Current number of thread structures in pool is 1
// Main completed


