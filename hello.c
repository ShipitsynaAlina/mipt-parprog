//g++ -fopenmp hello.cpp
//./a.out
#include "omp.h"

#include <iostream>
using namespace std;
int main() {
    omp_set_num_threads(3);
    #pragma omp parallel
    {

        cout << "Hello World... from thread = " << omp_get_thread_num() << endl;
    }
    return 0;
}
