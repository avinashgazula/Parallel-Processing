// Example using MPI_Comm_split to divide a communicator into subcommunicators
//

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(NULL, NULL);
    
    // Get the rank and size in the original communicator
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    int color = world_rank / 4; // Determine color based on row
    
    // Split the communicator based on the color and use the original rank for ordering
    MPI_Comm row_comm;
	//comm,color,key, newcomm
    MPI_Comm_split(MPI_COMM_WORLD, color, 10, &row_comm);
    
    int row_rank, row_size;
    MPI_Comm_rank(row_comm, &row_rank);
    MPI_Comm_size(row_comm, &row_size);
    
    printf("WORLD RANK/SIZE: %d/%d --- ROW RANK/SIZE: %d/%d\n",
    world_rank, world_size, row_rank, row_size);
    
    MPI_Comm_free(&row_comm);
    
    MPI_Finalize();
}

/*
	color : determines to which new communicator each processes will belong
	color = MPI_UNDEFINED, that process won't be included in any of the new communicators
	key : determines the ordering (rank) within each new communicator
*/
