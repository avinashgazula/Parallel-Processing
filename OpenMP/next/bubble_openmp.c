#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{
    int i, n, tmp, *x, changes;
    int chunk;

    scanf("%d", &n);
    chunk = n / 4;
    x = (int*) malloc(n * sizeof(int));

    for(i = 0; i < n; ++i)
		x[i]=rand()%1000;

	printf("********************UNSorted Array********************\n");
	for(i = 0; i < n; ++i)
		printf("%d\t",x[i]);
	printf("\n");

    changes = 1;
    int nr = 0;
    while(changes)
    {
	    #pragma omp parallel private(tmp)
	    {
		    nr++;
		    changes = 0;
		    #pragma omp for \
		            reduction(+:changes)
		    for(i = 0; i < n - 1; i = i + 2)
		    {
		            if(x[i] > x[i+1] )
		            {
		                    tmp = x[i];
		                    x[i] = x[i+1];
		                    x[i+1] = tmp;
		                    ++changes;
		            }
		    }
		    #pragma omp for \
		            reduction(+:changes)
		    for(i = 1; i < n - 1; i = i + 2)
		    {
		            if( x[i] > x[i+1] )
		            {
		                    tmp = x[i];
		                    x[i] = x[i+1];
		                    x[i+1] = tmp;
		                    ++changes;
		            }
		    }
	    }

    }
	printf("********************Sorted Array********************\n");
    for(i = 0; i < n; ++i)
    	printf("%d\t",x[i]);
//	printf("x[%d] = %d\n",i,x[i]);
    printf("\n");
    return 0;
}
