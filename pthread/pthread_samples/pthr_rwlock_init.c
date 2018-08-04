
// pthread_rwlock_init()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

pthread_rwlock_t rwlock;

void *rdlockThread(void *arg)
{
	int rc;

	printf("Entered thread, getting read lock\n");
	rc = pthread_rwlock_rdlock(&rwlock);
	checkResults("pthread_rwlock_rdlock()\n", rc);

	printf("got the rwlock read lock\n");
	sleep(5);
	printf("unlock the read lock\n");
	rc = pthread_rwlock_unlock(&rwlock);
	checkResults("pthread_rwlock_unlock()\n", rc);

	printf("Secondary thread unlocked\n");
	return NULL;
}

void *wrlockThread(void *arg)
{
	int rc;
	printf("Entered thread, getting write lock\n");
	rc = pthread_rwlock_wrlock(&rwlock);
	checkResults("pthread_rwlock_wrlock()\n", rc);

	printf("Got the rwlock write lock, now unlock\n");
	rc = pthread_rwlock_unlock(&rwlock);
	checkResults("pthread_rwlock_unlock()\n", rc);

	printf("Secondary thread unlocked\n");
	return NULL;
}

int main(int argc, char **argv)
{
	int rc=0;

	pthread_t thread, thread1;
	printf("Enter Testcase - %s\n", argv[0]);
	printf("Main, initialize the read write lock\n");
	rc = pthread_rwlock_init(&rwlock, NULL);
	checkResults("pthread_rwlock_init()\n", rc);

	printf("Main, grab a read lock\n");
	rc = pthread_rwlock_rdlock(&rwlock);
	checkResults("pthread_rwlock_rdlock()\n",rc);

	printf("Main, grab the same read lock again\n");
	rc = pthread_rwlock_rdlock(&rwlock);
	checkResults("pthread_rwlock_rdlock() second\n", rc);

	printf("Main, create the read lock thread\n");
	rc = pthread_create(&thread, NULL, rdlockThread, NULL);
	checkResults("pthread_create\n", rc);

	printf("Main - unlock the first read lock\n");
	rc = pthread_rwlock_unlock(&rwlock);
	checkResults("pthread_rwlock_unlock()\n", rc);

	printf("Main, create the write lock thread\n");
	rc = pthread_create(&thread1, NULL, wrlockThread, NULL);
	checkResults("pthread_create\n", rc);

	sleep(5);
	printf("Main - unlock the second read lock\n");
	rc = pthread_rwlock_unlock(&rwlock);
	checkResults("pthread_rwlock_unlock()\n", rc);

	printf("Main, wait for the threads\n");
	rc = pthread_join(thread, NULL);
	checkResults("pthread_join\n", rc);

	rc = pthread_join(thread1, NULL);
	checkResults("pthread_join\n", rc);

	rc = pthread_rwlock_destroy(&rwlock);
	checkResults("pthread_rwlock_destroy()\n", rc);

	printf("Main completed\n");
	return 0;
}

// Output:
// Enter Testcase - 
// Main, initialize the read write lock
// Main, grab a read lock
// Main, grab the same read lock again
// Main, create the read lock thread
// Main - unlock the first read lock
// Main, create the write lock thread
// Entered thread, getting read lock
// got the rwlock read lock
// Entered thread, getting write lock
// Main - unlock the second read lock
// Main, wait for the threads
// unlock the read lock
// Secondary thread unlocked
// Got the rwlock write lock, now unlock
// Secondary thread unlocked
// Main completed





