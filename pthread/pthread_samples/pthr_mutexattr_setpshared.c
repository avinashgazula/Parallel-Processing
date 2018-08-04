
// pthread_mutexattr_setpshared()-- Example
#include <pthread.h>
#include <stdio.h>
#include "check.h"

void showPshared(pthread_mutexattr_t *mta) {
	int rc;
	int pshared;

	printf("Check pshared attribute\n");
	rc = pthread_mutexattr_getpshared(mta, &pshared);
	checkResults("pthread_mutexattr_getpshared()\n", rc);

	printf("The pshared attributed is: ");
	switch (pshared) {
	case PTHREAD_PROCESS_PRIVATE:
		printf("PTHREAD_PROCESS_PRIVATE\n");
		break;
	case PTHREAD_PROCESS_SHARED:
		printf("PTHREAD_PROCESS_SHARED\n");
		break;
	default :
		printf("! pshared Error !\n");
		exit(1);
	}

	return;
}

int main(int argc, char **argv)
{
	int rc=0;
	pthread_mutexattr_t mta;
	int pshared=0;

	printf("Entering testcase\n");
	printf("Create a default mutex attribute\n");
	rc = pthread_mutexattr_init(&mta);
	checkResults("pthread_mutexattr_init()\n", rc);

	showPshared(&mta);

	printf("Change pshared attribute\n");
	rc = pthread_mutexattr_setpshared(&mta, PTHREAD_PROCESS_SHARED);
	checkResults("pthread_mutexattr_setpshared()\n", rc);

	showPshared(&mta);

	printf("Destroy mutex attribute\n");
	rc = pthread_mutexattr_destroy(&mta);
	checkResults("pthread_mutexattr_destroy()\n", rc);

	printf("Main completed\n");
	return 0;
}

// Output:
// Entering testcase
// Create a default mutex attribute
// Check pshared attribute
// The pshared attribute is: PTHREAD_PROCESS_PRIVATE
// Change pshared attribute
// The pshared attribute is: PTHREAD_PROCESS_SHARED
// Destroy mutex attribute
// Main completed




