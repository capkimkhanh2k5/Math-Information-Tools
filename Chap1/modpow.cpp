#include <iostream>
#include <math.h>

using namespace std;

int modPow1(int x, int n, int m) {
    int result = 1;
    x = x % m;
    while(n) {
        if(n % 2) {
            result *= x;
            result = result % m; 
        }
        x *= x;
        x %= m;
        n /= 2;
    }
    return result;
}

int modPow2(int x, int n, int m) {
    if(n == 0) return 1 % m;
    long long result = modPow2(x, n / 2, m);
    result = (result * result) % m;
    if(n % 2) result = (result * x) % m;
    return result;
}

int main() {
    int x, n, m;
    cout << "Nhap x, n, m: ";
    cin >> x >> n >> m;
    cout << "Ket qua 1: " << modPow1(x, n, m) << endl;
    cout << "Ket qua 2: " << modPow2(x, n, m) << endl;
    return 0;
}