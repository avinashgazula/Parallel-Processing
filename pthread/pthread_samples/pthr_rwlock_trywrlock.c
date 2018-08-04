
// pthread_rwlock_trywrlock()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void *wrlockThread(void *arg)
{
	int rc;
	int count=0;

	printf("%.8x %.8x: Entered thread, getting write lock with timeout\n",
	pthread_getthreadid_np());

Retry:
	rc = pthread_rwlock_trywrlock(&rwlock);
	if (rc == EBUSY) {
		if (count >= 10) {
			printf("%.8x %.8x: Retried too many times, failure!\n",
			pthread_getthreadid_np());
			exit(EXIT_FAILURE);
		}

		++count;
		printf("%.8x %.8x: Go off an do other work, then RETRY...\n",
		pthread_getthreadid_np());
		sleep(1);
		goto Retry;
	}

	checkResults("pthread_rwlock_trywrlock() 1\n", rc);
	printf("%.8x %.8x: Got the write lock\n", pthread_getthreadid_np());
	sleep(2);

	printf("%.8x %.8x: Unlock the write lock\n",
	pthread_getthreadid_np());
	rc = pthread_rwlock_unlock(&rwlock);
	checkResults("pthread_rwlock_unlock()\n", rc);

	printf("%.8x %.8x: Secondary thread complete\n",
	pthread_getthreadid_np());

	return NULL;
}

int main(int argc, char **argv)
{
	int rc=0;
	pthread_t thread, thread2;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Main, get the write lock\n");
	rc = pthread_rwlock_wrlock(&rwlock);
	checkResults("pthread_rwlock_wrlock()\n", rc);

	printf("Main, create the timed write lock threads\n");
	rc = pthread_create(&thread, NULL, wrlockThread, NULL);
	checkResults("pthread_create\n", rc);

	rc = pthread_create(&thread2, NULL, wrlockThread, NULL);
	checkResults("pthread_create\n", rc);

	printf("Main, wait a bit holding this write lock\n");
	sleep(1);

	printf("Main, Now unlock the write lock\n");
	rc = pthread_rwlock_unlock(&rwlock);
	checkResults("pthread_rwlock_unlock()\n", rc);

	printf("Main, wait for the threads to end\n");
	rc = pthread_join(thread, NULL);
	checkResults("pthread_join\n", rc);

	rc = pthread_join(thread2, NULL);
	checkResults("pthread_join\n", rc);

	rc = pthread_rwlock_destroy(&rwlock);
	checkResults("pthread_rwlock_destroy()\n", rc);

	printf("Main completed\n");
	return 0;
}

// Output:
// Enter Testcase - 
// Main, get the write lock
// Main, create the timed write lock threads
// 00000000 0000000d: Entered thread, getting write lock with timeout
// 00000000 0000000d: Go off an do other work, then RETRY...
// Main, wait a bit holding this write lock
// 00000000 0000000e: Entered thread, getting write lock with timeout
// 00000000 0000000e: Go off an do other work, then RETRY...
// 00000000 0000000d: Go off an do other work, then RETRY...
// Main, Now unlock the write lock
// Main, wait for the threads to end
// 00000000 0000000e: Got the write lock
// 00000000 0000000d: Go off an do other work, then RETRY...
// 00000000 0000000e: Unlock the write lock
// 00000000 0000000e: Secondary thread complete
// 00000000 0000000d: Got the write lock
// 00000000 0000000d: Unlock the write lock
// 00000000 0000000d: Secondary thread complete
// Main completed




