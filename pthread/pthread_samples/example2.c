

// The following example shows an incorrectly written application.
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

pthread_t thread

void *threadfunc(void *parm)
{
	pthread_id_np_t tid;

#error "This is an ERROR."
#error "The 'thread' variable is shared between threads"
#error "and must be protected by a mutex."

	pthread_getunique_np(&thread, &tid);
	printf("Thread 0x%.8x %.8x started\n", tid);

	return NULL;
}

int main(int argc, char **argv)
{
	int rc=0;

	printf("Enter Testcase - %s\n", argv[0]);

#error "This is an ERROR."
#error "The order of thread thread startup, and return from"
#error "the pthread_create() API is NOT deterministic."

	rc = pthread_create(&thread, NULL, threadfunc, NULL);
	checkResults("pthread_create(NULL)\n", rc);

	/* sleep() isn't a very robust way to wait for the thread */
	sleep(5);
	printf("Main completed\n");

	return 0;
}



