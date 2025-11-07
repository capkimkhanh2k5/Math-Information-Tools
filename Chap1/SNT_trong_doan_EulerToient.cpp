#include <iostream>
#include <math.h>
#include <vector>


using namespace std;

vector<int> primes, alphas;

bool check_primes(int n) {
    if(n <= 1) return false;
    for(int i = 2; i <= sqrt(n); i++) {
        if(n % i == 0) return false;
    }
    return true;
}

void create_primes(int n) {
    for(int i = 1; i <= n; i++) {
        if(check_primes(i)) primes.push_back(i);
    }
}

void create_alphas(int n) {
    for(int i = 0; i < primes.size() ; i++) {
        int exponent = 0;
        while(n != 1) {
            if(n % primes[i] == 0) {
                exponent++;
                n /= primes[i];
            } else break;
        }
        alphas.push_back(exponent);
    }
}

void primeFactorization(int n) {
    cout << n << " = ";
    int i; // Biến đếm
    for(i = 0; i < primes.size(); i++) {
        if(alphas[i]) {
            if(alphas[i] == 1) cout << primes[i];
            else cout << primes[i] << "^" << alphas[i];
            break;
        }
    }
    for(i = ++i; i < primes.size(); i++) {
        if(alphas[i]) {
            if(alphas[i] == 1) cout << "*" << primes[i];
            else cout << "*" << primes[i] << "^" << alphas[i];
        }
    }
    cout << endl;
}

int gcd1(int x, int y) {
    if (x == 0) return y;
    if(y == 0) return x;

    while (x != y) {
        if(x > y) x -= y;
        else y -= x;
    }
    return x;
}


void EulerTotient(int n, vector<int> primes, vector<int> alphas) {
    long long result = 1;
    for(int i = 0; i < primes.size(); i++) {
        if(alphas[i]) result *= pow(primes[i], alphas[i] - 1) * (primes[i] - 1);
    }
    cout << "Ham Euler cua " << n << " la: " << result << endl;
    cout << "Cac so nguyen to cung nhau voi " << n << " trong [" << 1 << ", " << n << "]: ";
    for(int i = 1; i <= n; i++) {
        if(gcd1(i, n) == 1) cout << i << " ";
    }
}




int main() {
    int n = 53;
    create_primes(n);
    create_alphas(n);
    EulerTotient(n, primes, alphas);

    return 0;
}