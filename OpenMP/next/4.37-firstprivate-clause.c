#include <stdio.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0

int main()
{
  int *a;
  int n = 2, nthreads, vlen, indx, offset = 4, i, TID;
  int failed;

  indx = offset;

   /*
   ------------------------------------------------------------------------
   Set up parameters for computation and allocate memory
   ------------------------------------------------------------------------
   */
   #pragma omp parallel firstprivate(indx) shared(a,n,nthreads,failed)
   {
     #pragma omp single
     {
        nthreads = omp_get_num_threads();
        vlen = indx + n*nthreads;
        if ( (a = (int *) malloc(vlen*sizeof(int))) == NULL )
           failed = TRUE;
        else
           failed = FALSE;
     }
   } /*-- End of parallel region --*/

   if ( failed == TRUE ) {
      printf("Fatal error: memory allocation for a failed vlen = %d\n",vlen);
      return(-1);
   }
   else
   {
      printf("Diagnostics:\n");
      printf("nthreads = %d\n",nthreads);
      printf("indx     = %d\n",indx);
      printf("n        = %d\n",n);
      printf("vlen     = %d\n",vlen);
   }

   for(i=0; i<vlen; i++)
    a[i] = -i-1;

   /*
   ------------------------------------------------------------------------
   Each thread starts access to array a through variable indx
   ------------------------------------------------------------------------
   */
   printf("Length of segment per thread is %d\n",n);
   printf("Offset for vector a is %d\n",indx);
   #pragma omp parallel default(none) firstprivate(indx) \
           private(i,TID) shared(n,a)
   {
      TID = omp_get_thread_num();
      indx += n*TID;
      for(i=indx; i<indx+n; i++)
         a[i] = TID + 1;
   } /*-- End of parallel region --*/

   printf("After the parallel region:\n");
   for (i=0; i<vlen; i++)
      printf("a[%d] = %d\n",i,a[i]);

   free(a);

   return(0);
}
