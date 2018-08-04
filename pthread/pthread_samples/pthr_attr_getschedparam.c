

// int pthread_attr_getschedparam()  Example

#define _MULTI_THREADED
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include "check.h"

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc=0;
	pthread_attr_t pta;
	struct sched_param param;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create a thread attributes object\n");
	rc = pthread_attr_init(&pta);
	checkResults("pthread_attr_init()\n", rc);

	printf("Get scheduling parameters\n");
	rc = pthread_attr_getschedparam(&pta, &param);
	checkResults("pthread_attr_getschedparam()\n", rc);

	printf("The thread attributes object indicates: ");
	printf("priority %d\n", param.sched_priority);

	printf("Destroy thread attributes object\n");
	rc = pthread_attr_destroy(&pta);
	checkResults("pthread_attr_destroy()\n", rc);
	printf("Main completed\n");

	return 0;
}

// Output:
// Enter Testcase - QP0WTEST/TAGSP0
// Create a thread attributes object
// Get scheduling parameters
// The thread attributes object indicates: priority 0
// Destroy thread attributes object
// Main completed

