//
//  exp_mpi.c
//
//  Created by Alina Shipitsyna on 09.04.2020.
//  Copyright © 2020 Alina Shipitsyna. All rights reserved.
//

#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

long double Fact(long int n){
    if (n == 0)
        return 1;
    else
        return n*Fact(n-1);
}

long int N = 100000;

int main(int argc, const char * argv[]) {
    int myid, nprocs;
    long int n;
    double start_time, finish_time;
    double frac_sum=0;
    double result=0;
    int i;
    
    MPI_Status status;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    
    printf("PROGMPI: nprocs=%d myid=%d\n", nprocs, myid);
    fflush(stdout);
    
    if(myid == 0)
        start_time = MPI_Wtime();
    
    
    for (i = myid; i <= N; i += nprocs){
        if(i == 0)
            frac_sum += 1;
        else
            frac_sum += 1 / Fact(i);
    }
    
    if (myid == 0){
        result = frac_sum;
        for (i = 1; i < nprocs; i++){
            MPI_Recv(&frac_sum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            result = result + frac_sum;
        }
    }
    else{
        MPI_Send(&frac_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    
    
    if (myid == 0){
        printf("Exp = %f \n", (float)result);
        
        finish_time = MPI_Wtime();
        printf("Time: %f \n", (float)(finish_time - start_time));
    }
    
    fflush(stdout);
    MPI_Finalize();
    
    return 0;
}

