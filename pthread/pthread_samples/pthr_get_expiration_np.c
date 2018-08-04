
// pthread_get_expiration_np()-- Example
#define _MULTI_THREADED
#include <stdio.h>
#include <qp0z1170.h>
#include <time.h>
#include <pthread.h>
#include "check.h"

/* For safe condition variable usage, must use a boolean predicate and */
/* a mutex with the condition. */
int workToDo = 0;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int failStatus=99;

#define NTHREADS 2
#define WAIT_TIME_SECONDS 3

void *threadfunc(void *parm)
{
	int rc;
	struct timespec delta;
	struct timespec abstime;
	int retries = 2;
	pthread_id_np_t tid;

	tid = pthread_getthreadid_np();
	rc = pthread_mutex_lock(&mutex);
	checkResults("pthread_mutex_lock()\n", rc);

	while (retries--) {
		delta.tv_sec = WAIT_TIME_SECONDS;
		delta.tv_nsec = 0;
		rc = pthread_get_expiration_np(&delta, &abstime);
		checkResults("pthread_get_expiration_np()\n", rc);

		while (!workToDo) {
			printf("Thread 0x%.8x %.8x blocked\n", tid);
			rc = pthread_cond_timedwait(&cond, &mutex, &abstime);

			if (rc != ETIMEDOUT) {
				printf("pthread_cond_timedwait() - expect timeout %d\n", rc);
				rc = pthread_mutex_unlock(&mutex);
				checkResults("pthread_mutex_lock()\n", rc);

				return __VOID(failStatus);
			}

			/* Since there is no code in this example to wake up any */
			/* thread on the condition variable, we know we are done */
			/* because we have timed out. */
			break;
		}

		printf("Wait timed out! tid=0x%.8x %.8x\n", tid);
	}

	rc = pthread_mutex_unlock(&mutex);
	checkResults("pthread_mutex_lock()\n", rc);

	return __VOID(0);
}

int main(int argc, char **argv)
{
	int rc=0;
	int i;
	pthread_t threadid[NTHREADS];
	void *status;
	int results=0;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create %d threads\n", NTHREADS);

	for(i=0; i<NTHREADS; ++i) {
		rc = pthread_create(&threadid[i], NULL, threadfunc, NULL);
		checkResults("pthread_create()\n", rc);
	}

	printf("Wait for threads and cleanup\n");
	for (i=0; i<NTHREADS; ++i) {
		rc = pthread_join(threadid[i], &status);
		checkResults("pthread_join()\n", rc);

		if (__INT(status) == failStatus) {
			printf("A thread failed!\n");
			results++;
		}
	}

	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
	printf("Main completed\n");

	return results;
}

// Output:
// Enter Testcase - 
// Create 2 threads
// Wait for threads and cleanup
// Thread 0x00000000 000002ab blocked
// Thread 0x00000000 000002ac blocked
// Wait timed out! tid=0x00000000 000002ab
// Thread 0x00000000 000002ab blocked
// Wait timed out! tid=0x00000000 000002ac
// Thread 0x00000000 000002ac blocked
// Wait timed out! tid=0x00000000 000002ab
// Wait timed out! tid=0x00000000 000002ac
// Main completed




