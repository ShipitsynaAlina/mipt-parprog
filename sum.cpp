#include <iostream>
#include <sstream>
#include <omp.h>
#include <time.h>
#include <string>

#define n_digits 7
#define n_chars 3   
#define N 200

using namespace std;
typedef long unsigned int type;

void product(type* input, bool parallel) {
    if (parallel) {
        int i = 0;
        type q;
        type r;
        #pragma omp parallel for private(i, r, q)
        for (i = 1; i <= N; ++i) {
            r = 1;
            for (q = 0; q < n_digits + n_chars + 1 && r; ++q) {
                type div = r / i;
                type mod = r % i;
                #pragma omp atomic
                input[q] += div;
                r = mod * 10;
            }
    }
    } else {
        for (int i = 1; i <= N; ++i) {
            type r = 1;
            for (type q = 0; q < n_digits + n_chars + 1 && r; ++q) {
                type div = r / i;
                type mod = r % i;
                input[q] += div;
                r = mod * 10;
            }
        }
    }
}


void sum(char* output, bool parallel) {
    int ord = n_digits + n_chars + 1;
    type dgs[ord];
    for (int dg = 0; dg < ord; ++dg)
        dgs[dg] = 0;

    product(dgs, parallel);

    // заполнение дробной части
    for (int i = ord - 1; i > 0; --i) {
        dgs[i - 1] += dgs[i] / 10;
        dgs[i] %= 10;
    }
    if (dgs[n_digits + 1] >= 5) {
        ++dgs[n_digits];
    }
    // заполнение целой части
    for (int i = n_digits; i > 0; --i) {
        dgs[i - 1] += dgs[i] / 10;
        dgs[i] %= 10;
    }
    stringstream ss;
    ss << dgs[0] << ".";


    for (int i = 1; i <= n_digits; ++i) {
        ss << dgs[i];
    }
    string str = ss.str();
    str.copy(output, str.size());
}


int main() {
    cout << "Strndard Method: " << endl;
    char output1[n_digits + n_chars];
    sum(output1, 0);
    cout << output1 << endl;

    cout << "Parallel Method: " << endl;
    char output2[n_digits + n_chars];
    sum(output2, 1);
    cout << output2 << endl;


    return 0;
}
