#include <stdio.h>
#include <omp.h>
#include <vector>
#include <map>
#include <iostream>
#include <unistd.h>

using namespace std;


void print_map(map<int, vector<int> > m) {
    for (auto i : m) {
        cout << i.first << ": ";
        for (auto k : i.second)
            cout << k << ", ";
        cout << endl;
    }
}

void print_hello() {
#pragma omp parallel
    {
        usleep(omp_get_thread_num() * 100);
        std::cout << "Total number of nodes: " << omp_get_num_threads() << " My id: " << omp_get_thread_num() << std::endl;
    }
}
/*
void balansir()
{
    int i;
    map<int, vector<int>> stat, dynam1, dynam2, guided;
#pragma omp parallel private(i)
    {
#pragma omp for schedule (dynamic, 2)
        for (i = 0; i < 36; i++)
        {
#pragma omp critical 
            {
                stat[omp_get_thread_num()].push_back(i);
            }
            Sleep(1);
        }
    }
    cout << "Static:" << endl;
    print_map(stat);


#pragma omp parallel private(i)
    {
#pragma omp for schedule (dynamic)
        for (i = 0; i < 36; i++)
        {
#pragma omp critical 
            {
                dynam1[omp_get_thread_num()].push_back(i);
            }
        }       
    }
    cout << endl << "Dynamic1:" << endl;
    print_map(dynam1);


#pragma omp parallel private(i)
    {

#pragma omp for schedule (dynamic, 2)
        for (i = 0; i < 36; i++)
        {
#pragma omp critical
            {
                dynam2[omp_get_thread_num()].push_back(i);
            }
        }
        
    }
    cout << endl << "Dynamic2:" << endl;
    print_map(dynam2);

#pragma omp parallel private(i)
    {

#pragma omp for schedule (guided)
        for (i = 0; i < 36; i++)
        {
#pragma omp critical
            {
                guided[omp_get_thread_num()].push_back(i);
            }
        }

    }
    cout << endl << "Guided:" << endl;
    print_map(guided);
}
 */

double sum_of_row(int n) {
    double sum = 0;
#pragma omp parallel reduction (+: sum)
    {
#pragma omp for 
        for (int i = 1; i <= n; ++i)
            sum += 1/(double)i;
    }
    return sum;
}

int in_order() {
    int sum = 0;
#pragma omp parallel
    {
#pragma omp critical
        {
            sum += omp_get_thread_num();
        }
    }
    return sum;
}

int main() {
    print_hello();
    int k;
    cout << "Enter nuber of element in row" << endl;
    cin >> k;
    cout << "Sum of 1/n: " << sum_of_row(k) << endl;
    cout << "After circle summ: " << in_order() << endl;
    return 0;

}
