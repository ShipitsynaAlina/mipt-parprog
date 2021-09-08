//
//  matrix1.c
//  
//
//  Created by Alina Shipitsyna on 28.04.2020.
//

#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#define N 5000      //Кратно числу процессов

int main (int argc, char* argv[])
{
    int myid, nprocs;
    int i, j, k, p, ind;
    int n;
    
    int *A, *B, *C, *Bt;
    int *a, *b, *c;
    
    double start_time, finish_time;

    MPI_Status Status;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    
    n = N / nprocs; //количество строк матрицы А (столбцов В) в одном процессе
    
//иницилиализируем матрицы, в которых будут храниться результаты вычислений на каждом процессе
    a = (int *) malloc((n * N) * sizeof(int));
    b = (int *) malloc((n * N) * sizeof(int));
    c = (int *) malloc((n * N) * sizeof(int));
    
    B = (int *) malloc((N * N) * sizeof(int));
    Bt = (int *) malloc((N * N) * sizeof(int));
    
//инициализируем матрицы в нулевом процессе
    if (myid == 0)
    {
        
        A = (int *) malloc((N * N) * sizeof(int));
        C = (int *) malloc((N * N) * sizeof(int));
           
// матрицу удобнее рассматривать как одномерный массив но с индексами i*n+j
// заполняем матрицы рандомными числами от 0 до 4
        
//        printf("A: \n");
        for (i = 0; i < N; i++){
            for (j = 0; j < N ; j++){
                A[i*N+j] = rand() % 5;
//                printf("%d ", A[i*N+j]);
            }
//            printf("\n");
        }
        
//        printf("B: \n");
        for (i = 0; i < N; i++){
            for (j = 0; j < N; j++){
                B[i*N+j] = rand() % 5;
//                printf("%d ", B[i*N+j]);
            }
//            printf("\n");
        }
        fflush(stdout);
    }
    
    
    if(myid == 0)
        start_time = MPI_Wtime();
    
    
    MPI_Bcast(B, N*N, MPI_INT, 0, MPI_COMM_WORLD);
    
// рассылаем части матриц по процессам
    MPI_Scatter(A, N*n, MPI_INT, a, N*n, MPI_INT, 0, MPI_COMM_WORLD);

    int temp = 0;
    for (i = 0; i < n; i++){
        for (j = 0; j < N; j ++){
            for (k = 0; k < N; k++)
                temp += a[i*N + k] * B[k*N + j];
            c[i*N + j] = temp;
            temp = 0;
        }
    }
    
    MPI_Gather(c, N*n, MPI_INT, C, N*n, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (myid ==0){
        finish_time = MPI_Wtime();
        /*
        printf("C: \n");
        for (i = 0; i < N; i++){
            for (j = 0; j < N; j++)
                printf("%d ", C[i*N+j]);
            printf("\n");
        }
        */
        printf("Time: %f \n", (float)(finish_time - start_time));
    }
    
    
    fflush(stdout);
    MPI_Finalize();
    
    
    return 0;
}
