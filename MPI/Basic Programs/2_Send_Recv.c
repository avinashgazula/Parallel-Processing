// MPI_Send, MPI_Recv example. Communicates the number -1 from process 0
// to process 1.

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);
    // Find out rank, size
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    // We are assuming at least 2 processes for this task
    if (world_size < 2) {
        fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    int number;
    if (world_rank == 0) {
        // If we are rank 0, set the number to 10 and send it to process 1
        number = 10;
        //int temp[]={5,3,6};
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (world_rank == 1) {
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n", number);
    }
    MPI_Finalize();
}

/*
 * Try sending arrays of integer, float values and string
 * 
 * What if count is greater than array size in MPI_Send
 * 
 * What if send count is greater than received count
 * 
 * Datatype used 
 * 	MPI_SHORT
	MPI_INT
	MPI_LONG
	MPI_UNSIGNED_CHAR
	MPI_FLOAT	float
	MPI_DOUBLE	double
 * 
 * Can use predefined constant MPI_ANY_TAG, if wants to receive from any tag value
 * */
