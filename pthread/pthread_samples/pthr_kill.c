
// pthread_kill()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include <sys/signal.h>
#include "check.h"

#define NUMTHREADS 3

void sighand(int signo);

void *threadfunc(void *parm)
{
	pthread_t self = pthread_self();
	pthread_id_np_t tid;
	int rc;

	pthread_getunique_np(&self, &tid);
	printf("Thread 0x%.8x %.8x entered\n", tid);
	errno = 0;

	rc = sleep(30);
	if (rc != 0 && errno == EINTR) {
		printf("Thread 0x%.8x %.8x got a signal delivered to it\n", tid);
		return NULL;
	}

	printf("Thread 0x%.8x %.8x did not get expected results! rc=%d,
	errno=%d\n",
	tid, rc, errno);

	return NULL;
}
int main(int argc, char **argv)
{
	int rc;
	int i;
	struct sigaction actions;
	pthread_t threads[NUMTHREADS];

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Set up the alarm handler for the process\n");

	memset(&actions, 0, sizeof(actions));
	sigemptyset(&actions.sa_mask);
	actions.sa_flags = 0;
	actions.sa_handler = sighand;
	rc = sigaction(SIGALRM,&actions,NULL);
	checkResults("sigaction\n", rc);

	for(i=0; i<NUMTHREADS; ++i) {
		rc = pthread_create(&threads[i], NULL, threadfunc, NULL);
		checkResults("pthread_create()\n", rc);
	}

	sleep(3);

	for(i=0; i<NUMTHREADS; ++i) {
		rc = pthread_kill(threads[i], SIGALRM);
		checkResults("pthread_kill()\n", rc);
	}

	for(i=0; i<NUMTHREADS; ++i) {
		rc = pthread_join(threads[i], NULL);
		checkResults("pthread_join()\n", rc);
	}

	printf("Main completed\n");
	return 0;
}

void sighand(int signo)
{
	pthread_t self = pthread_self();
	pthread_id_np_t tid;

	pthread_getunique_np(&self, &tid);
	printf("Thread 0x%.8x %.8x in signal handler\n", tid);

	return;
}

// Output:
// Enter Testcase - 
// Set up the alarm handler for the process
// Thread 0x00000000 0000000c entered
// Thread 0x00000000 0000000d entered
// Thread 0x00000000 0000000e entered
// Thread 0x00000000 0000000c in signal handler
// Thread 0x00000000 0000000c got a signal delivered to it
// Thread 0x00000000 0000000d in signal handler
// Thread 0x00000000 0000000d got a signal delivered to it
// Thread 0x00000000 0000000e in signal handler
// Thread 0x00000000 0000000e got a signal delivered to it
// Main completed




