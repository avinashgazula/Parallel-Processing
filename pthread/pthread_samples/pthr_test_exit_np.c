
// pthread_test_exit_np()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

int checkStatusFailed1=0;
int missedHandler1=1;
int thread1Status=42;

void cleanupHandler1(void *arg)
{
	int rc;
	void *status;

	printf("Thread 1 - cleanup handler\n");
	missedHandler1=0;
	rc = pthread_test_exit_np(&status);

	if (rc != PTHREAD_STATUS_EXIT_NP) {
		printf("Thread 1 - returned %d instead "
			"of PTHREAD_STATUS_EXIT_NP\n", rc);
		checkStatusFailed1 = 1;
		return;
	}

	if (__INT(status) != thread1Status) {
		printf("Thread 1 - status = %d\n"
			"Thread 1 - expected %d\n",
			__INT(status), thread1Status);
		checkStatusFailed1=1;
	}

	printf("Thread 1 - correctly got PTHREAD_STATUS_EXIT_NP "
		"and thread exit status of %d\n", thread1Status);
}

void *thread1func(void *parm)
{
	printf("Thread 1 - Entered\n");
	pthread_cleanup_push(cleanupHandler1, NULL);
	pthread_exit(__VOID(thread1Status));
	pthread_cleanup_pop(0);
	return __VOID(0);
}

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc=0;
	void *status;

	printf("Enter Testcase - %s\n", argv[0]);
	rc = pthread_test_exit_np(&status);

	if (rc != PTHREAD_STATUS_ACTIVE_NP) {
		printf("We should always be in an ACTIVE status here! rc=%d\n", rc);
		exit(1);
	}

	printf("Create the pthread_exit thread\n");
	rc = pthread_create(&thread, NULL, thread1func, NULL);
	checkResults("pthread_create()\n", rc);

	rc = pthread_join(thread, &status);
	checkResults("pthread_join()\n", rc);

	if (__INT(status) != thread1Status) {
		printf("Wrong status from thread 1\n");
	}

	if (checkStatusFailed1 || missedHandler1) {
		printf("The thread did not complete its test correctly! "
			" check=%d, missed=%d\n",
		checkStatusFailed1, missedHandler1);
		exit(1);
	}

	printf("Main completed\n");
	return 0;
}

// Output:
// Enter Testcase - 
// Create the pthread_exit thread
// Thread 1 - Entered
// Thread 1 - cleanup handler
// Thread 1 - correctly got PTHREAD_STATUS_EXIT_NP and thread exit status of 42
// Main completed


