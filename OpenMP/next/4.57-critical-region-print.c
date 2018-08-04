#include <stdio.h>
#include <stdlib.h>

int main()
{
   int TID;


   #pragma omp parallel
   {
      #pragma omp single
         printf("Number of threads is %d\n",omp_get_num_threads());
   }

   #pragma omp parallel default(none) private(TID)
   {
      TID = omp_get_thread_num();

      #pragma omp critical (print_tid)
      {
        printf("I am thread %d\n",TID);
      }
   } /*-- End of parallel region --*/

   return(0);

}
