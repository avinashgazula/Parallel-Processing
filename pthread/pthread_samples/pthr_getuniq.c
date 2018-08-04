
// pthread_getunique_np()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

#define NUMTHREADS 3

void *threadfunc(void *parm)
{
	pthread_id_np_t tid;
	pthread_t me = pthread_self();
	pthread_getunique_np(&me, &tid);
	printf("Thread 0x%.8x %.8x started\n", tid);
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t thread[NUMTHREADS];
	int rc=0;
	pthread_id_np_t tid;
	int i=0;
	pthread_t me = pthread_self();

	printf("Enter Testcase - %s\n", argv[0]);
	pthread_getunique_np(&me, &tid);
	printf("Main Thread 0x%.8x %.8x\n", tid);
	printf("Create %d threads using joinable attributes\n", NUMTHREADS);

	for (i=0; i<NUMTHREADS; ++i) {
		rc = pthread_create(&thread[i], NULL, threadfunc, NULL);
		checkResults("pthread_create()\n", rc);

		pthread_getunique_np(&thread[i], &tid);
		printf("Created thread 0x%.8x %.8x\n", tid);
	}

	printf("Join to threads\n");
	for (i=0; i<NUMTHREADS; ++i) {
		rc = pthread_join(thread[i], NULL);
		checkResults("pthread_join()\n", rc);
	}

	printf("Main completed\n");
	return 0;
}


// Output:
// Enter Testcase - 
// Main Thread 0x00000000 0000006c
// Create 3 threads using joinable attributes
// Created thread 0x00000000 0000006d
// Thread 0x00000000 0000006d started
// Created thread 0x00000000 0000006e
// Created thread 0x00000000 0000006f
// Join to threads
// Thread 0x00000000 0000006f started
// Thread 0x00000000 0000006e started
// Main completed



