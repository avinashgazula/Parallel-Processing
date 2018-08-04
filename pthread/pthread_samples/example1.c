
// The following example shows the correct way to store and retrieve integer information in pointer variables.
#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include "check.h"

int main(int argc, char **argv)
{
	void *status1 = __VOID(5);
	void *status2 = __VOID(999);

	if (status1 == NULL) {
		printf("Status1 pointer is NULL\n");
	}
	else {
		printf("Status1 pointer is non-NULL\n");
	}
	if (status1 == status2) {
		printf("Both status variables as pointers are equal\n");
	}
	else {
		if (status1 < status2) {
			printf("Status1 is greater than status2\n");
		}
		else {
			if (status1 < status2) {
				printf("Status1 is less then status2\n");
			}
			else {
				printf("The pointers are unordered!\n");
			}
		}
	}

	printf("Pointer values stored in status variables are:\n"
	" status1 = %.8x %.8x %.8x %.8x\n"
	" status2 = %.8x %.8x %.8x %.8x\n",
	status1, status2);

	printf("Integer values stored in status variables are:\n"
		" status1 = %d\n"
		" status2 = %d\n",
		__INT(status1), __INT(status2));

	return;
}


// Output:
// Status1 pointer is non-NULL
// Status1 is less then status2
// Pointer values stored in status variables are:
// status1 = 80000000 00000000 00008302 00000005
// status2 = 80000000 00000000 00008302 000003e7
// Integer values stored in status variables are:
// status1 = 5
// status2 = 999


