#include <iostream>
#include <math.h>
#include <vector>
#include <cstring>
#include <string>

using namespace std;

// Tìm các số nguyên tố trong đoạn
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


// Tổng các số nguyên tố tìm được
long long sumPrimesOnSegment(vector<int> primes) {
    long long sum = 0;
    for(int prime : primes) {
        sum += prime;
    }
    return sum;
}

// Tích các số nguyên tố tìm được
unsigned long long productPrimesOnSegment(vector<int> primes) {
    unsigned long long product = 1;

    for (int prime : primes) {
        product *= prime;
    }
    return product;
}

// Tìm số nguyên tố gần với số x nhất
pair<int, int> findClosestPrime(const vector<int>& primes, int x) {
    if (primes.empty()) {
        throw invalid_argument("Prime vector is empty.");
    }
    
    int closestPrime = primes[0];
    int minDistance = abs(primes[0] - x);

    for (int prime : primes) {
        int distance = abs(prime - x);
        if (distance < minDistance) {
            closestPrime = prime;
            minDistance = distance;
        }
    }
    
    return {closestPrime, minDistance};
}


int main() {
    int a = 50;
    int n = 500;
    int x = 301;
    vector<int> primesOnSegment = SieveOfEratosthenesOnSegment(a, n);
    cout << "Cac so nguyen to tu trong doan [" << a << ", " << n << "]: ";
    for(int x : primesOnSegment) cout << x << " ";  
    cout << "\nTong cac so nguyen to trong doan [" << a << ", " << n << "]: " << sumPrimesOnSegment(primesOnSegment) << endl;
    // cout << "Tich cac so nguyen to trong doan [" << a << ", " << n << "]: " << productPrimesOnSegment(primesOnSegment) << endl;
    auto [closestPrime, distance] = findClosestPrime(primesOnSegment, x);
    cout << "So nguyen to gan nhat voi " << x << " la " << closestPrime << " voi khoang cach " << distance << "\n";
}