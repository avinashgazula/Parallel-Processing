
/*
  Example 2.5	       : reduce.c

  Objective            : To find sum of 'n' integers on 'p' processors
                         using MPI collective communication and communciation
                         library call ( MPI_REDUCE ).

  Input               : Automatic generation of input
                        The rank of each proceess is input on each process.

  Output              : Process with Rank 0 should print the sum of 'n' values.

  Necessary Condition : Number of Processes should be
                        less than or equal to 8.

*/



#include <stdio.h>
#include "mpi.h"

int main(int argc,char *argv[])
{
    int	   sum = 0;
    int    MyRank, Numprocs, Root = 0;
    MPI_Status status;

    /*....MPI Initialisation....*/

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&Numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&MyRank);

    /*....The REDUCE function of MPI....*/

    MPI_Reduce(&MyRank, &sum, 1, MPI_INT, MPI_SUM, Root, MPI_COMM_WORLD);

    if(MyRank == Root) 
		 printf("SUM = %d\n", sum);

    MPI_Finalize();
}

