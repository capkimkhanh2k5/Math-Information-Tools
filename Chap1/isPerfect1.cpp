#include <iostream>
#include <math.h>
#include <vector>
#include <cstring>
#include <string>

using namespace std;

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

void numberPerfect(int n) {

}

int main() {
    int N = 95777;
    cout << "So hoan hao gan voi" << 3000 nhat la: " << M << endl;
    cout << endl;
    cout << "Cac so nguyen to be hon " << M << " la:\n";
    int sum = SieveOfEratosthenes(M);
    cout << "\nTong cac so nguyen to la: " << sum << endl;
    return 0;
}