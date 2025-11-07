#include <iostream>
#include <math.h>
#include <vector>
#include <cstring>
#include <string>

using namespace std;

const float e = 2.718281828;

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

void sum_product_divisor(int n) {
    int sum = 0;
    long long product = 1;
    for(int i = 1; i <= n; i++) {
        if(n % i == 0) {
            sum += i;
            product *= i;
        }
    }
    cout << "Tong cac uoc so: " << sum << endl;
    cout << "Tich cac uoc so: " << product << endl;
}

bool isPerfectNumber1(int n) {
    int sum = 0;
    for(int i = 1; i < n; i++) {
        if(n % i == 0) {
            sum += i;
        }
    }
    if(n == sum) return true;
    else return false;
}

bool isPerfectNumber2(int n) {
    long long xichma = 1;
    for(int i = 0; i < alphas.size(); i++) {
        if(alphas[i]) xichma *= (long long)((pow(primes[i], alphas[i] + 1) - 1) / (primes[i] - 1));
    }
    if(n == xichma - n) return true;
    else return false;
}

int densityOfPrimes(int n) {
    return (int)(n / (log(n)/log(e)));
}

void SieveOfEratosthenes(int n) {
    vector<bool> primes(n+1, true);
    for(int i = 2; i <= sqrt(n); i++) {
        if(!primes[i]) continue;
        for(int j = i*i; j <= n; j += i) {
            primes[j] = false;   
        }
    }

    // Display primes
    for(int i = 2; i < primes.size(); i++) {
        if(primes[i]) cout << i << " ";
    }
}

vector<int> SieveOfEratosthenesOnSegment(int a, int b) {
    vector<bool> primes(b - a + 1, true);
    for(int i = 2; i <= sqrt(b); i++) {
        for(int j = max(i*i, (a + i - 1)/i * i); j <= b; j += i)
            primes[j - a] = false;
    }

    vector<int> result;
    for (int i = 0; i <= b - a; i++) {
        if (primes[i]) {
            result.push_back(i + a);
        }
    }
    return result;
}

long long sumPrimesOnSegment(vector<int> primes) {
    long long sum = 0;
    for(int prime : primes) {
        sum += prime;
    }
    return sum;
}

unsigned long long productPrimesOnSegment(vector<int> primes) {
    unsigned long long product = 1;

    for (int prime : primes) {
        product *= prime;
    }
    return product;
}

int main() {

    int a = 20;
    int n = 50;
    create_primes(n);
    create_alphas(n);

    primeFactorization(n);

    display_divisor(n);

    sum_product_divisor(n);
    cout << endl;

    if(isPerfectNumber1(n)) cout << "Cach 1: " << n <<" la so hoan hao\n";
    else cout << "Cach 1: " << n <<" khong la so hoan hao\n";

    if(isPerfectNumber2(n)) cout << "Cach 2: " << n <<" la so hoan hao\n\n";
    else cout << "Cach 2: " << n <<" khong la so hoan hao\n\n";

    cout << "Cac so nguyen to tu 2 toi n: ";
    SieveOfEratosthenes(n);
    cout << endl;

    cout << "Mat do cac so nguyen to trong doan [2" << ", " << n << "]: " << densityOfPrimes(n) << endl;
    cout << endl;


    vector<int> primesOnSegment = SieveOfEratosthenesOnSegment(a, n);
    cout << "Cac so nguyen to tu trong doan [" << a << ", " << n << "]: ";
    for(int x : primesOnSegment) cout << x << " ";  

    cout << "\nTong cac so nguyen to trong doan [" << a << ", " << n << "]: " << sumPrimesOnSegment(primesOnSegment) << endl;
    cout << "Tich cac so nguyen to trong doan [" << a << ", " << n << "]: " << productPrimesOnSegment(primesOnSegment) << endl;
}