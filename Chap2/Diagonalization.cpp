//Chéo hoa ma trận

#include <iostream>
#include <iomanip>
#include <math.h>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

const float pi = 3.1415926535898;

#define MAX_SIZE 4

typedef float Matrix[MAX_SIZE][MAX_SIZE];

Matrix A; // Ma trận ban đầu

// Vector lưu các giá trị riêng
vector<float> lamda;

Matrix P, P1; // P1 là ma trận nghịch đảo của P

Matrix D; // Ma trận chéo hóa


void inputMatrix(Matrix &A, int n) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cin >> A[i][j];
        }
    }
}

void outputMatrix(Matrix A, int n) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cout << fixed << setprecision(3) << A[i][j] << "\t";
        }
        cout << endl;
    }
}

// C = A x B
void mulMatrix(const Matrix &a, const Matrix &b, Matrix &c, int n) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            c[i][j] = 0; 
            for(int k = 0; k < n; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void copyMatrix(Matrix &a, const Matrix &b, int n) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            a[i][j] = b[i][j];
        }
    }
}

bool compareMatrix(const Matrix &a, const Matrix &b, int n) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(fabs(a[i][j] - b[i][j]) > 0.001) {
                return false;
            }
        }
    }
    return true;
}

bool inverseMatrix(const Matrix &A, Matrix &inverse, int n) {
    Matrix temp;
    copyMatrix(temp, A, n);
    // Khởi tạo ma trận nghịch đảo là ma trận đơn vị
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            inverse[i][j] = (i == j) ? 1.0 : 0.0;
    }

    for (int i = 0; i < n; i++)
    {
        if (temp[i][i] == 0)
        {
            // Tìm hàng khác có phần tử ở cột i khác 0 để đổi hàng
            int check = 0;
            for (int j = i + 1; j < n; j++)
            {
                if (temp[j][i] != 0)
                {
                    for (int k = 0; k < n; k++)
                    {
                        swap(temp[i][k], temp[j][k]);
                        swap(inverse[i][k], inverse[j][k]);
                    }
                    check++;
                    break;
                }
            }
            if (check == 0)
                return false;  // Ma trận không khả nghịch
        }

        // Chia hàng hiện tại cho phần tử chéo chính temp[i][i] để đưa về 1
        double diag = temp[i][i];
        for (int j = 0; j < n; j++)
        {
            temp[i][j] /= diag;
            inverse[i][j] /= diag;
        }

        // Khử các phần tử khác ở cột i
        for (int j = 0; j < n; j++)
        {
            if (j != i)
            {
                double factor = temp[j][i];
                for (int k = 0; k < n; k++)
                {
                    temp[j][k] -= factor * temp[i][k];
                    inverse[j][k] -= factor * inverse[i][k];
                }
            }
        }
    }
    return true;  // Ma trận nghịch đảo thành công
}

// giải phương trình bật 3 tổng quát
vector<float> Trigonometrical(vector<float> A, int n) {
    // Các hệ số của phương trình
    float a = A[0];
    float b = A[1];
    float c = A[2];
    float d = A[3];
    float delta = b*b - 3*a*c;
    float k = (9*a*b*c - 2*pow(b, 3) - 27*a*a*d) / (2*sqrt(abs(pow(delta, 3))));
/*
    Nếu delta > 0
        Nếu |k| <= 1: phương trình có 3 nghiệm
        Nếu |k| > 1: phương trình có một nghiệm duy nhất
    Nếu delta = 0: Phương trình có một nghiệm bội
    Nếu delta < 0: Phương trình có một nghiệm duy nhất
    Trong bài toán này chỉ đề cập tới trường hợp có 3 nghiệm
*/
    vector<float> Solution;
    Solution.push_back((2*sqrt(delta) * cos(acos(k)/3) - b) / (3 * a));
    Solution.push_back((2*sqrt(delta) * cos(acos(k)/3 - 2*pi/3) - b) / (3 * a));
    Solution.push_back((2*sqrt(delta) * cos(acos(k)/3 + 2*pi/3) - b) / (3 * a));
    return Solution;
} 

void Danhilepxki(const Matrix &A, int n) {
    Matrix tmpA; // Ma trận copy từ ma trận A, biến đổi thành ma trận đồng dạng với ma trận A
    copyMatrix(tmpA, A, n);
    Matrix B; // B = A x M
    Matrix M1, M; // M1 là ma trận nghịch đảo của M
    Matrix C; // Tích các ma trận M biến đổi từ lần thứ 1->n-1 
    Matrix tmp;
    for(int k = n - 2; k >= 0; k--) {
        // Tính ma trận M và M1 (M1 là ma trận nghịch đảo của M)
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                if(i != k) {
                    if(i == j) {
                        M[i][j] = 1; M1[i][j] = 1;
                    } else {
                        M[i][j] = 0; M1[i][j] = 0;
                    }
                } else {
                    M1[i][j] = tmpA[k + 1][j];
                    if(j == k) {
                        M[i][j] = 1/tmpA[k + 1][k];
                    } else {
                        M[i][j] = -tmpA[k + 1][j] / tmpA[k + 1][k];
                    }
                }
            }
        }
        mulMatrix(tmpA, M, B, n);  // B = A x M
        mulMatrix(M1, B, tmpA, n); // A = M1 x B <=> tmpA = M1 x A x M
        
        // Tính ma trận C
        if(k == n - 2) {
            copyMatrix(C, M, n);
        } else {
            mulMatrix(C, M, tmp, n);
            copyMatrix(C, tmp, n); // C = C x M
        }
    }

    // Vector lưu phương trình với nghiệm là các giá trị riêng của ma trận A
    vector<float> equation;
    equation.push_back(1);
    for(int i = 0; i < n; i++)
        equation.push_back(-tmpA[0][i]);

    // Vector lưu các giá trị riêng
    lamda = Trigonometrical(equation, n);
    sort(lamda.begin(), lamda.end());

    // Các vector riêng của ma trận C lưu trên các cột của ma trận tmp
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            tmp[j][i] = pow(lamda[i], n - j - 1);
        }
    }
    
    // Các vector riêng của ma trận A được tính bằng cách P = C x P
    mulMatrix(C, tmp, P, n);
}

// Chéo hóa ma trận
bool Diagonalization(int n) {
    // Tính các giá trị riêng vector riêng của ma trận A
    Danhilepxki(A, n);
    
    // Tính ma trận chéo hóa D
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i == j) {
                D[i][j] = lamda[i];
            } else {
                D[i][j] = 0;
            }
        }
    }

    // Tính ma trận nghịch đảo P^-1
    inverseMatrix(P, P1, n);

    // Kiểm tra điều kiện A = P x D x P^-1
    Matrix tmp, B;
    mulMatrix(P, D, tmp, n);
    mulMatrix(tmp, P1, B, n);

    return compareMatrix(A, B, n);
        
}

int main() {
    int n;
    cout << "Nhap kich thuoc ma tran (nxn, n <=4): ";
    cin >> n;
    inputMatrix(A, n);

    if(Diagonalization(n)) {
        cout << "Ma tran cheo hoa thanh cong\n";
        cout << "Ma tran D:\n";
        outputMatrix(D, n);
        cout << "Ma tran P:\n";
        outputMatrix(P, n);
        cout << "Ma tran nghich dao P^-1:\n";
        outputMatrix(P1, n);
    }
    else cout << "Ma tran cheo hoa khong thanh cong\n";
    Diagonalization(n);
}