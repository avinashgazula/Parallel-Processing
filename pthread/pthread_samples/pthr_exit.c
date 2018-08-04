
// pthread_exit()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

int theStatus=5;

void *threadfunc(void *parm)
{
	printf("Inside secondary thread\n");
	pthread_exit(__VOID(theStatus));
	return __VOID(theStatus); /* Not needed, but this makes the compiler smile
	*/
}

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc=0;
	void *status;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create thread using attributes that allow join\n");
	rc = pthread_create(&thread, NULL, threadfunc, NULL);
	checkResults("pthread_create()\n", rc);

	printf("Wait for the thread to exit\n");
	rc = pthread_join(thread, &status);
	checkResults("pthread_join()\n", rc);

	if (__INT(status) != theStatus) {
		printf("Secondary thread failed\n");
		exit(1);
	}

	printf("Got secondary thread status as expected\n");
	printf("Main completed\n");

	return 0;
}

// Output:
// Enter Testcase - 
// Create thread using attributes that allow join
// Wait for the thread to exit
// Inside secondary thread
// Got secondary thread status as expected
// Main completed


