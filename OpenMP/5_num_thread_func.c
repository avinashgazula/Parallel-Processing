/*
	controlling number of threads using "num_threads"
*/

#include <stdio.h>

int main(){
	
	int i;

	#pragma omp parallel num_threads(3)
	{
	// This code will be executed by three threads.

	// Chunks of this loop will be divided amongst
	// the (three) threads of the current team.
		#pragma omp for
		for(i=0; i<10; ++i)
			printf(" %d thread num is %d\n", i, omp_get_thread_num());
			//printf(" %d", i);
		
	}
	printf(".\n");

}