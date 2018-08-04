#include <stdio.h>
#include <stdlib.h>

int main()
{
   int i, j, n = 9;


#pragma omp parallel for default(none) schedule(runtime) \
        private(i,j) shared(n)
   for (i=0; i<n; i++)
   {
      printf("Iteration %d executed by thread %d\n",
          i, omp_get_thread_num());
      for (j=0; j<i; j++)
          system("sleep 1");
   } /*-- End of parallel for --*/

   return(0);
}
