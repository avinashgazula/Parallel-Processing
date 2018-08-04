
// pthread_getschedparam()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include "check.h"

void *threadfunc(void *parm)
{
	printf("Inside secondary thread\n");
	sleep(5); /* Sleep is not a very robust way to serialize threads */
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc=0;
	struct sched_param param;
	int policy;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create thread using default attributes\n");
	rc = pthread_create(&thread, NULL, threadfunc, NULL);
	checkResults("pthread_create()\n", rc);

	printf("Get scheduling parameters\n");
	rc = pthread_getschedparam(thread, &policy, &param);
	checkResults("pthread_getschedparam()\n", rc);

	printf("The thread scheduling parameters indicate:\n policy = %d\n", policy);
	printf("priority = %d\n", param.sched_priority);
	printf("Main completed\n");

	return 0;
}

// Output:
// Enter Testcase - 
// Create thread using default attributes
// Get scheduling parameters
// The thread scheduling parameters indicate:
// policy = 0
// priority = 0
// Main completed


