
// pthread_rwlockattr_destroy()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

pthread_rwlock_t rwlock1;
pthread_rwlock_t rwlock2 = PTHREAD_RWLOCK_INITIALIZER;

int main(int argc, char **argv)
{
	int rc=0;
	pthread_rwlockattr_t attr;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create a default rwlock attribute\n");
	rc = pthread_rwlockattr_init(&attr);
	checkResults("pthread_rwlockattr_init()\n", rc);

	printf("Use the rwlock attributes to created rwlocks here\n");
	rc = pthread_rwlock_init(&rwlock1, &attr);
	checkResults("pthread_rwlock_init()\n", rc);

	printf("The rwlock1 is now ready for use.\n");
	printf("The rwlock2 that was statically initialized was ready when\n"
	"the main routine was entered\n");
	printf("Destroy rwlock attribute\n");
	rc = pthread_rwlockattr_destroy(&attr);
	checkResults("pthread_rwlockattr_destroy()\n", rc);

	printf("Use the rwlocks\n");
	rc = pthread_rwlock_rdlock(&rwlock1);
	checkResults("pthread_rwlock_rdlock()\n", rc);

	rc = pthread_rwlock_wrlock(&rwlock2);
	checkResults("pthread_rwlock_wrlock()\n", rc);

	rc = pthread_rwlock_unlock(&rwlock1);
	checkResults("pthread_rwlock_unlock(1)\n", rc);

	rc = pthread_rwlock_unlock(&rwlock2);
	checkResults("pthread_rwlock_unlock(2)\n", rc);

	printf("Destroy the rwlocks\n");
	rc = pthread_rwlock_destroy(&rwlock1);
	checkResults("pthread_rwlock_destroy(1)\n", rc);

	rc = pthread_rwlock_destroy(&rwlock2);
	checkResults("pthread_rwlock_destroy(2)\n", rc);

	printf("Main completed\n");
	return 0;
}

// Output:
// Enter Testcase - 
// Create a default rwlock attribute
// Use the rwlock attributes to created rwlocks here
// The rwlock is now ready for use.
// The rwlock that was statically initialized was ready when
// the main routine was entered
// Destroy rwlock attribute
// Use the rwlocks
// Destroy the rwlocks
// Main completed




