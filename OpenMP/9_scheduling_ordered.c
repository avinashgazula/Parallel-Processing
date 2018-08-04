
#include <stdio.h>

int main(){
	
	int n;
	
	#pragma omp parallel
	#pragma omp for ordered schedule(dynamic)
	for(n=0; n<10; ++n)
	{
		printf("unordered %d\t",n);
		#pragma omp ordered
		printf("ordered %d\n",n);
	}

}

