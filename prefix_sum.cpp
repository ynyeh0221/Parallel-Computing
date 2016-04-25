//This program does the prefix sum of {1,2,3,...,N}, N needs to be power of 2.
//mpiexec -N ./prefixsum N
//where N is the number of processors
//For example, if N=16, the output is
//prefix sum of processor 15 =136
//prefix sum of processor 7 =36
//prefix sum of processor 13 =105
//prefix sum of processor 14 =120
//prefix sum of processor 5 =21
//prefix sum of processor 9 =55
//prefix sum of processor 11 =78
//prefix sum of processor 10 =66
//prefix sum of processor 2 =6
//prefix sum of processor 6 =28
//prefix sum of processor 8 =45
//prefix sum of processor 0 =1
//prefix sum of processor 1 =3
//prefix sum of processor 12 =91
//prefix sum of processor 4 =15
//prefix sum of processor 3 =10

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char * argv[]) {
    // initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    int N = atoi(argv[1]);
    int rank;
    MPI_Comm_rank(comm, &rank);
    int tab[N];
    int sum[N];
    for(int i=0;i<N;i++)
    {
        tab[i]=i+1;
        sum[i]=i+1;
    }
    int rounds=2;
    while(rounds<=N)
    {
        if(rounds==2)
        {
            if(rank%rounds==0)
            {
                int x=sum[rank];
                MPI_Status stat;
                MPI_Send(&x, 1, MPI_INT, rank+rounds/2, 1, MPI_COMM_WORLD);
                int a;
                MPI_Recv(&a, 1, MPI_INT, rank+rounds/2, 1, MPI_COMM_WORLD, &stat);
                sum[rank]=a;
            }
            else
            {
                int a;
                MPI_Status stat;
                MPI_Recv(&a, 1, MPI_INT, rank-rounds/2, 1, MPI_COMM_WORLD, &stat);
                tab[rank]+=a;
                sum[rank]+=a;
                int x=sum[rank];
                MPI_Send(&x, 1, MPI_INT, rank-rounds/2, 1, MPI_COMM_WORLD);
            }
        }
        else
        {
            if((int)(rank/(rounds/2))%2==0)
            {
                int x=sum[rank];
                MPI_Status stat;
                MPI_Send(&x, 1, MPI_INT, rank+rounds/2, 1, MPI_COMM_WORLD);
                int a;
                MPI_Recv(&a, 1, MPI_INT, rank+rounds/2, 1, MPI_COMM_WORLD, &stat);
                sum[rank]=a;
            }
            else
            {
                int a;
                MPI_Status stat;
                MPI_Recv(&a, 1, MPI_INT, rank-rounds/2, 1, MPI_COMM_WORLD, &stat);
                tab[rank]+=a;
                sum[rank]+=a;
                int x=sum[rank];
                MPI_Send(&x, 1, MPI_INT, rank-rounds/2, 1, MPI_COMM_WORLD);
            }
        }
        rounds*=2;
    }
    printf("prefix sum of processor %d =%d\n", rank, tab[rank]);
    // finished with MPI
    MPI_Finalize();
    return 0;
}
