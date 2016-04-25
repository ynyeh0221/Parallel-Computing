//sequence to be sorted:147 554 677 945 888 402 608 852 627 955 40 269 273 765 42 88 
//rank of processor 3 =147
//rank of processor 1 =42
//rank of processor 11 =765
//rank of processor 2 =88
//rank of processor 9 =627
//rank of processor 0 =40
//rank of processor 10 =677
//rank of processor 8 =608
//rank of processor 15 =955
//rank of processor 14 =945
//rank of processor 13 =888
//rank of processor 12 =852
//rank of processor 7 =554
//rank of processor 6 =402
//rank of processor 5 =273
//rank of processor 4 =269

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int max ( int a, int b ) { return a > b ? a : b; }
int min ( int a, int b ) { return a < b ? a : b; }

void int2bin(int num, char *str)
{
  *(str+5) = '\0';
  int mask = 0x10 << 1;
  while(mask >>= 1)
    *str++ = !!(mask & num) + '0';
}

int bin2int(char *s)
{
    int rc;
    for (rc = 0; '\0' != *s; s++)
    {
        if ('1' == *s) {
            rc = (rc * 2) + 1;
        } else if ('0' == *s) {
            rc *= 2;
        }
    }
    return rc;
}

int main(int argc, char * argv[]) {
    // initialize MPI
    srand(time(0));
    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    int N = atoi(argv[1]);
    int rank;
    MPI_Comm_rank(comm, &rank);
    int tab[N];
    for(int i=0;i<N;i++)
    {
        tab[i]=(rand()%1000)+1;
    }
    if(rank==0)
    {
        printf("sequence to be sorted:");
        for(int i=0;i<N;i++)
        {
            printf("%d ", tab[i]);
        }
        printf("\n");
    }
    
    int i=0;
    while(i<int(log(N) / log( 2 )))
    {
        int j=i;
        while(j>=0)
        {
            char str[N];
            int2bin(rank,str);
            //printf("ori %s\n", str);
            if(str[4-j]=='0') str[4-j]='1';
            else str[4-j]='0';
            int dest=bin2int(str);
            int x=tab[rank];
            MPI_Status stat;
            MPI_Send(&x, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            int a;
            MPI_Recv(&a, 1, MPI_INT, dest, 1, MPI_COMM_WORLD, &stat);
            if(str[4-i-1]=='0')
            {
                if(dest>rank)
                    tab[rank]=min(x,a);
                else
                    tab[rank]=max(x,a);
            }
            else
            {
                if(dest>rank)
                    tab[rank]=max(x,a);
                else
                    tab[rank]=min(x,a);
            }
            //printf("%s\n",str);
            //printf("%d %d %d rank%d %d\n",int(log(N) / log( 2 )),i,j, rank, tab[rank]);
            j--;
        }
        i++;
    }
    printf("rank of processor %d =%d\n", rank, tab[rank]);
    // finished with MPI
    MPI_Finalize();
    return 0;
}
