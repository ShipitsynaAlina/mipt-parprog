//
//  Integral.c
//  
//
//  Created by Alina Shipitsyna on 12.05.2020.
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>



double f(double x){
    return cos(1/(x*x));
}


double q_integral(double left, double right, double f_left, double f_right, double int_current, double EPS){
    double mid = (left + right)/2;
    double f_mid = f(mid);
    
//    левый отрезок
    double l_integral = (f_left + f_mid)*(mid - left)/2;
//    правый отрезок
    double r_integral = (f_mid + f_right)*(right - mid)/2;

    
    if(fabs((l_integral + r_integral) - int_current) > EPS){
        #pragma omp task shared(l_integral)
            l_integral = q_integral(left, mid, f_left, f_mid, l_integral, EPS/2);
        #pragma omp task shared(r_integral)
            r_integral = q_integral(mid, right, f_mid, f_right, r_integral, EPS/2);
    }
    
    #pragma omp taskwait
    return (l_integral + r_integral);
}




int main(){
    double a = 0.01;
    double b = 1;
    double area = 0;
    double EPS = 1.e-6;
    
    printf("accurate result: %f \n", -0.092475);
    
    omp_set_max_active_levels(0);
    omp_set_dynamic(0);
    double t = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single nowait
            area = q_integral(a, b, f(a), f(b), (f(a)+f(b))*(b-a)/2, EPS);
    }
    
    t = omp_get_wtime() - t;
    printf("approximate result: %.7f \n", area);
    printf("Time: %f \n", t);
    
    printf("eps: %.7f \n",  EPS);
    
    return 0;
}
