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

bool isPerfectNumber2(int n) {
    long long xichma = 1;
    for(int i = 0; i < alphas.size(); i++) {
        if(alphas[i]) xichma *= (long long)((pow(primes[i], alphas[i] + 1) - 1) / (primes[i] - 1));
    }
    if(n == xichma - n) return true;
    else return false;
}

int main() {
    int n = 496;
    create_primes(n);
    create_alphas(n);

    if(isPerfectNumber2(n)) cout << n <<" la so hoan hao\n\n";
    else cout << n <<" khong la so hoan hao\n\n";
}