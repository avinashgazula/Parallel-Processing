#include <stdio.h>
#include <stdlib.h>

int main()
{
   int i, a, n = 5;
   #pragma omp parallel for private(i) lastprivate(a)
   for (i=0; i<n; i++)
   {
       a = i+1;
       printf("Thread %d has a value of a = %d for i = %d\n",
              omp_get_thread_num(),a,i);
   } /*-- End of parallel for --*/

   printf("Value of a after parallel for: a = %d\n",a);

   return(0);
}
