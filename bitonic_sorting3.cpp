//sequence to be sorted:-24 -330 -83 442 637 -867 655 475 -76 537 -92 964 62 -885 312 -19 
//rank of processor 0 =-885
//rank of processor 11 =475
//rank of processor 14 =655
//rank of processor 1 =-867
//rank of processor 13 =637
//rank of processor 7 =-19
//rank of processor 12 =537
//rank of processor 3 =-92
//rank of processor 6 =-24
//rank of processor 8 =62
//rank of processor 15 =964
//rank of processor 9 =312
//rank of processor 10 =442
//rank of processor 2 =-330
//rank of processor 4 =-83
//rank of processor 5 =-76

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
        int up=1000;
        int low=-1000;
        tab[i]=rand() % (up - low + 1) + low;
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
