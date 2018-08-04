#include <stdio.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0

int main ()
{
   int a, b, c, i, n;
   int a_check, c_check;

   b = 50;
   n = 1858;

   a_check = b + n-1;
   c_check = a_check + b;

   printf("Before parallel loop: b = %d n = %d\n",b,n);

   #pragma omp parallel for private(i), firstprivate(b), \
                            lastprivate(a)
   for (i=0; i<n; i++)
   {
       a = b+i;
   } /*-- End of parallel for --*/

   c = a + b;

   printf("Values of a and c after parallel for:\n");
   printf("\ta = %d\t(correct value is %d)\n",a,a_check);
   printf("\tc = %d\t(correct value is %d)\n",c,c_check);

   return(0);
}
