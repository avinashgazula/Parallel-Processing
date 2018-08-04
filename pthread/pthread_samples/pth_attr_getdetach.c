
// pthread_attr_getdetachstate() Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc=0;
	pthread_attr_t pta;
	int state;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create a thread attributes object\n");
	rc = pthread_attr_init(&pta);
	checkResults("pthread_attr_init()\n", rc);

	printf("Get detach state\n");
	rc = pthread_attr_getdetachstate(&pta, &state);
	checkResults("pthread_attr_getdetachstate()\n", rc);
	printf("The thread attributes object indicates: ");

	switch (state) {
	case PTHREAD_CREATE_DETACHED:
		printf("DETACHED\n");
		break;
	case PTHREAD_CREATE_JOINABLE:
		printf("JOINABLE\n");
		break;
	}

	printf("Destroy thread attributes object\n");
	rc = pthread_attr_destroy(&pta);
	checkResults("pthread_attr_destroy()\n", rc);
	printf("Main completed\n");
	return 0;
}

// Output:
// Enter Testcase - QP0WTEST/TAGDS0
// Create a thread attributes object
// Get detach state
// The thread attributes object indicates: JOINABLE
// Destroy thread attributes object
// Main completed


