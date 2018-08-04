/*
A simple MPI example program using non-blocking send and receive.

The program consists of two processes. Process 0 sends a message
to the receiver. This receives the message and sends it back.
Both processes use non-blocking send and receive operations
(MPI_Isend and MPI_Irecv, and MPI_Wait to wait until the message
has arrived).

Compile the program with 'mpicc -O3 send-nonblocking-wait.c -o send-nonblocking-wait'
Run the program on two processes.
*/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc, char* argv[]) {
  int x, y, np, me;
  int tag = 42;
  MPI_Status status;
  MPI_Request send_req, recv_req;    /* Request object for send and receive */

  MPI_Init(&argc, &argv);              /* Initialize MPI */
  MPI_Comm_size(MPI_COMM_WORLD, &np);  /* Get number of processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &me);  /* Get own identifier */

  /* Check that we run on exactly two processes */
  if (np != 2) {
    if (me == 0) {
      printf("You have to use exactly 2 processes to run this program\n");
    }
    MPI_Finalize();	       /* Quit if there is only one process */
    exit(0);
  }
  
  x = 12345; y = me;   /* Initialize */
  if (me == 0) {    /* Process 0 does this */
    
    printf("Process %d sending to process 1\n", me);
    MPI_Isend(&x, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &send_req);
    /* We could do computations here while we are waiting for communication */
    MPI_Wait(&send_req, &status);

    printf("Process %d receiving from process 1\n", me);
    MPI_Irecv (&y, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &recv_req);
    /* We could do computations here while we are waiting for communication */
    printf ("Process %d has value %d\n", me, y);

    MPI_Wait(&recv_req, &status);
    printf ("Process %d received value %d\n", me, y);
    
  } else {         

    MPI_Irecv (&y, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &recv_req);
    /* We could do computations here while we are waiting for communication */
    MPI_Wait(&recv_req, &status);


    MPI_Isend (&y, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &send_req);
    /* We could do computations here while we are waiting for communication */
    MPI_Wait(&send_req, &status);

  }

  MPI_Finalize();
  exit(0);
}
