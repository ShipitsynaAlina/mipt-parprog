//
//  matrix_omp.c
//  
//
//  Created by Alina Shipitsyna on 21.09.2020.
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>



//matrix addition
void add(int *c, int *a, int *b, int n){
    for(int i = 0; i < n * n; i++)
        c[i] = a[i] + b[i];
}
 
//matrix subtraction
void sub(int *c, int *a, int *b, int n){
    for(int i = 0; i < n * n; i++)
        c[i] = a[i] - b[i];
}

//normal matrix multiplication
void mul_normal(int *c, int *a, int *b, int n)
{
    int i, j, k;
    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
        {
            c[i*n + j] = 0;
            for(k = 0; k < n; k++)
                c[i*n + j] += a[i*n + k] * b[k*n + j];
        }
}

//copy part of matrix B to matrix A
void copy(int *a, int *b, int ib, int jb, int n)
{
    int i, j, k;
    int imax = ib + n/2;      //до какого индекса строки копируем
    int jmax = jb + n/2;      //аналогично по столбцам
 
    for(k = 0,i = ib; i < imax; i++)
    {
        for(j = jb; j < jmax; j++)
        {
            a[k++] = b[i*n + j];  //в матрице a элементы идут подряд
        }
    }
}

//reverse copy
void copy_back(int *a, int ia, int ja, int *b, int n)
{
    int i, j, k;
    int imax = ia + n/2;
    int jmax = ja + n/2;
 
    for(k = 0,i = ia; i < imax; i++)
    {
        for(j = ja; j < jmax; j++)
        {
            a[i*n + j] = b[k++];
        }
    }
}


//Алгоритм Штрассена
/*
 C = A * B = A00 A01 * B00 B01 = (S1+S4-S5+S7)     (S3+S5)
             A10 A11   B10 B11      (S2+S4)     (S1-S2+S3+S6)
 
 S1 = (A00 + A11)(B00 + B11) = T1 * T2
 S2 = (A10 + A11)B00 = T3 * B00
 S3 = A00(B01 - B11) = A00 * T4
 S4 = A11((B10 - B00) = A11 * T5
 S5 = (A00 + A01)B11 = T6 * B11
 S6 = (A10 - A00)(B00 + B01) = T7 * T8
 S7 = (A01 - A11)(B10 + B11) = T9 * T10
 
 */


void Strassen(int* C, int* A, int* B, int N){
    
    int h = N/ 2;  //size new matrix
    
    if (N <= 32){      //можно изменить размер на более оптимальный
        mul_normal(C, A, B, N);
    }
    else{
        int* H = (int*)malloc(34 * h * h * sizeof(int));  //выделяем один буфер под все вспомогательные матрицы, чтобы работало быстрее
        
        
        copy(&H[0], A, 0, 0, N);                    //H[0] = A00
        copy(&H[h*h], A, 0, h, N);                  //H[1] = A01
        copy(&H[2*h*h], A, h, 0, N);                //H[2] = A10
        copy(&H[3*h*h], A, h, h, N);                //H[3] = A11
        
        copy(&H[4*h*h], B, 0, 0, N);                //H[4] = B00
        copy(&H[5*h*h], B, 0, h, N);                //H[5] = B01
        copy(&H[6*h*h], B, h, 0, N);                //H[6] = B10
        copy(&H[7*h*h], B, h, h, N);                //H[7] = B11
        
        add(&H[8*h*h], &H[0], &H[3*h*h], h);          //H[8] = T1
        add(&H[9*h*h], &H[4*h*h], &H[7*h*h], h);      //H[9] = T2
        add(&H[10*h*h], &H[2*h*h], &H[3*h*h], h);     //H[10] = T3
        sub(&H[11*h*h], &H[5*h*h], &H[7*h*h], h);     //H[11] = T4
        sub(&H[12*h*h], &H[6*h*h], &H[4*h*h], h);     //H[12] = T5
        add(&H[13*h*h], &H[0], &H[h*h], h);           //H[13] = T6
        sub(&H[14*h*h], &H[2*h*h], &H[0], h);         //H[14] = T7
        add(&H[15*h*h], &H[4*h*h], &H[5*h*h], h);     //H[15] = T8
        sub(&H[16*h*h], &H[h*h], &H[3*h*h], h);       //H[16] = T9
        add(&H[17*h*h], &H[6*h*h], &H[7*h*h], h);     //H[17] = T10
        
        Strassen(&H[18*h*h], &H[8*h*h], &H[9*h*h], h);      //H[18] = S1
        Strassen(&H[19*h*h], &H[10*h*h], &H[4*h*h], h);     //H[19] = S2
        Strassen(&H[20*h*h], &H[0], &H[11*h*h], h);         //H[20] = S3
        Strassen(&H[21*h*h], &H[3*h*h], &H[12*h*h], h);     //H[21] = S4
        Strassen(&H[22*h*h], &H[13*h*h], &H[7*h*h], h);     //H[22] = S5
        Strassen(&H[23*h*h], &H[14*h*h], &H[15*h*h], h);    //H[23] = S6
        Strassen(&H[24*h*h], &H[16*h*h], &H[17*h*h], h);    //H[24] = S7
        
        add(&H[25*h*h], &H[18*h*h], &H[21*h*h], h);       //H[25] = T11
        add(&H[26*h*h], &H[19*h*h], &H[21*h*h], h);       //H[26] = T12
        add(&H[27*h*h], &H[20*h*h], &H[23*h*h], h);       //H[27] = T13
        sub(&H[28*h*h], &H[24*h*h], &H[22*h*h], h);       //H[28] = T14
        add(&H[29*h*h], &H[20*h*h], &H[22*h*h], h);       //H[29] = T16
        sub(&H[30*h*h], &H[18*h*h], &H[19*h*h], h);       //H[30] = T17
        
        add(&H[31*h*h], &H[25*h*h], &H[28*h*h], h);       //H[31] = T15
        add(&H[32*h*h], &H[27*h*h], &H[30*h*h], h);       //H[32] = T18
        
        copy_back(C, 0, 0, &H[31*h*h], N);  //C00
        copy_back(C, 0, h, &H[29*h*h], N);  //C01
        copy_back(C, h, 0, &H[26*h*h], N);  //C10
        copy_back(C, h, h, &H[32*h*h], N);  //C11
 
        free(H);
    }
    
}


