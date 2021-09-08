//
//  Lab1Sem2.cpp
//  
//
//  Created by Alina Shipitsyna on 25.11.2020.
//

#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
#include <time.h>
#include <math.h>

#define ISIZE 1000
#define JSIZE 1000


int main (int argc, char **argv) {
    double a[ISIZE][JSIZE];
    int i, j;
    
    FILE *ff;
    
    double t;
    
    t = omp_get_wtime();
    
    #pragma omp parallel
    {
        #pragma omp for
        {
            for (i = 0; i < ISIZE; i++){
                for (j = 0; j < JSIZE; j++){
                    a[i][j] = 10 * i + j;
                }
            }
        }
    
    
    
        #pragma omp for
        {
            for (i = 0; i < ISIZE; i++){
                for (j = 0; j < JSIZE; j++){
                    a[i][j] = sin(0.00001 * a[i][j]);
                }
            }
        }
    
    }
    t = omp_get_wtime() - t;
    
    printf("Time: %lf\n", t);
    
    ff = fopen("result.txt","w");
    
    
    
    for(i = 0; i < ISIZE; i++){
        for (j = 0; j < JSIZE; j++){
            fprintf(ff,"%f ",a[i][j]);
        }
        fprintf(ff,"\n");
    }
    
    
    fclose(ff);
    
    
    return 0;
}


