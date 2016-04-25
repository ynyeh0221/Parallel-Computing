//mpicxx filename.cpp -o name
//mpiexec -np N ./name N
//Evaluate P(x) at a given value of x, say x0.
//P(x)=a_0x+a_1x^2+...+a_{n-2}x^{n-1}
//Enter x0:
//2
//a_{7}x_0^{8} =1792
//a_{5}x_0^{6} =320
//a_{6}x_0^{7} =768
//a_{3}x_0^{4} =48
//a_{1}x_0^{2} =4
//a_{2}x_0^{3} =16
//a_{4}x_0^{5} =128
//a_{0}x_0^{1} =0


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
    int coeff[N];
    for(int i=1;i<=N;i++)
        coeff[i]=i;
    int rounds=2;
    if(rank==0)
    {
        int x0=0;
        printf("Enter x0:\n");
        scanf("%d", &x0);
        MPI_Bcast(&x0, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
        tab[rank]=x0;
        sum[rank]=x0;
    }
    else
    {
        int x0=0;
        MPI_Bcast(&x0, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
        tab[rank]=x0;
        sum[rank]=x0;
    }
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
                tab[rank]*=a;
                sum[rank]*=a;
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
                tab[rank]*=a;
                sum[rank]*=a;
                int x=sum[rank];
                MPI_Send(&x, 1, MPI_INT, rank-rounds/2, 1, MPI_COMM_WORLD);
            }
        }
        rounds*=2;
    }
    printf("a_{%d}x_0^{%d} =%d\n", rank, rank+1, coeff[rank]*tab[rank]);
    // finished with MPI
    MPI_Finalize();
    return 0;
}
