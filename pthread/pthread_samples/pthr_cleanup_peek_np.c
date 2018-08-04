
// pthread_cleanup_peek_np()-- Example
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

void cleanupHandler1(void *arg) { printf("In Handler 1\n"); return; }
void cleanupHandler2(void *arg) { printf("In Handler 2\n"); return; }
void cleanupHandler3(void *arg) { printf("In Handler 3\n"); return; }

int args[3] = {0,0,0};

int main(int argc, char **argv)
{
	int rc=0;
	pthread_cleanup_entry_np_t entry;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Check for absence of cleanup handlers\n");
	rc = pthread_cleanup_peek_np(&entry);

	if (rc != ENOENT) {
		printf("pthread_cleanup_peek_np(), expected ENOENT\n");
		exit(1);
	}

	printf("Push some cancellation cleanup handlers\n");
	pthread_cleanup_push(cleanupHandler1, &args[0]);
	pthread_cleanup_push(cleanupHandler2, &args[1]);

	printf("Check for cleanupHandler2\n");
	rc = pthread_cleanup_peek_np(&entry);
	checkResults("pthread_cleanup_peek_np(2)\n", rc);

	if (entry.handler != cleanupHandler2 ||
		entry.arg != &args[1]) {
		printf("Did not get expected handler(2) information!\n");
		exit(1);
	}

	pthread_cleanup_push(cleanupHandler3, &args[2]);

	printf("Check for cleanupHandler3\n");
	rc = pthread_cleanup_peek_np(&entry);
	checkResults("pthread_cleanup_peek_np(3)\n", rc);

	if (entry.handler != cleanupHandler3 ||
		entry.arg != &args[2]) {
		printf("Did not get expected handler(3) information!\n");
		exit(1);
	}

	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	printf("Main completed\n");
	return 0;
}


// Output:
// Enter Testcase - 
// Check for absence of cleanup handlers
// Push some cancellation cleanup handlers
// Check for cleanupHandler2
// Check for cleanupHandler3
// Main completed


