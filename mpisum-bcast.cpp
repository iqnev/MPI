///////////////////////////////////////////////////////////////////////////////////////
//                                                                                   //
//     MPI Matrix Sum 															     //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////

#include <mpi.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{

const int PNUM = 1; //number of processes
const int MSIZE = 2; //matrix size

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
int B[MSIZE][MSIZE];
int C[MSIZE][MSIZE];

for(int i=0;i<MSIZE;i++)
{
    for(int j=0;j<MSIZE;j++)
    {
        C[i][j]=0;
    }
}


if(rank==0){

	std::cout<<("\n<<MPI Matrix Sum>>\n\n");
	std::cout<<"Please chose one:";
    std::cout<<"\n1. Initialize A with 1 and B with 2";
    std::cout<<"\n2. Initialize A and B with random numbers between 0..100\n";
    std::cout<<"\nYour choise: ";
	std::cin>>value;

	if(value==1) {
	
        for(int i=0;i<MSIZE;i++)
		{
            for(int j=0;j<MSIZE;j++)
			{
				A[i][j]=1;
				B[i][j]=2;
			}
		}

	}
	else {

    for(int i=0;i<MSIZE;i++)
		{
            for(int j=0;j<MSIZE;j++)
			{
                A[i][j]=rand()%100;
                B[i][j]=rand()%100;
			}
		}
	}
	
   // for(int i=1;i<PNUM;i++)
	//{
  //  MPI_Send(&A[i*MSIZE/PNUM][0],MSIZE*2,MPI_INT,i,0,MPI_COMM_WORLD);
  //  MPI_Send(&B[i*MSIZE/PNUM][0],MSIZE*2,MPI_INT,i,0,MPI_COMM_WORLD);
//	}

	MPI_Bcast(&A,MSIZE*MSIZE,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&B,MSIZE*MSIZE,MPI_INT,0,MPI_COMM_WORLD);

    for(int i=0;i<MSIZE/PNUM;i++)
	{
        for(int j=0;j<MSIZE;j++)
		{
		  C[i][j]=A[i][j]+B[i][j];
		
		}
	}

    for(int i=1;i<PNUM;i++)  MPI_Recv(&C[i*MSIZE/PNUM][0],MSIZE*2,MPI_INT,i,0,MPI_COMM_WORLD,&status);


        for(int i=0;i<MSIZE;i++)
		{
        for(int j=0;j<MSIZE;j++)
			{
				std::cout<<C[i][j]<<" ";
			}
		std::cout<<std::endl;
		}

		time2 = MPI_Wtime();

std::cout<<"\nElasped Time: "<<time2-time1;


}
else{    
    MPI_Bcast(&A,MSIZE*MSIZE,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&B,MSIZE*MSIZE,MPI_INT,0,MPI_COMM_WORLD);

    for(int i=rank*MSIZE/PNUM;i<(rank*MSIZE/PNUM)+2;i++)
	{
        for(int j=0;j<MSIZE;j++)
		{
           C[i][j]=A[i][j]+B[i][j];
        }
	}

    MPI_Send(&C[rank*MSIZE/PNUM][0],MSIZE*2,MPI_INT,0,0,MPI_COMM_WORLD);

	

}
MPI_Finalize();

return 0;
}
