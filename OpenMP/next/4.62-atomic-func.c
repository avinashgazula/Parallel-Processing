#include <stdio.h>
#include <stdlib.h>

int bigfunc(int TID);

int main()
{
   int ic, i, n = 7;

   ic = 0;
   #pragma omp parallel for shared(ic,n) private(i)
   for (i=0; i<n; i++)
   {

      int TID = omp_get_thread_num();
      #pragma omp atomic
         ic += bigfunc(TID);
   }
   printf("Counter = %d\n",ic);

   return(0);

}

int bigfunc(int TID)
{
   printf("This function is called from thread %d and needs to be thread safe\n",TID);
   return(1);
}
