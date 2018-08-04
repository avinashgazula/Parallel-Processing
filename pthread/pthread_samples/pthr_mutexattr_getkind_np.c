
// pthread_mutexattr_getkind_np()-- Example
#include <pthread.h>
#include <stdio.h>
#include "check.h"

void showKind(pthread_mutexattr_t *mta) {
	int rc;
	int kind;

	printf("Check kind attribute\n");
	rc = pthread_mutexattr_getkind_np(mta, &kind);
	checkResults("pthread_mutexattr_getpshared()\n", rc);

	printf("The pshared attributed is: ");

	switch (kind) {
	case PTHREAD_MUTEX_NONRECURSIVE_NP:
		printf("PTHREAD_MUTEX_NONRECURSIVE_NP\n");
		break;
	case PTHREAD_MUTEX_RECURSIVE_NP:
		printf("PTHREAD_MUTEX_RECURSIVE_NP\n");
		break;
	default :
		printf("! kind Error kind=%d !\n", kind);
		exit(1);
	}

	return;
}

int main(int argc, char **argv)
{
	int rc=0;
	pthread_mutexattr_t mta;
	int pshared=0;

	printf("Enter Testcase - %s\n", argv[0]);
	printf("Create a default mutex attribute\n");
	rc = pthread_mutexattr_init(&mta);
	checkResults("pthread_mutexattr_init()\n", rc);

	showKind(&mta);

	printf("Change mutex kind attribute\n");
	rc = pthread_mutexattr_setkind_np(&mta, PTHREAD_MUTEX_RECURSIVE_NP);
	checkResults("pthread_mutexattr_setkind()\n", rc);

	showKind(&mta);

	printf("Destroy mutex attribute\n");
	rc = pthread_mutexattr_destroy(&mta);
	checkResults("pthread_mutexattr_destroy()\n", rc);

	printf("Main completed\n");
	return 0;
}

//  Output:
//  Enter Testcase - 
//  Create a default mutex attribute
//  Check kind attribute
//  The pshared attributed is:
//  PTHREAD_MUTEX_NONRECURSIVE_NP
//  Change mutex kind attribute
//  Check kind attribute
//  The pshared attributed is:
//  PTHREAD_MUTEX_RECURSIVE_NP
//  Destroy mutex attribute
//  Main completed


