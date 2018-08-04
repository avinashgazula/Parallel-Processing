#include <stdio.h>

int main(){
	
	int i;

	#pragma omp parallel
	{
		#pragma omp for
		for(i=0; i<10; ++i)			printf(" %d", i);
	}
	printf(".\n");

}

/*Try replacing main body with following code
	int i;

	#pragma omp parallel for 		//alternate shorthand method to use parallel and for directice together
		for(i=0; i<10; ++i)
			printf(" %d", i);

	printf(".\n");

*/
