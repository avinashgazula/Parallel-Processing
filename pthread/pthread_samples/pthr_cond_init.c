
// pthread_cond_init()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2;
pthread_cond_t cond3;

int main(int argc, char **argv)
{
	int rc=0;
	pthread_condattr_t attr;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create the default cond attributes object\n");
	rc = pthread_condattr_init(&attr);
	checkResults("pthread_condattr_init()\n", rc);

	printf("Create the all of the default conditions in different ways\n");
	rc = pthread_cond_init(&cond2, NULL);
	checkResults("pthread_cond_init()\n", rc);

	rc = pthread_cond_init(&cond3, &attr);
	checkResults("pthread_cond_init()\n", rc);

	printf("- At this point, the conditions with default attributes\n");
	printf("- Can be used from any threads that want to use them\n");
	printf("Cleanup\n");

	pthread_condattr_destroy(&attr);
	pthread_cond_destroy(&cond1);
	pthread_cond_destroy(&cond2);
	pthread_cond_destroy(&cond3);

	printf("Main completed\n");
	return 0;
}

// Output:
// Enter Testcase - 
// Create the default cond attributes object
// Create the all of the default conditions in different ways
// - At this point, the conditions with default attributes
// - Can be used from any threads that want to use them
// Cleanup
// Main completed



