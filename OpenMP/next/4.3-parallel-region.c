#include <stdio.h>
#include <stdlib.h>

int main()
{

#pragma omp parallel 
   {
     printf("The parallel region is executed by thread %d\n",
        omp_get_thread_num());

     if ( omp_get_thread_num() == 2 ) {
        printf("  Thread %d does things differently\n",
               omp_get_thread_num());
     }
   }  /*-- End of parallel region --*/

   return(0);
}
