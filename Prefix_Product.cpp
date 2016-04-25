//mpicxx filename.cpp -o name
//mpiexec -np N ./name N
//prefix product of processor 15 =2004189184
//prefix product of processor 11 =479001600
//prefix product of processor 7 =40320
//prefix product of processor 3 =24
//prefix product of processor 13 =1278945280
//prefix product of processor 9 =3628800
//prefix product of processor 1 =2
//prefix product of processor 5 =720
//prefix product of processor 14 =2004310016
//prefix product of processor 6 =5040
//prefix product of processor 12 =1932053504
//prefix product of processor 8 =362880
//prefix product of processor 0 =1
//prefix product of processor 4 =120
//prefix product of processor 10 =39916800
//prefix product of processor 2 =6

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
                //printf("%d: %d sends %d to %d\n", rounds, rank, x, rank+rounds/2);
                MPI_Recv(&a, 1, MPI_INT, rank+rounds/2, 1, MPI_COMM_WORLD, &stat);
                //printf("%d: %d recives %d from %d\n", rounds, rank, a, workerid);
                sum[rank]=a;
            }
            else
            {
                int a;
                MPI_Status stat;
                MPI_Recv(&a, 1, MPI_INT, rank-rounds/2, 1, MPI_COMM_WORLD, &stat);
                //printf("%d: %d recives %d from %d\n", rounds, rank, a, workerid);
                tab[rank]=tab[rank]*a;
                sum[rank]=sum[rank]*a;
                int x=sum[rank];
                MPI_Send(&x, 1, MPI_INT, rank-rounds/2, 1, MPI_COMM_WORLD);
                //printf("%d: %d sends %d to %d\n", rounds, rank, x, workerid);
            }
        }
        else
        {
            if((int)(rank/(rounds/2))%2==0)
            {
                int x=sum[rank];
                MPI_Status stat;
                MPI_Send(&x, 1, MPI_INT, rank+rounds/2, 1, MPI_COMM_WORLD);
                //printf("%d: %d sends %d to %d\n", rounds, rank, x, rank+rounds/2);
                int a;
                MPI_Recv(&a, 1, MPI_INT, rank+rounds/2, 1, MPI_COMM_WORLD, &stat);
                //printf("%d: %d recives %d from %d\n", rounds, rank, a, workerid);
                sum[rank]=a;
            }
            else
            {
                int a;
                MPI_Status stat;
                MPI_Recv(&a, 1, MPI_INT, rank-rounds/2, 1, MPI_COMM_WORLD, &stat);
                //printf("%d: %d recives %d from %d\n", rounds, rank, a, workerid);
                tab[rank]=tab[rank]*a;
                sum[rank]=sum[rank]*a;
                int x=sum[rank];
                MPI_Send(&x, 1, MPI_INT, rank-rounds/2, 1, MPI_COMM_WORLD);
                //printf("%d: %d sends %d to %d\n", rounds, rank, x, workerid);
            }
        }
        rounds*=2;
    }
    printf("prefix product of processor %d =%d\n", rank, tab[rank]);
    // finished with MPI
    MPI_Finalize();
    return 0;
}
