//This program uses N processors to do summation of 1, 2, 3, ..., N in parallel.
//mpicxx filename.cpp -o name
//mpiexec -np N ./name N
//where N is the number of processors (N needs to be power of 2)

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char * argv[]) {
    // initialize MPI
    MPI_Init(&argc, &argv);
	
    // get communicator size and ranks
    MPI_Comm comm = MPI_COMM_WORLD;
    int N = atoi(argv[1]);
    int rank;
    MPI_Comm_rank(comm, &rank);
    int tab[N];
    for(int i=0;i<N;i++)
    {
        tab[i]=i+1;
    }
    while(N>0)
    {
        if(rank>=N/2 && rank<N)
        {
            int x=tab[rank];
            MPI_Send(&x, 1, MPI_INT, rank-N/2, 1, MPI_COMM_WORLD);
        }
        else if(rank>=0 && rank<N/2)
        {
            int a;
            MPI_Status stat;
            MPI_Request req;
            MPI_Irecv(&a, 1, MPI_INT, rank+N/2, 1, MPI_COMM_WORLD, &req);
            MPI_Wait(&req, &stat);
            tab[rank]=a+tab[rank];
        }
        N=N/2;
    }
    if(N==0 && rank==0) printf("Total sum=%d\n",tab[0]);

    // finished with MPI
    MPI_Finalize();
    return 0;
}
