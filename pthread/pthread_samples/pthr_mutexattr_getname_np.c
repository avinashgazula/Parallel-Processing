
// pthread_mutexattr_getname_np()-- Example
#include <pthread.h>
#include <stdio.h>
#include "check.h"

int main(int argc, char **argv)
{
	int rc=0;
	pthread_mutexattr_t mta;
	char mutexname[16];

	printf("Entering testcase\n");
	printf("Create a default mutex attribute\n");
	rc = pthread_mutexattr_init(&mta);
	checkResults("pthread_mutexattr_init\n", rc);

	memset(mutexname, 0, sizeof(mutexname));
	printf("Find out what the default name of the mutex is\n");
	rc = pthread_mutexattr_getname_np(&mta, mutexname);
	checkResults("pthread_mutexattr_getname_np()\n", rc);

	printf("The default mutex name will be: %.15s\n", mutexname);
	printf("- At this point, mutexes created with this attribute\n");
	printf("- will show up by name on many OS/400 debug and service
	screens\n");
	printf("- The default attribute contains a special automatically\n");
	printf("- incrementing name that changes for each mutex created in \n");
	printf("- the process\n");

	printf("Destroy mutex attribute\n");
	rc = pthread_mutexattr_destroy(&mta);
	checkResults("pthread_mutexattr_destroy()\n", rc);

	printf("Main completed\n");
	return 0;
}

// Output:
// Entering testcase
// Create a default mutex attribute
// Find out what the default name of the mutex is
// The default mutex name is: QP0WMTX UNNAMED
// - At this point, mutexes created with this attribute
// - will show up by name on many OS/400 debug and service screens
// - The default attribute contains a special automatically
// - incrementing name that changes for each mutex created in
// - the process
// Destroy mutex attribute
// Main completed


