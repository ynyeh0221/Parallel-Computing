//difference between MPI_Scan, MPI_Exscan, and MPI_Reduce
//Scan: process 0:   reduce = 0
//Exscan: process 0:   reduce = 0
//Scan: process 1:   reduce = 1
//Scan: process 2:   reduce = 3
//Exscan: process 1:   reduce = 0
//Exscan: process 2:   reduce = 1
//Scan: process 3:   reduce = 6
//Exscan: process 3:   reduce = 3
//Scan: process 4:   reduce = 10
//Exscan: process 4:   reduce = 6
//Scan: process 5:   reduce = 15
//Exscan: process 5:   reduce = 10
//Scan: process 6:   reduce = 21
//Exscan: process 6:   reduce = 15
//Scan: process 7:   reduce = 28
//Exscan: process 7:   reduce = 21
//Reduce: process 0:   reduce = 28

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstdlib>
#include <utility>
#include <math.h>
#include <mpi.h>
#include <iostream>

int main(int argc, char *argv[])
{
int root = 0;
int processCount;
int currentRank;
MPI_Status status;

MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD,&processCount);  
MPI_Comm_rank(MPI_COMM_WORLD,&currentRank);
int reduce = currentRank;   
int reduce2 = currentRank; 
int reduce3 = 0; 

MPI_Scan(&currentRank,&reduce,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
printf("Scan: process %d:   reduce = %d\n", currentRank, reduce);
MPI_Exscan(&currentRank,&reduce2,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
printf("Exscan: process %d:   reduce = %d\n", currentRank, reduce2);
MPI_Reduce(&currentRank,&reduce3,1,MPI_INT,MPI_SUM, 0, MPI_COMM_WORLD);
if(currentRank==0)
    printf("Reduce: process %d:   reduce = %d\n", currentRank, reduce3);

MPI_Finalize();
return 0;
}
