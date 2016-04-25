//mpiexec -N ./prefixsum N
//where N is the number of processors
//rank of processor 15 =16
//rank of processor 11 =12
//rank of processor 3 =4
//rank of processor 7 =8
//rank of processor 14 =15
//rank of processor 12 =13
//rank of processor 6 =7
//rank of processor 8 =9
//rank of processor 4 =5
//rank of processor 13 =14
//rank of processor 9 =10
//rank of processor 10 =11
//rank of processor 1 =2
//rank of processor 5 =6
//rank of processor 2 =3
//rank of processor 0 =1

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
        tab[i]=1;
        sum[i]=1;
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
    printf("rank of processor %d =%d\n", rank, tab[rank]);
    // finished with MPI
    MPI_Finalize();
    return 0;
}
