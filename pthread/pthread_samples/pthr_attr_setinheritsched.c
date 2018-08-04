// pthread_attr_setinheritsched() Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include <except.h>
#include "check.h"

void showInheritSched(pthread_attr_t *attr) {
	int rc;
	int inheritsched;

	rc = pthread_attr_getinheritsched(attr, &inheritsched);
	checkResults("pthread_attr_getinheritsched()\n", rc);

	switch(inheritsched) {
	case PTHREAD_EXPLICIT_SCHED:
		printf("Inherit Sched - PTHREAD_EXPLICIT_SCHED\n");
		break;
	case PTHREAD_INHERIT_SCHED:
		printf("Inherit Sched - PTHREAD_INHERIT_SCHED\n");
		break;
	default:
		printf("Invalid inheritsched attribute!\n");
		exit(1);
	}

	return;
}

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc=0;
	pthread_attr_t attr;
	char c;
	void *status;

	printf("Enter Testcase - %s\n", argv[0]);
	rc = pthread_attr_init(&attr);
	checkResults("pthread_attr_init()\n", rc);

	showInheritSched(&attr);

	rc = pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
	checkResults("pthread_attr_setinheritsched()\n", rc);

	showInheritSched(&attr);

	rc = pthread_attr_destroy(&attr);
	checkResults("pthread_attr_destroy()\n", rc);
	printf("Main completed\n");

	return 0;
}

// Output:
// Enter Testcase - QP0WTEST/TPSIS0
// Inherit Sched - PTHREAD_EXPLICIT_SCHED
// Inherit Sched - PTHREAD_INHERIT_SCHED
// Main completed

