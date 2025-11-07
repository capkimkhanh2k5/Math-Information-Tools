#include <Eigen/Dense>
#include <iostream>

// g++ -I C:/Users/Loc/eigen "SVD(nxn).cpp" -o "SVD(nxn)"
#include <math.h>

using namespace std;
using namespace Eigen;

MatrixXd U, D, V;

void inputMatrix(MatrixXd &A, int n) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cin >> A(i, j);
        }
    }
}

void checkResults(MatrixXd A, MatrixXd B, int n) {

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(abs(A(i, j) - B(i, j)) > 1e-5) {
                cout << "Loi: A(" << i << ", " << j << ") = " << A(i, j) << ", B(" << i << ", " << j << ") = " << B(i, j) << endl;
                return;
            }
        }
    }
    cout << "Chinh xac\n" << B << endl;
}

void SVD(MatrixXd &A, int n) {
    MatrixXd AT = A.transpose(); // Ma trận chuyển vị của A

    MatrixXd S = AT * A;

    // Tính giá trị riêng và vector trị riêng của S
    EigenSolver<MatrixXd> es(S);

    // Lưu giá trị riêng vào vector
    VectorXd GTR = es.eigenvalues().real();

    // Ma trận V lưu các VTR của ma trận S
    V = es.eigenvectors().real(); 

    // Sắp xếp các GTR giảm dần đông thời sắp xếp các VTR trong V
    for(int i = 0; i < n; ++i) {
        for(int j = i + 1; j < n; ++j) {
            if (GTR(j) > GTR(i)) {
                // Hoán đổi giá trị riêng
                swap(GTR(i), GTR(j));
                // Hoán đổi các VTR tương ứng
                V.col(i).swap(V.col(j));
            }
        }
    }

    // Tính Xichma và đưa vào đường chéo của ma trận D
    D.resize(n, n); // Thay đổi kích thước ma trận
    D.setZero();
    for(int i = 0; i < n; i++) 
        D(i, i) = sqrt(GTR(i));

    // Tính các phần tử của mà trận U
    // Ma trận D là ma trận đường chéo nên nghịch đảo ma trận bằng nghịch đảo các từng thành phần trên đường chéo
    U = A * V * D.inverse();

    // In ra ma trận U, D, V
    cout << "Ma tran U:" << endl << U << endl;
    cout << "Ma tran D:" << endl << D << endl;
    cout << "Ma tran VT:" << endl << V.transpose() << endl;
    MatrixXd B = U * D * V.transpose();
    cout << "Kiem tra ket qua\n";
    checkResults(A, B, n); // Kiểm tra kết quả
}

int main() {
    int n;
    cout << "Nhap kich thuoc ma tran (nxn, n <=4): ";
    cin >> n;
    MatrixXd A(n, n);
    cout << "Nhap gia tri cua ma tran:\n";
    inputMatrix(A, n);
    SVD(A, n);
    
    return 0;
}
