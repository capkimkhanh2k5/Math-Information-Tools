#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

int x, y; // Hệ số của phương trình ax + by = gcd(a, b)
/* 
    Với hệ phương trình ax + my = 1
    Lấy modun của m hay vế ta được:
        ax % m = 1
        => Khi đó x chính là nghịch đảo modun của a
*/
int gcd; // Ước chung của a và b

// Phương pháp Euclid để tìm ước chung của 2 số đồng thời tìm hệ số x và y
void extendedEuclid(int a, int b) {
    if(b == 0) {
        x = 1; y = 0; gcd = a;
    }
    else {
        extendedEuclid(b, a % b);
        int temp = x;
        x = y;
        y = temp - (a / b) * y;
    }
}

// Phương pháp tìm nghịch đảo modulo của a mod m
int modunlarInverse(int a, int m) {
    extendedEuclid(a, m); // Tìm hệ số x
    return (x + m) % m; // Khi x < 0 thì trả về x + m
}


// Các phần tử trong vector M là đôi một nguyên tố cùng nhau
void input(int n, vector<int> &A, vector<int> &M) {
    cout << "Nhap cac phuong trinh (a mod m): \n";
    for(int i = 0; i < n; i++) {
        cout << "Nhap phuong trinh so " << i + 1 << ": ";
        int a, m; cin >> a >> m;
        A.push_back(a);
        M.push_back(m);
    }
}

long long ChineseRemainderTheorem(int n, vector<int> A, vector<int> M) {
    long long result = 0;
    long long prod = 1; // Tích các m1*m2*...*mn
    for(int i = 0; i < n; i++) {
        prod *= M[i];
    }
    for(int i = 0; i < n; i++) {
        long long temp = prod / M[i];
        result = (result + A[i] * temp * modunlarInverse(temp, M[i])) % prod;
    }
    return result;
}

int main() {
    int n;
    cout << "Nhap so phuong trinh: "; cin >> n;
    vector<int> A, M;
    input(n, A, M);
    long long prod = 1; // Tích các m1*m2...*mn
    for(int x : M) prod *= x;
    long long result = ChineseRemainderTheorem(n, A, M);
    cout << endl;
    cout << "Ket qua: x = " << result << " + k" << prod << endl;
    return 0;
}