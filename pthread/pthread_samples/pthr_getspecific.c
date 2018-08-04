
// pthread_getspecific()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

#define NUMTHREADS 3

pthread_key_t tlsKey = 0;

void globalDestructor(void *value)
{
	printf("In the globalDestructor\n");
	free(value);
	pthread_setspecific(tlsKey, NULL);
}

void showGlobal(void)
{
	void *global;
	pthread_id_np_t tid;

	global = pthread_getspecific(tlsKey);
	pthread_getunique_np((pthread_t *)global, &tid);
	printf("showGlobal: global data stored for thread 0x%.8x%.8x\n", tid);
}

void *threadfunc(void *parm)
{
	int rc;
	int *myThreadDataStructure;
	pthread_t me = pthread_self();

	printf("Inside secondary thread\n");
	myThreadDataStructure = malloc(sizeof(pthread_t) + sizeof(int) * 10);

	memcpy(myThreadDataStructure, &me, sizeof(pthread_t));
	pthread_setspecific(tlsKey, myThreadDataStructure);

	showGlobal();
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	pthread_t thread[NUMTHREADS];
	int rc=0;
	int i=0;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create a thread local storage key\n");
	rc = pthread_key_create(&tlsKey, globalDestructor);
	checkResults("pthread_key_create()\n", rc);

	/* The key can now be used from all threads */
	printf("Create %d threads using joinable attributes\n", NUMTHREADS);
	for (i=0; i<NUMTHREADS; ++i) {
		rc = pthread_create(&thread[i], NULL, threadfunc, NULL);
		checkResults("pthread_create()\n", rc);
	}

	printf("Join to threads\n");
	for (i=0; i<NUMTHREADS; ++i) {
		rc = pthread_join(thread[i], NULL);
		checkResults("pthread_join()\n", rc);
	}

	printf("Delete a thread local storage key\n");
	rc = pthread_key_delete(tlsKey);
	checkResults("pthread_key_delete()\n", rc);

	/* The key and any remaining values are now gone. */
	printf("Main completed\n");
	return 0;
}

// Output:
// Enter Testcase - 
// Create a thread local storage key
// Create 3 threads using joinable attributes
// Join to threads
// Inside secondary thread
// showGlobal: global data stored for thread 0x000000000000000b
// In the globalDestructor
// Inside secondary thread
// showGlobal: global data stored for thread 0x000000000000000d
// In the globalDestructor
// Inside secondary thread
// showGlobal: global data stored for thread 0x000000000000000c
// In the globalDestructor
// Delete a thread local storage key
// Main completed


