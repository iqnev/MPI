///////////////////////////////////////////////////////////////////////////////////////
//                                                                                   //
//     MPI Matrix Multiply     							     //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////

#include <mpi.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{

const int PNUM = 2; //number of processes
const int MSIZE = 4; //matrix size

int rank,value,size;
int namelen;

double time1,time2;

srand(time(NULL));

MPI_Init(&argc, &argv);

time1 = MPI_Wtime();



char processor_name[MPI_MAX_PROCESSOR_NAME];


MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Get_processor_name(processor_name,&namelen);
MPI_Status status;

int A[MSIZE][MSIZE];
int B[MSIZE];
int C[MSIZE];

if(rank==0){

		int a=0;
        for(int i=0;i<MSIZE;i++)
		{
            for(int j=0;j<MSIZE;j++)
			{
				A[i][j]=++a;
				B[i]=2;
				C[i]=0;
			}
		}
}


	MPI_Bcast(&A,MSIZE*MSIZE,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&B,MSIZE,MPI_INT,0,MPI_COMM_WORLD);
	
	int F=0;
    for(int i=rank*MSIZE/PNUM;i<(rank*MSIZE/PNUM)+2;i++)
	{
		 for(int j=0;j<MSIZE;j++)
		{
			 F+=A[i][j]*B[i];
			
        }
		C[i]=F;
		F=0;
	}

	if(rank==1){
	time2 = MPI_Wtime();
	std::cout<<"\nElasped Time: "<<time2-time1;
	MPI_Send(&C[rank*MSIZE/PNUM],MSIZE/2,MPI_INT,0,0,MPI_COMM_WORLD);

	}
	if(rank==0)
	{
    for(int i=1;i<PNUM;i++)  MPI_Recv(&C[i*MSIZE/PNUM],MSIZE/2,MPI_INT,i,0,MPI_COMM_WORLD,&status);
	

        for(int i=0;i<MSIZE;i++)
		{
        
		std::cout<<C[i]<<" ";
		std::cout<<std::endl;
		}


	}

MPI_Finalize();

return 0;
}