int StrassenParallel(int* C, int* A, int* B, int N){
    
    int h = N / 2;  //size new matrix
    
    if (N <= 32){      //можно изменить размер на более оптимальный
        mul_normal(C, A, B, N);
    }
    else{
        int* H = (int*)malloc(34 * h * h * sizeof(int));  //выделяем один буфер под все вспомогательные матрицы, чтобы работало быстрее
        if (H == NULL) return 1;
        
        copy(&H[0], A, 0, 0, N);                    //H[0] = A00
        copy(&H[h*h], A, 0, h, N);                  //H[1] = A01
        copy(&H[2*h*h], A, h, 0, N);                //H[2] = A10
        copy(&H[3*h*h], A, h, h, N);                //H[3] = A11
        
        copy(&H[4*h*h], B, 0, 0, N);                //H[4] = B00
        copy(&H[5*h*h], B, 0, h, N);                //H[5] = B01
        copy(&H[6*h*h], B, h, 0, N);                //H[6] = B10
        copy(&H[7*h*h], B, h, h, N);                //H[7] = B11
        
        omp_set_num_threads(7);
        #pragma omp parallel shared(H)
        {
            #pragma omp single
            {
                #pragma omp task
                {
                    add(&H[8*h*h], &H[0], &H[3*h*h], h);               //H[8] = T1
                    add(&H[9*h*h], &H[4*h*h], &H[7*h*h], h);           //H[9] = T2
                    Strassen(&H[18*h*h], &H[8*h*h], &H[9*h*h], h);     //H[18] = S1
                }
                #pragma omp task
                {
                    add(&H[10*h*h], &H[2*h*h], &H[3*h*h], h);          //H[10] = T3
                    Strassen(&H[19*h*h], &H[10*h*h], &H[4*h*h], h);    //H[19] = S2
                }
                #pragma omp task
                {
                    sub(&H[11*h*h], &H[5*h*h], &H[7*h*h], h);          //H[11] = T4
                    Strassen(&H[20*h*h], &H[0], &H[11*h*h], h);        //H[20] = S3
                }
                #pragma omp task
                {
                    sub(&H[12*h*h], &H[6*h*h], &H[4*h*h], h);          //H[12] = T5
                    Strassen(&H[21*h*h], &H[3*h*h], &H[12*h*h], h);    //H[21] = S4
                }
                #pragma omp task
                {
                    add(&H[13*h*h], &H[0], &H[h*h], h);                //H[13] = T6
                    Strassen(&H[22*h*h], &H[13*h*h], &H[7*h*h], h);    //H[22] = S5
                }
                #pragma omp task
                {
                    sub(&H[14*h*h], &H[2*h*h], &H[0], h);              //H[14] = T7
                    add(&H[15*h*h], &H[4*h*h], &H[5*h*h], h);          //H[15] = T8
                    Strassen(&H[23*h*h], &H[14*h*h], &H[15*h*h], h);   //H[23] = S6
                }
                #pragma omp task
                {
                    sub(&H[16*h*h], &H[h*h], &H[3*h*h], h);            //H[16] = T9
                    add(&H[17*h*h], &H[6*h*h], &H[7*h*h], h);          //H[17] = T10
                    Strassen(&H[24*h*h], &H[16*h*h], &H[17*h*h], h);   //H[24] = S7
                }
            
                #pragma omp taskwait
            
            
                #pragma omp task
                {
                add(&H[25*h*h], &H[18*h*h], &H[21*h*h], h);       //H[25] = T11
                }
                #pragma omp task
                {
                    add(&H[26*h*h], &H[19*h*h], &H[21*h*h], h);       //H[26] = T12
                }
                #pragma omp task
                {
                    add(&H[27*h*h], &H[20*h*h], &H[23*h*h], h);       //H[27] = T13
                }
                #pragma omp task
                {
                    sub(&H[28*h*h], &H[24*h*h], &H[22*h*h], h);       //H[28] = T14
                }
                #pragma omp task
                {
                    add(&H[29*h*h], &H[20*h*h], &H[22*h*h], h);       //H[29] = T16
                }
                #pragma omp task
                {
                    sub(&H[30*h*h], &H[18*h*h], &H[19*h*h], h);       //H[30] = T17
                }
                
                #pragma omp taskwait
                
                
                #pragma omp task
                {
                    add(&H[31*h*h], &H[25*h*h], &H[28*h*h], h);       //H[31] = T15
                }
                #pragma omp task
                {
                    add(&H[32*h*h], &H[27*h*h], &H[30*h*h], h);       //H[32] = T18
                }
                
                #pragma omp taskwait
            }
        }
        
        copy_back(C, 0, 0, &H[31*h*h], N);  //C00
        copy_back(C, 0, h, &H[29*h*h], N);  //C01
        copy_back(C, h, 0, &H[26*h*h], N);  //C10
        copy_back(C, h, h, &H[32*h*h], N);  //C11
    
        free(H);
    }
    
}


