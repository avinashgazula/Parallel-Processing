
// pthread_cond_destroy()-- Example
#include <pthread.h>
#include <stdio.h>
#include "check.h"

pthread_cond_t cond;

int main(int argc, char **argv)
{
	int rc=0;
	pthread_mutexattr_t attr;

	printf("Entering testcase\n");
	printf("Create the condition using the condition attributes object\n");
	rc = pthread_cond_init(&cond, NULL);
	checkResults("pthread_cond_init()\n", rc);

	printf("- At this point, the condition with its default attributes\n");
	printf("- Can be used from any threads that want to use it\n");
	printf("Destroy condition\n");
	rc = pthread_cond_destroy(&cond);
	checkResults("pthread_cond_destroy()\n", rc);

	printf("Main completed\n");
	return 0;
}

// Output:
// Entering testcase
// Create the condition using the condition attributes object
// - At this point, the condition with its default attributes
// - Can be used from any threads that want to use it
// Destroy condition
// Main completed



