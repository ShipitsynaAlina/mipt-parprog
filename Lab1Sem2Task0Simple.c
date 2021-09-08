//
//  Lab1Sem2Task0Simple.c
//  
//
//  Created by Alina Shipitsyna on 25.11.2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
   
#define ISIZE 1000
#define JSIZE 1000
int main(int argc, char **argv)
{
    double a[ISIZE][JSIZE];
    FILE *ff;
    clock_t start, end;
    start = clock();
    
        for (int i=0; i<ISIZE; i++){
                for (int j=0; j<JSIZE; j++){
                a[i][j] = 10*i +j;
                }
    }



    
        for (int i=0; i<ISIZE; i++){
            for (int j = 0; j < JSIZE; j++){
                    a[i][j] = sin(0.00001*a[i][j]);
                }
    }
    
    

    
    end = clock();
    
    ff = fopen("Lab1_pre_seq.txt","w");
        for(int i=0; i < ISIZE; i++){
                for (int j=0; j < JSIZE; j++){
                    fprintf(ff,"%f ",a[i][j]);
                }
                fprintf(ff,"\n");
    }

                                
    fclose(ff);
    printf("time: %f\n", (double)(end-start)/ CLOCKS_PER_SEC);
    
    return 0;
}
