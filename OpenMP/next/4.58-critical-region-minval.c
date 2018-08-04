#include <stdio.h>
#include <stdlib.h>

#define N 10

int main()
{
   int    ix;
   double Scale, LScale, ssq, Lssq;

   #pragma omp parallel 
   {
      #pragma omp single
         printf("Number of threads is %d\n",omp_get_num_threads());
   }

   Scale = 2.0;
   ssq   = 1.0;

   printf("Before parallel region: Scale = %f ssq = %f\n",Scale,ssq);
   
   #pragma omp parallel default(none) private(ix,LScale,Lssq) \
                        shared(Scale,ssq)
   {

      int TID = omp_get_thread_num();

      Lssq = 2.0 * TID;

      #pragma omp for
      for (ix = 1; ix < N; ix ++)
      {
         LScale = TID + ix;
      }
      printf("Thread %d computed LScale = %f\n",TID,LScale);


      #pragma omp critical
      {

         printf("Thread %d entered critical region\n",TID);

         if ( Scale < LScale )
         {
            ssq = (Scale/LScale) * ssq + Lssq;
            Scale = LScale;
            printf("\tThread %d: Reset Scale to %f\n",TID,Scale);
         } else {
            ssq = ssq + (LScale/Scale) * Lssq;
         } 
         printf("\tThread %d: New value of ssq = %f\n",TID,ssq);

      } /*-- End of critical region --*/

   } /*-- End of parallel region --*/

   printf("After parallel region: Scale = %f ssq = %f\n",Scale,ssq);

   return(0);

}
