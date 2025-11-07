#include <iostream>
#include <math.h>
#include <vector>
#include <cstring>
#include <string>

using namespace std;

const float e = 2.718281828;
int densityOfPrimes(int n) {
    return (int)(n / (log(n)/log(e)));
}


int main() {
    int n = 50;

    cout << "Mat do cac so nguyen to trong doan [2" << ", " << n << "]: " << densityOfPrimes(n) << endl;
    cout << endl;
    return 0;
}