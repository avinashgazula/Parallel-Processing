
// pthread_cond_timedwait()-- Example
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

#define NTHREADS 3
#define WAIT_TIME_SECONDS 15

void *threadfunc(void *parm)
{
	int rc;
	struct timespec ts;
	struct timeval tp;

	rc = pthread_mutex_lock(&mutex);
	checkResults("pthread_mutex_lock()\n", rc);

	/* Usually worker threads will loop on these operations */
	while (1) {
		rc = gettimeofday(&tp, NULL);
		checkResults("gettimeofday()\n", rc);

		/* Convert from timeval to timespec */
		ts.tv_sec = tp.tv_sec;
		ts.tv_nsec = tp.tv_usec * 1000;
		ts.tv_sec += WAIT_TIME_SECONDS;

		while (!workToDo) {
			printf("Thread blocked\n");
			rc = pthread_cond_timedwait(&cond, &mutex, &ts);

			/* If the wait timed out, in this example, the work is complete, and
			*/
			/* the thread will end.
			*/
			/* In reality, a timeout must be accompanied by some sort of checking
			*/
			/* to see if the work is REALLY all complete. In the simple example
			*/
			/* we will just go belly up when we time
			out. */

			if (rc == ETIMEDOUT) {
				printf("Wait timed out!\n");
				rc = pthread_mutex_unlock(&mutex);
				checkResults("pthread_mutex_lock()\n", rc);

				pthread_exit(NULL);
			}

			checkResults("pthread_cond_timedwait()\n", rc);
		}

		printf("Thread consumes work here\n");
		workToDo = 0;
	}

	rc = pthread_mutex_unlock(&mutex);
	checkResults("pthread_mutex_lock()\n", rc);

	return NULL;
}

int main(int argc, char **argv)
{
	int rc=0;
	int i;
	pthread_t threadid[NTHREADS];

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create %d threads\n", NTHREADS);
	for(i=0; i<NTHREADS; ++i) {
		rc = pthread_create(&threadid[i], NULL, threadfunc, NULL);
		checkResults("pthread_create()\n", rc);
	}

	rc = pthread_mutex_lock(&mutex);
	checkResults("pthread_mutex_lock()\n", rc);

	printf("One work item to give to a thread\n");
	workToDo = 1;
	rc = pthread_cond_signal(&cond);
	checkResults("pthread_cond_signal()\n", rc);

	rc = pthread_mutex_unlock(&mutex);
	checkResults("pthread_mutex_unlock()\n", rc);

	printf("Wait for threads and cleanup\n");
	for (i=0; i<NTHREADS; ++i) {
		rc = pthread_join(threadid[i], NULL);
		checkResults("pthread_join()\n", rc);
	}

	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
	printf("Main completed\n");

	return 0;
}

// Output:
// Enter Testcase - 
// Create 3 threads
// Thread blocked
// One work item to give to a thread
// Wait for threads and cleanup
// Thread consumes work here
// Thread blocked
// Thread blocked
// Thread blocked
// Wait timed out!
// Wait timed out!
// Wait timed out!
// Main completed




