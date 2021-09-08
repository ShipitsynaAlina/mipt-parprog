//
//  Lab1Sem2Task1a.c
//  
//
//  Created by Alina Shipitsyna on 25.11.2020.
//

#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
#include <math.h>


#define ISIZE 10000
#define JSIZE 10000

#define N_THREADS 4

int main(int argc, char **argv) {
    double** a;
    int i, j;
    FILE *ff;
    double t_start, t_end;
    
    a = (double**)malloc(ISIZE  * sizeof(double));
    for (i = 0; i < ISIZE; i++){
        a[i] = (double*)malloc(JSIZE  * sizeof(double));
    }
    
//    omp_set_dynamic(1);
//    omp_set_nested(1);
    #pragma omp parallel for
    {
    for (i = 0; i < ISIZE; i++){
        #pragma omp parallel for
        {
        for (j = 0; j < JSIZE; j++) {
            a[i][j] = 10 * i +j;
        }
        }
    }
    }
    
    if (omp_get_dynamic())
        omp_set_dynamic(0);
    
    omp_set_num_threads(N_THREADS);

    t_start = omp_get_wtime();
    #pragma omp parallel
    {
    for (i = 1; i < ISIZE; i++) {
        #pragma omp for
        {
        for (j = 0; j < JSIZE - 1; j++) {
            a[i][j] = sin(0.00001 * a [i - 1][j + 1]);
        }
//        #pragma omp barrier
        }
    }
    }
    
    t_end = omp_get_wtime();
    
    printf("Time: %lf\n", t_end - t_start);
        
    ff = fopen("result.txt","w");
    
    for(i = 0; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
            fprintf(ff,"%f ",a[i][j]);
        }
        fprintf(ff,"\n");
    }
    
    fclose(ff);
    
    free(a);
    
    return 0;
}
