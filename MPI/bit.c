#include<stdio.h>
#include<mpi.h>
void main(int argc,char *argv[])
{
	int array[16]={10,20,5,9,3,8,12,14,90,0,60,40,23,35,95,18};

	MPI_Init(&argc,&argv);

	int a,recv;

	MPI_Comm comm_hyp;
	MPI_Status status;

	int rank,left_source,rank_source,rank_dest,i=0,temp,rank4d,j=0;
	//Created cart for 4 dimension
	int ndims=4,dims[4]={2,2,2,2},period[4]={1,1,1,1},coords[4];

	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	MPI_Scatter(array,1,MPI_INT,&a,1,MPI_INT,0,MPI_COMM_WORLD);
	
	/*
	
	comm_old
    		input communicator (handle) 
	ndims
    		number of dimensions of cartesian grid (integer) 
	dims
    		integer array of size ndims specifying the number of processes in each dimension periods
    		logical array of size ndims specifying whether the grid is periodic (true) or not (false) in each dimension reorder
    		ranking may be reordered (true) or not (false) (logical) 
	*/
	MPI_Cart_create(MPI_COMM_WORLD,ndims,dims,period,0,&comm_hyp);

	MPI_Comm_rank(comm_hyp,&rank4d);
	//Determines process coords in cartesian topology given rank in group 
	MPI_Cart_coords(comm_hyp,rank,ndims,coords);

	printf("rank n coords in 4d %d,%d,%d,%d,%d\n",rank4d,coords[0],coords[1],coords[2],coords[3]);

	for(i=0;i<ndims;i++)
	{
		for(j=i;j>=0;j--)
		{
			//Returns the shifted source and destination ranks, given a shift direction and amount 
			MPI_Cart_shift(comm_hyp,3-j,1,&rank_source,&rank_dest);		
			
			printf("my src and dest is %d,%d\n",rank_source,rank_dest);
			MPI_Sendrecv(&a,1,MPI_INT,rank_dest,j,&recv,1,MPI_INT,rank_source,j,comm_hyp,&status);
			if(coords[3-i-1]!=coords[3-j])
			{
				if(a>recv)
					a=recv;
			
			}
			else
			{
				if(a<recv)
				a=recv;
			}
			MPI_Barrier(comm_hyp);	
		}
		MPI_Barrier(comm_hyp);	
	}
	MPI_Barrier(comm_hyp);
	printf("process rank %d is having element= %d  %d\n",rank,a, array[rank]);
	MPI_Finalize();
}
 


