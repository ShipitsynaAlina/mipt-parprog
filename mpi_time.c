//
//  mpi_time.c
//  Time
//
//  Created by Alina Shipitsyna on 07.04.2020.
//  Copyright © 2020 Alina Shipitsyna. All rights reserved.
//

#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

static int message[1]; // max length of message

int main(int argc, const char * argv[]) {
    double time_start, time_finish;
    int i, n, nprocs, myid, len;

    MPI_Status status;
    
    MPI_Init(&argc, &argv); /* initialization MPI */
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    
    printf("PROGMPI: nprocs=%d myid=%d\n", nprocs, myid);
    fflush(stdout);
    
    len = atoi(argv[1]); // get length message from command line
    
    if (myid == 0) /* I a m MASTER-process ! */
    {
        time_start = MPI_Wtime();
        
        MPI_Send(message, len, MPI_CHAR, nprocs-1, 99, MPI_COMM_WORLD);
        MPI_Recv(message, len, MPI_CHAR, nprocs-1, 99, MPI_COMM_WORLD, &status);
        
        time_finish = MPI_Wtime();
        
        printf("Time %f \n", (float)(time_finish - time_start));
        
    } /* end if (myid==0) */
    
    else
    if (myid == (nprocs-1)) /* I am is last of all processes ! */
    {
        MPI_Recv(message, len, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &status);
        MPI_Send(message, len, MPI_CHAR, 0, 99, MPI_COMM_WORLD);
            
    }     /* end if (myid==(nproc-1)) */
    
    fflush(stdout);
    MPI_Finalize(); /* deinitialize MPI */

    return 0;
}
