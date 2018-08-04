
// pthread_once()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

#define NUMTHREADS 3

int number = 0;
int okStatus = 777;
pthread_once_t onceControl = PTHREAD_ONCE_INIT;

void initRoutine(void)
{
	printf("In the initRoutine\n");
	number++;
}

void *threadfunc(void *parm)
{
	printf("Inside secondary thread\n");
	pthread_once(&onceControl, initRoutine);
	return __VOID(okStatus);
}

int main(int argc, char **argv)
{
	pthread_t thread[NUMTHREADS];
	int rc=0;
	int i=NUMTHREADS;
	void *status;

	printf("Enter Testcase - %s\n", argv[0]);
	for (i=0; i < NUMTHREADS; ++i) {
		printf("Create thread %d\n", i);
		rc = pthread_create(&thread[i], NULL, threadfunc, NULL);
		checkResults("pthread_create()\n", rc);
	}

	for (i=0; i < NUMTHREADS; ++i) {
		printf("Wait for thread %d\n", i);
		rc = pthread_join(thread[i], &status);
		checkResults("pthread_join()\n", rc);

		if (__INT(status) != okStatus) {
			printf("Secondary thread failed\n");
			exit(1);
		}
	}

	if (number != 1) {
		printf("An incorrect number of 1 one-time init routine was called!\n");
		exit(1);
	}

	printf("One-time init routine called exactly once\n");
	printf("Main completed\n");
	return 0;
}

// Output:
// Enter Testcase - 
// Create thread 0
// Create thread 1
// Create thread 2
// Wait for thread 0
// Inside secondary thread
// In the initRoutine
// Inside secondary thread
// Wait for thread 1
// Wait for thread 2
// Inside secondary thread
// One-time init routine called exactly once
// Main completed


