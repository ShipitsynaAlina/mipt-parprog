#include<stdio.h>
#include<omp.h>

int main() {
    int num_threads = 0;
    printf("Enter num_threads\n");
    scanf("%d", &num_threads);
    int num = 0;
    #pragma omp parallel num_threads(num_threads)
    {
        do {
            int nthread = omp_get_thread_num();
            #pragma omp critical 
            if(num == nthread){
                num++;
                printf("thread num [%d] my_num = %d\n", nthread, num);
            }
        } while(num < num_threads);
    }
    return 0;
}
