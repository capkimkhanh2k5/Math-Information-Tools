// Ma trận nhập vào phải là ma trận vuông

#include<iostream>
#include<math.h>
#include<cstring>
#include <iomanip>

using namespace std;


void inputMatrix(double A[][10], int n, int m);
void outputResult(double A[][10], double D[], int n);
bool checkMatrixCholesky(double A[][10], int n, int m);
bool SymmetricalMatrix(double A[][10], int n);
void CholeskyLDL_Decomposition(double A[][10], double L[][10], double D[], int n);

int main(){
    double A[10][10];
    double L[10][10];
    double D[10];
    int n, m;
    cout << "Nhap kich thuoc ma tran: "; cin >> n >> m;
    cout << "Nhap ma tran:\n";
    inputMatrix(A, n, m);
    if(!checkMatrixCholesky(A, n, m)){
        cout << "Ma tran khong the phan ra\n";
        return 0;
    } else {
        checkMatrixCholesky(A, n, m);
        CholeskyLDL_Decomposition(A, L, D, n);
        outputResult(L, D, n);
    }
    return 0;
}

void inputMatrix(double A[][10], int n, int m){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> A[i][j];
        }
    }
}

void outputResult(double A[][10], double D[], int n) {
    cout << "Matrix L : \n";
    cout << fixed << setprecision(3); 
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << A[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\nMatrix D : \n";
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) { 
            if(i == j)
                cout << D[i] << " ";
            else
                cout << "0.00" << " ";
        }
        cout << "\n";
    }
    cout << "\nMatrix L*: \n";
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << A[j][i] << " ";
        }
        cout << "\n";
    }
}

bool checkMatrixCholesky(double A[][10], int n, int m)
{
    if(n != m){
        return false;
    }
    // if(!SymmetricalMatrix(A, n)){
    //     return false;
    // }
    return true;
}

bool SymmetricalMatrix(double A[][10], int n)
{
    for(int i = 0; i < n; i++){
        for(int j = 0; j < i; j++){
            if(A[i][j]!= A[j][i]){
                return false;
            }
        }
    }
    return true;
}

void CholeskyLDL_Decomposition(double A[][10], double L[][10], double D[], int n){
    memset(L, 0, sizeof(double) * 10 * 10);
    for(int j = 0; j < n; j++) {
        for (int k = 0; k < j; k++) {
            A[j][j] -= L[j][k] * L[j][k] * D[k];
        }
        D[j] = A[j][j];
        for(int i = j + 1; i < n; i++) {
            L[i][j] = A[i][j];
            for(int k = 0; k < j; k++) {
                L[i][j] -= L[i][k] * L[j][k] * D[k];
            }
            L[i][j] /= D[j];
        }
        L[j][j] = 1;
    }
}


