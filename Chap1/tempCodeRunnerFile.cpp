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