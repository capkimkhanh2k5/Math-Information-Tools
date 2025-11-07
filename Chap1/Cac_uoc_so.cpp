#include <iostream>
#include <math.h>
#include <vector>
#include <cstring>
#include <string>
#include <limits>

using namespace std;

const float e = 2.718281828;

vector<int> primes, alphas;

bool isPrimes(int n) {
    if(n <= 1) return false;
    for(int i = 2; i <= sqrt(n); i++) {
        if(n % i == 0) return false;
    }
    return true;
}

void create_primes(int n) {
    for(int i = 1; i <= n; i++) {
        if(isPrimes(i)) primes.push_back(i);
    }
}

void create_alphas(int n) {
    for(int i = 0; i < primes.size(); i++) {
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
            if(alphas[i] == 1) cout <<  "*" << primes[i];
            else cout << "*" << primes[i] << "^" << alphas[i];
        }
    }
    cout << endl;
}

void display_divisor(int n) {
    int cout_divisor = 1;
    for(int i = 0; i < alphas.size(); i++) {
        if(alphas[i]) cout_divisor *= alphas[i] + 1;
    }
    cout << "So luong cac uoc so: " << cout_divisor << endl;
    cout << "Cac uoc so: ";
    for(int i = 1; i <= n; i++) {
        if(n % i == 0) cout << i << " ";
    }
    cout << endl;
}

void sum_divisors(int n) {
    int sum = 0;
    for(int i = 1; i <= n; i++) {
        if(n % i == 0) {
            sum += i;
        }
    }
    cout << "Tong cac uoc so: " << sum << endl;
}

void product_divisors(int n) {
    long long product = 1;
    for(int i = 1; i <= n; i++) {
        if(n % i == 0) {
            // Kiểm tra tràn số trước khi nhân
            if (product > LLONG_MAX / i) {
                cout << "Tich cac uoc so: Bi tran so" << endl;
                return;
            }
            product *= i;
        }
    }
    cout << "Tich cac uoc so: " << product << endl;
}

int main() {
    int n = 50;
    create_primes(n);
    create_alphas(n);

    display_divisor(n);

    sum_divisors(n);
    product_divisors(n);

    cout << endl;
    return 0;
}
