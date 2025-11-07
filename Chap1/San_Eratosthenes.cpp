#include <iostream>
#include <math.h>
#include <vector>
#include <cstring>
#include <string>

using namespace std;

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

int main() {
    int n = 496;
    cout << "Cac so nguyen to tu 2 toi " << n << ": ";
    SieveOfEratosthenes(n);
    cout << endl;
}