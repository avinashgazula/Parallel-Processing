// pthread_attr_destroy() Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

void *threadfunc(void *parm) {
	printf("Thread created using an default attributes\n");
	return NULL;
}

int main(int argc, char **argv) {
	pthread_t thread;
	int rc=0;
	pthread_attr_t pta;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create a thread attributes object\n");

	rc = pthread_attr_init(&pta);
	checkResults("pthread_attr_init()\n", rc);

	printf("Create a thread using the attributes object\n");
	rc = pthread_create(&thread, &pta, threadfunc, NULL);
	checkResults("pthread_create()\n", rc);

	printf("Create a thread using the default attributes\n");
	rc = pthread_create(&thread, NULL, threadfunc, NULL);
	checkResults("pthread_create()\n", rc);

	printf("Destroy thread attributes object\n");
	rc = pthread_attr_destroy(&pta);
	checkResults("pthread_attr_destroy()\n", rc);

	/* sleep() is not a very robust way to wait for the thread */
	sleep(5);
	printf("Main completed\n");

	return 0;
}

// Output:
// Enter Testcase - .... 
// Create a thread attributes object
// Create a thread using the attributes object
// Create a thread using the default attributes
// Destroy thread attributes object
// Thread created using an default attributes
// Thread created using an default attributes
// Main completed

