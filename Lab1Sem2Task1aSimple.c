//
//  Lab1Sem2Task1aSimple.c
//  
//
//  Created by Alina Shipitsyna on 26.11.2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define ISIZE 10000
#define JSIZE 10000

int main(int argc, char **argv) {
    double** a;
    int i, j;
    FILE *ff;
    double t_start, t_end;
    
    a = (double**)malloc(ISIZE  * sizeof(double));
    for (i = 0; i < ISIZE; i++){
        a[i] = (double*)malloc(JSIZE  * sizeof(double));
    }
    
    
    for (i = 0; i < ISIZE; i++){
        for (j = 0; j < JSIZE; j++) {
            a[i][j] = 10 * i +j;
        }
    }
    
    t_start = clock();
    
    for (i = 1; i < ISIZE; i++) {
        for (j = 0; j < JSIZE - 1; j++) {
            a[i][j] = sin(0.00001 * a [i - 1][j + 1]);
        }
    }
    
    t_end = clock();
    
    printf("Time: %lf\n", (t_end - t_start) / CLOCKS_PER_SEC);
        
    ff = fopen("resultSimple.txt","w");
    
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

