
/*
  Example 2.1		: broadcast.c

    Objective           : To braodcast an integer array of size "n" by 
                          "with Rank 0"  using MPI Collective communication 
                           library call 

                          (MPI_Bcast)

    Input               : Input Data file "sdata.inp" by proces with Rank 0

    Output              : Print the scattered array on all processes.

    Necessary Condition : Number of processes should be 
                          less than or equal to 8.

*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"
#define BUFFER_SIZE 14
int main(int argc,char **argv)
{
/*................VARIABLE INITIALIZATION...............*/
   int   MyRank,Numprocs;
   int   Root=0;
   char message[BUFFER_SIZE];  


/*................MPI INITIALIZATION...............*/
   
   MPI_Init(&argc,&argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&MyRank);
   MPI_Comm_size(MPI_COMM_WORLD,&Numprocs);

/*..............GETTING MESSAGE...............*/

   if(MyRank==0)
   strcpy(message,"HELLO_WORLD");

 /*................CALL MPI BROADCAST..................*/

   MPI_Bcast(message,BUFFER_SIZE,MPI_CHAR,Root,MPI_COMM_WORLD);

/*...................PRINT RECIEVED MESSAGE..........*/

   printf("\n Rank %d gets %s from Process %d *** \n",MyRank,message,Root);  
         

 MPI_Finalize();
}

