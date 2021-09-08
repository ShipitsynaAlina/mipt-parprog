//
//  Task2.c
//  
//
//  Created by Alina Shipitsyna on 16.12.2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>


int main(int argc, char **argv)
{
    int N = 4096;
    int log_N = (int)log2(N);
    double a[log_N][N+1];
    double b[log_N][N+1];
    double c[log_N][N+1];
    double f[log_N][N+1];
    int i, j, r, k;
    int step;
    float B = 1000;
    double delta[N+1];

    float h = 1./N;
    FILE *ff;

    delta[0]=0;
    delta[N]=0;

    omp_set_num_threads(4);
    double start = omp_get_wtime();
    r=0;
    #pragma omp parallel for
    for(k=1; k < N; k++)
    {
        a[r][k]= 1./(h*h) - (1./12) * 5 * pow(((B-2)*(k-1)*h + 1), 4);
        b[r][k]= 2./(h*h) + (10./12)* 5 * pow(((B-2) * k * h + 1), 4);
        c[r][k]= 1./(h*h) - (1./12) * 5 * pow(((B-2) * (k + 1)* h + 1), 4);
        f[r][k]= -((1./12)*(5 * pow(((B-2) * (k + 1)* h + 1), 4) + (10./12.0) * 5 * pow(((B-2) * k * h + 1), 4) + (1./12.0) * 5 * pow(((B-2)*(k-1)*h + 1), 4)));
    }


    for (r = 1; r < log_N; r++)
    {
        #pragma omp parallel for
        for(k=1; k < (int)(N/pow(2,r)); k++)
        {
            a[r][k] = a[r-1][2*k-1]*a[r-1][2*k]/b[r-1][2*k-1];
            b[r][k] = -(c[r-1][2*k-1]*a[r-1][2*k]/b[r-1][2*k-1] - b[r-1][2*k]+c[r-1][2*k]*a[r-1][2*k+1]/b[r-1][2*k+1]);
            c[r][k] = c[r-1][2*k]*c[r-1][2*k+1]/b[r-1][2*k+1];
            f[r][k] = f[r-1][2*k-1]*a[r-1][2*k]/b[r-1][2*k-1] + f[r-1][2*k] + f[r-1][2*k+1] + c[r-1][2*k]/b[r-1][2*k+1];
        }
    }


    for (k = 1; k <= log_N; k++)
    {
        i = 1;
        step = (int)(N/pow(2,k-1));
        for (j = (int)(N/pow(2,k)); j < N; j += step)
        {
            delta[j] = (f[log_N-k][i]-c[log_N-k][i]*delta[j+step/2] - a[log_N-k][i]*delta[j-step/2])/-b[log_N-k][i];
            i+= 2;
        }
    }

    double end = omp_get_wtime();

    

    ff = fopen("result.txt","w");
    for(k=0; k < N+1; k++)
        fprintf(ff,"%f ",delta[k]+(B-1)*k*h+1);

    fprintf(ff,"\n");

    fclose(ff);
    
    printf("N = %d\n", N);
    printf("time %lf\n", end-start);
    

return 0;
}
