#include<iostream>
#include<math.h>
#include<cstring>
#include <iomanip>

using namespace std;


void inputMatrix(double A[][10], int n, int m);
void outputResult(double A[][10], int n);
bool checkMatrixCholesky(double A[][10], int n, int m);
bool SymmetricalMatrix(double A[][10], int n);
float Det(double A[][10], int n);
bool checkPositive(double A[][10], int n);
void Cholesky_Decomposition(double A[][10], double L[][10], int n);

int main(){
    double A[10][10];
    double L[10][10];
    memset(L, 0, sizeof(L));
    int n, m;
    cout << "Nhap kich thuoc ma tran (n x m): "; cin >> n >> m;
    inputMatrix(A, n, m);
    if(!checkMatrixCholesky(A, n, m)){
        cout << "Ma tran khong the phan ra\n";
        return 0;
    } else {
        Cholesky_Decomposition(A, L, n);
        outputResult(L, n);
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

void Cholesky_Decomposition(double A[][10], double L[][10], int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j <= i; j++){
            double sum = 0;
            if(i == j){
                for(int k = 0; k < j; k++){
                    sum += pow(L[j][k], 2);
                }
                L[j][j] = sqrt(A[j][i] - sum);
            } else {
                for(int k = 0; k < j; k++){
                    sum += L[i][k]*L[j][k];
                }
                L[i][j] = (1/L[j][j])*(A[i][j] - sum);
            }
        }
    }
}

void outputResult(double A[][10], int n) {
    cout << "Matrix L : \n";
    cout << fixed << setprecision(3); 
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << A[i][j] << " ";
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
    if(!SymmetricalMatrix(A, n)){
        return false;
    }
    if(!checkPositive(A, n)){
        return false;
    }
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

float Det(double A[][10], int n)
{
    double temp[10][10];
    memcpy(temp, A, sizeof(double) * 10 * 10);
    float res = 1;
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (temp[i][i] == 0)
        {
            int check = 0;
            for (int j = i + 1; j < n; j++)
            {
                if (temp[j][i] != 0)
                {
                    for (int k = 0; k < n; k++)
                    {
                        swap(temp[j][k], temp[j][k]);
                    }
                    res *= -1;
                    check++;
                    break;
                }
            }
            if (check == 0)
                return 0;
        }
        for (int j = i + 1; j <= n; j++)
        {
            float h = -temp[j][i] / temp[i][i];
            for (int k = i; k < n; k++)
                temp[j][k] += h * temp[i][k];
        }
        res *= temp[i][i];
    }
    return res * temp[n - 1][n - 1];
}

bool checkPositive(double A[][10], int n)
{
    double subMatrix[10][10];
    for(int k = 1; k <= n; k++){
        for(int i = 0; i < k; i++){
            for(int j = 0; j < k; j++){
                subMatrix[i][j] = A[i][j];
            }
        }
        if(Det(subMatrix, k) <= 0) {
            return false;
        }
    }
    return true;
}
