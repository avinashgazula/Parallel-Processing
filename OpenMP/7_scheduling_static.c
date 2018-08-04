 #include <stdio.h>

int main(){
	
	int n;

	#pragma omp parallel
	#pragma omp for schedule(static)
		for(n=0; n<16; ++n)
			printf(" %d thread num is %d\n", n, omp_get_thread_num());
//			printf(" %d", n);
	printf(".\n");
}

/*
	run static and dynamic and see the difference
*/