int main (int argc, char* argv[]) {
    int* A;
    int* B;
    int* C1;
    int* C2;
    int* C0;
    
    int N = 512;  //степень 2
    
    A = (int*)malloc((N * N) * sizeof(int));
    B = (int*)malloc((N * N) * sizeof(int));
    C1 = (int*)malloc((N * N) * sizeof(int));
    C2 = (int*)malloc((N * N) * sizeof(int));
    C0 = (int*)malloc((N * N) * sizeof(int));
   
    
// матрицу удобнее рассматривать как одномерный массив но с индексами i*n+j
// заполняем матрицы рандомными числами от 0 до 4
//    printf("A:\n");
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N ; j++){
            A[i*N+j] = rand() % 5;
//            printf("%d ", A[i*N+j]);
        }
//        printf("\n");
    }
    
//    printf("B:\n");
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            B[i*N+j] = rand() % 5;
//            printf("%d ", B[i*N+j]);
        }
//        printf("\n");
    }
    
    fflush(stdout);
    
    /*
    double t0 = omp_get_wtime();
    
    mul_normal(C0, A, B, N);
    
    t0 = omp_get_wtime() - t0;
    printf("Time for non-parallel normal method: %f \n", t0);
    */
    
    
    double t1 = omp_get_wtime();
    
    Strassen(C1, A, B, N);
    
    t1 = omp_get_wtime() - t1;
    printf("Time for non-parallel: %f \n", t1);
    
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N ; j++){
//            printf("%d ", C1[i*N+j]);
        }
//        printf("\n");
    }
    
    double t2 = omp_get_wtime();
    
    StrassenParallel(C2, A, B, N);
    
    t2 = omp_get_wtime() - t2;
    printf("Time for parallel: %f \n", t2);
    
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N ; j++){
//            printf("%d ", C2[i*N+j]);
        }
//        printf("\n");
    }
    
    free(C0);
    free(C1);
    free(C2);
    
    free(A);
    free(B);
    
    return 0;
}
