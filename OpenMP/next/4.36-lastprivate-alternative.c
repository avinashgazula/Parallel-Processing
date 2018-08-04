#include <stdio.h>
#include <stdlib.h>

int main()
{
   int i, n = 5;
   int a, a_shared;

   #pragma omp parallel for private(i) private(a) shared(a_shared)
   for (i=0; i<n; i++)
   {
       a = i+1;
       printf("Thread %d has a value of a = %d for i = %d\n",
              omp_get_thread_num(),a,i);
       if ( i == n-1 ) a_shared = a;
   } /*-- End of parallel for --*/

   printf("Value of a after parallel for: a_shared = %d\n",a_shared);

   return(0);
}
