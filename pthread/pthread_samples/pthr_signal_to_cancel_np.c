
// pthread_signal_to_cancel_np()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include <sys/signal.h>
#include "check.h"

void sighand(int signo);

void cancellationCleanup(void *parm) { printf("Thread was canceled\n"); }

void *threadfunc(void *parm)
{
	pthread_t self = pthread_self();
	pthread_id_np_t tid;
	int rc;
	int i=5;

	pthread_getunique_np(&self, &tid);
	printf("Thread 0x%.8x %.8x entered\n", tid);

	while (i--) {
		printf("Thread 0x%.8x %.8x looping\n",
		tid, rc, errno);
		sleep(2);
		pthread_testcancel();
	}

	printf("Thread 0x%.8x %.8x did not expect to get here\n", tid);
	return NULL;
}

int main(int argc, char **argv)
{
	int rc;
	int i;
	pthread_t thread;
	struct sigaction actions;
	sigset_t mask;
	void *status;
	pthread_t self;
	pthread_id_np_t tid;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Set up the alarm handler for the process\n");

	memset(&actions, 0, sizeof(actions));
	sigemptyset(&actions.sa_mask);
	actions.sa_flags = 0;
	actions.sa_handler = sighand;
	rc = sigaction(SIGALRM,&actions,NULL);
	checkResults("sigaction\n", rc);

	printf("Block all signals in the parent so they can be inherited\n");
	sigfillset(&mask); /* Mask all allowed signals */
	rc = pthread_sigmask(SIG_BLOCK, &mask, NULL);
	checkResults("pthread_sigmask()\n", rc);

	printf("Create thread that inherits blocking mask\n");
	/* Thread will inherit blocking mask */
	rc = pthread_create(&thread, NULL, threadfunc, NULL);
	checkResults("pthread_create()\n", rc);

	/* Convert signals to cancels */
	rc = pthread_signal_to_cancel_np(&mask, &thread);
	checkResults("pthread_signal_to_cancel()\n", rc);

	sleep(3);
	self = pthread_self();
	pthread_getunique_np(&self, &tid);
	printf("Thread 0x%.8x %.8x sending a signal to the process\n", tid);

	kill(getpid(), SIGALRM);
	checkResults("kill()\n", rc);

	printf("Wait for masked and unmasked threads to complete\n");
	rc = pthread_join(thread, &status);
	checkResults("pthread_join()\n", rc);

	if (status != PTHREAD_CANCELED) {
		printf("Got an incorrect thread status\n");
		return 1;
	}

	printf("The target thread was canceled\n");
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
// Block all signals in the parent so they can be inherited
// Create thread that inherits blocking mask
// Thread 0x00000000 00000007 entered
// Thread 0x00000000 00000007 looping
// Thread 0x00000000 00000007 looping
// Thread 0x00000000 00000006 sending a signal to the process
// Wait for masked and unmasked threads to complete
// The target thread was canceled
// Main completed





