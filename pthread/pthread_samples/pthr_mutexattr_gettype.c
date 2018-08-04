
// pthread_mutexattr_gettype()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

int showType(pthread_mutexattr_t *mta) {
	int rc;
	int type;

	printf("Check type attribute\n");
	rc = pthread_mutexattr_gettype(mta, &type);
	checkResults("pthread_mutexattr_gettype()\n", rc);

	printf("The type attributed is: ");
	switch (type) {
	case PTHREAD_MUTEX_NORMAL:
		printf("PTHREAD_MUTEX_NORMAL (DEFAULT)\n");
		break;
	case PTHREAD_MUTEX_RECURSIVE:
		printf("PTHREAD_MUTEX_RECURSIVE\n");
		break;
	case PTHREAD_MUTEX_ERRORCHECK:
		printf("PTHREAD_MUTEX_ERRORCHECK\n");
		break;
	case PTHREAD_MUTEX_OWNERTERM_NP:
		printf("PTHREAD_MUTEX_OWNERTERM_NP\n");
		break;
	default :
		printf("! type Error type=%d !\n", type);
		exit(1);
	}

	return type;
}

int main(int argc, char **argv)
{
	int rc=0;
	pthread_mutexattr_t mta;
	int type=0;
	pthread_mutex_t mutex;
	struct timespec ts;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create a default mutex attribute\n");
	rc = pthread_mutexattr_init(&mta);
	checkResults("pthread_mutexattr_init()\n", rc);

	printf("Change mutex type attribute to recursive\n");
	rc = pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_RECURSIVE);
	checkResults("pthread_mutexattr_settype()\n", rc);

	showType(&mta);

	rc = pthread_mutexattr_setname_np(&mta, "RECURSIVE ONE");
	checkResults("pthread_mutexattr_setname_np()\n", rc);

	printf("Create the named, recursive mutex\n");
	rc = pthread_mutex_init(&mutex, &mta);
	checkResults("pthread_mutex_init()\n", rc);

	printf("Lock the named, recursive mutex\n");
	rc = pthread_mutex_lock(&mutex);
	checkResults("pthread_mutex_lock() 1\n", rc);

	printf("ReLock the named, recursive mutex\n");
	rc = pthread_mutex_lock(&mutex);
	checkResults("pthread_mutex_lock() 2\n", rc);

	printf("Trylock the named, recursive mutex\n");
	rc = pthread_mutex_trylock(&mutex);
	checkResults("pthread_mutex_trylock()\n", rc);

	printf("Timedlock the named, recursive mutex\n");
	ts.tv_sec = 5;
	ts.tv_nsec = 0;
	rc = pthread_mutex_timedlock_np(&mutex, &ts);
	checkResults("pthread_mutex_timedlock_np()\n", rc);

	printf("Sleeping for a short time holding the recurive mutex\n");
	printf("Use DSPJOB, option 19 to see the held mutex\n");
	sleep(30);

	printf("Unlock the mutex 4 times\n");
	rc = pthread_mutex_unlock(&mutex);
	checkResults("pthread_mutex_unlock() 1\n", rc);

	rc = pthread_mutex_unlock(&mutex);
	checkResults("pthread_mutex_unlock() 2\n", rc);

	rc = pthread_mutex_unlock(&mutex);
	checkResults("pthread_mutex_unlock() 3\n", rc);

	rc = pthread_mutex_unlock(&mutex);
	checkResults("pthread_mutex_unlock() 4\n", rc);

	printf("Cleanup\n");
	rc = pthread_mutex_destroy(&mutex);
	checkResults("pthread_mutex_destroy()\n", rc);

	rc = pthread_mutexattr_destroy(&mta);
	checkResults("pthread_mutexattr_destroy()\n", rc);

	printf("Main completed\n");
	return 0;
}

// Output
// Enter Testcase - 
// Create a default mutex attribute
// Change mutex type attribute to recursive
// Check type attribute
// The type attributed is: PTHREAD_MUTEX_RECURSIVE
// Create the named, recursive mutex
// Lock the named, recursive mutex
// ReLock the named, recursive mutex
// Trylock the named, recursive mutex
// Timedlock the named, recursive mutex
// Sleeping for a short time holding the recurive mutex
// Use DSPJOB, option 19 to see the held mutex
// Unlock the mutex 4 times
// Cleanup
// Main completed


