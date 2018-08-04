#include <stdio.h>

int main(){
	
	int n;

	#pragma omp parallel
	#pragma omp for schedule(dynamic)
		for(n=0; n<16; ++n)
			printf(" %d thread num is %d\n", n, omp_get_thread_num());
//			printf(" %d", n);
	printf(".\n");
}

/*
	Try specifying chunk size,
	like #pragma omp for schedule(dynamic, 3)
*/