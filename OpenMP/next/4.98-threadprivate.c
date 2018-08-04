#include <stdio.h>
#include <stdlib.h>

int calculate_sum(int length);

int *pglobal;

#pragma omp threadprivate(pglobal)

int main()
{
   int i, j, sum, TID, n = 5;
   int length[n], check[n];

   for (i=0; i<n; i++)
   {
      length[i] = 10 * (i+1);			//i.e. 10 20 30 40 50
      check[i]  = length[i]*(length[i]+1)/2; 	//using sum formula[n*(n+1)/2]
   }

   #pragma omp parallel for shared(n,length,check) private(TID,i,j,sum)
   for (i=0; i<n; i++)
   {
      TID = omp_get_thread_num();

      if ( (pglobal = (int *) malloc(length[i]*sizeof(int))) != NULL ) {

         for (j=sum=0; j<length[i]; j++)
             pglobal[j] = j+1;
         
         sum = calculate_sum(length[i]);

         printf("TID %d: value of sum for i = %d is %8d (check = %8d)\n",TID,i,sum,check[i]);

         free(pglobal);

      } else {
         printf("TID %d: fatal error in malloc for length[%d] = %d\n", TID,i,length[i]);
      }
   } /*-- End of parallel for --*/

   return(0);
}
int calculate_sum(int length)
{
   int sum = 0, j;

   for (j=0; j<length; j++)
      sum += pglobal[j];

   return(sum);
}
