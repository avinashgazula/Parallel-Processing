#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i, n = 5;
    int a;

	#pragma omp parallel for private(i,a)
	for (i=0; i<n; i++)
	{
    	a = i+1;
        printf("Thread %d has a value of a = %d for i = %d\n",
        	omp_get_thread_num(),a,i);
    } /*-- End of parallel for --*/

	return(0);
}
