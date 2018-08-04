#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int i, thread_id;
    int glob_nloops, priv_nloops;
    glob_nloops = 0;

//using reduction to calculate total number of loops
    #pragma omp parallel private(priv_nloops, thread_id) reduction(+:glob_nloops)
    {
        priv_nloops = 0;
        thread_id = omp_get_thread_num();

        #pragma omp for
        for (i=0; i<100000; ++i)
        {
            ++priv_nloops;
        }
        glob_nloops += priv_nloops;
    }
    printf("Total # loop iterations is %d\n",
           glob_nloops);
    return 0;
}