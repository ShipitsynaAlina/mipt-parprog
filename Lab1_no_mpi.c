//
//  Lab1_no_mpi.c
//  
//
//  Created by Alina Shipitsyna on 10.04.2020.
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//
double f(x, t){
    return 5*x*t;
}

double fi(x){
    return -5/6*x*x*x;
}

double ksi(t){
    return 0;
}


int main(int argc, char* argv[]){
    clock_t start_time, finish_time;

    int X = 24000, T = 1;
    
    int M = 800000, K = 5; //  M кратно числу процессов
    double h, tau; // шаги
    
    int k, m;
    
//    размер шагов
    h = (double)X/M;
    tau = (double)T/K;
    
    double* u; // двумерный массив со значениями сетки в каждом узле
    double* x;
    double* t;
 
    u = (double*)malloc((M*K) * sizeof(double));
    x = (double*)malloc(M * sizeof(double));
    t = (double*)malloc(K * sizeof(double));
   
//    задание сетки
    for (m = 0; m < M; m++)
        x[m] = m * h;
    for (k = 0; k < K; k++)
        t[k] = k * tau;
    
//    задание начальных условий
    for (m = 0; m < M; m++)
        u[m] = -(5.0)/(6.0)*x[m]*x[m]*x[m];
    for (k = 0; k < K; k++)
        u[k*M] = 0;

//    старт времени
    start_time = clock();
    

    for (k = 1; k < K; k++){
        for (m = 0; m < M; m++){
            if (m == M-1)
                u[k*M + m] = u[(k-1)*M + m] - tau/h*(u[(k-1)*M + m] - u[(k-1)*M + (m-1)]) + tau*5*x[m]*t[k-1];
            else if (m == 0)
                u[k*M + m] = 0;
            else
                u[k*M + m] = 0.5*(1 - tau/h)*u[(k-1)*M + (m+1)] + 0.5*(1 + tau/h)*u[(k-1)*M + (m-1)] + tau*5*x[m]*t[k-1];
        }
    }
    
//  выводится результат
    finish_time = clock();
    printf("%f \n", u[M*(K-1) + M-1]);
    
    double time;
    time = (double)(finish_time-start_time)/CLOCKS_PER_SEC;
    printf("%f \n", time);
    
    
    free(u);
    
    return 0;
}
