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

bool isPerfect(int n) {
    int sum = 0;
    for(int i = 1; i < n; i++) {
        if(n % i == 0) {
            sum += i;
        }
    }
    if(n == sum) return true;
    else return false;
}

int numberPerfect(int n) {
    int count = 0;
    for(int i = 1; i <= n; i++) {
        if(isPerfect(i)) {
            cout << i << " ";
            count++;
        }
    }
    return count;
}

int GCD_max(vector<int> primes, int n) {
    for(int i = primes.size() - 1; i >= 0; i--) {
        if(n % primes[i] == 0) {
            return primes[i];
        }
    }
    return 1;
}

int main() {
    int n = 95777;
    create_primes(n);
    create_alphas(n);
    int M = GCD_max(primes, n);
    cout << "Uoc so nguyen to lon nhat cua " << n << " la "<< M << endl;
    cout << "Cac so hoan hao nho hon " << M << ":";
    int count = numberPerfect(M); 
    cout << "\nSo hoan hao nho hon " << M << " co " << count << " so.";
    return 0;

}