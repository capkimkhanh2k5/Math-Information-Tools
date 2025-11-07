#include <iostream>
#include <math.h>
#include <vector>


using namespace std;

int gcd1(int x, int y) {
    if (x == 0) return y;
    if(y == 0) return x;

    while (x != y) {
        if(x > y) x -= y;
        else y -= x;
    }
    return x;
}

int gcd2(int x, int y) {
    if (y == 0)
        return x;
    if(x == 0) return y;
    return gcd2(y, x % y);
}

int lcm(int x, int y) {
    return (x * y) / gcd2(x, y);
}


int main() {
    int x, y;
    x = 15; 
    y = 55;
    cout << "Uoc chung lon nhat 2 so " << x << " va " << y << " : " << gcd1(x, y) << endl; 
    cout << endl;
    cout << "Boi chung nho nhat 2 so " << x << " va " << y << ": " << lcm(x, y) << endl;

    return 0;
}