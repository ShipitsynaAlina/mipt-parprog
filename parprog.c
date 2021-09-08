//
//  parprog.c
//  
//
//  Created by Alina Shipitsyna on 30.10.2020.
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main (int argc, char* argv[]) {
    int a = 3;
    #pragma omp parallel for lastprivate(a) num_threads(4)
    for (int i = 0; i < 5; i++)
    {
        a = i;
    }
    printf ("%d\n", a);
}
