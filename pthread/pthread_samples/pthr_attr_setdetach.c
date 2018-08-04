


// pthread_attr_setdetachstate() Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

void showDetachState(pthread_attr_t *a)
{
	int rc=0;
	int state=0;

	printf("Get detach state\n");
	rc = pthread_attr_getdetachstate(a, &state);
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

	return;
}

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc=0;
	pthread_attr_t pta;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create a default thread attributes object\n");
	rc = pthread_attr_init(&pta);
	checkResults("pthread_attr_init()\n", rc);

	showDetachState(&pta);

	printf("Set the detach state\n");
	rc = pthread_attr_setdetachstate(&pta, PTHREAD_CREATE_DETACHED);
	checkResults("pthread_attr_setdetachstate()\n", rc);

	showDetachState(&pta);

	printf("Destroy thread attributes object\n");
	rc = pthread_attr_destroy(&pta);
	checkResults("pthread_attr_destroy()\n", rc);
	printf("Main completed\n");

	return 0;
}

// Output:
// Enter Testcase - QP0WTEST/TASDS0
// Create a default thread attributes object
// Get detach state
// The thread attributes object indicates: JOINABLE
// Set the detach state
// Get detach state
// The thread attributes object indicates: DETACHED
// Destroy thread attributes object
// Main completed

