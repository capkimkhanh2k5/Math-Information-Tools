#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

// Ham tim nghich dao cua ma tran Q
MatrixXd inverseMatrix(const MatrixXd& matrix) {
    if (matrix.rows() != matrix.cols()) {
        throw std::invalid_argument("Matrix must be square.");
    }
    int n = matrix.rows();
    MatrixXd augmented = MatrixXd::Zero(n, 2 * n);
    augmented.block(0, 0, n, n) = matrix;
    augmented.block(0, n, n, n) = MatrixXd::Identity(n, n);
    
    for (int i = 0; i < n; ++i) {
        if (augmented(i, i) == 0) {
            bool swapped = false;
            for (int j = i + 1; j < n; ++j) {
                if (augmented(j, i) != 0) {
                    augmented.row(i).swap(augmented.row(j));
                    swapped = true;
                    break;
                }
            }
            if (!swapped) {
                throw std::invalid_argument("Matrix is singular and cannot be inverted.");
            }
        }
        double pivot = augmented(i, i);
        augmented.row(i) /= pivot;
        
        for (int j = 0; j < n; ++j) {
            if (j != i) {
                double factor = augmented(j, i);
                augmented.row(j) -= factor * augmented.row(i);
            }
        }
    }
    return augmented.block(0, n, n, n);
}

// Hàm tính toán xác suất sau một số bước nhất định
double calculateProbability(const MatrixXd& P, const VectorXd& initialState, int steps, 
                            const vector<string>& states, int targetState) {
    EigenSolver<MatrixXd> solver(P);
    MatrixXd Q = solver.eigenvectors().real();
    MatrixXd Q_inv = inverseMatrix(Q);
    VectorXd realEigenvalues = solver.eigenvalues().real();
    
    MatrixXd Lambda(realEigenvalues.size(), realEigenvalues.size());
    for(int i = 0; i < realEigenvalues.size(); ++i) {
        for(int j = 0; j < realEigenvalues.size(); ++j) {
            Lambda(i, j) = (i == j) ? realEigenvalues(i) : 0;
        }
    }
    
    MatrixXd Lambda_n = Lambda;
    for (int i = 1; i < steps; ++i) {
        for(int j = 0; j < Lambda_n.rows(); ++j) {
            for(int k = 0; k < Lambda_n.cols(); ++k) {
                Lambda_n(j, k) = Lambda_n(j, k) * Lambda(j, k);
            }
        }
    }
    
    MatrixXd P_n_temp(Q.rows(), Q.cols());
    for(int i = 0; i < Q.rows(); ++i) {
        for(int j = 0; j < Lambda_n.cols(); ++j) {
            P_n_temp(i, j) = 0;
            for(int k = 0; k < Q.cols(); ++k) {
                P_n_temp(i, j) += Q(i, k) * Lambda_n(k, j);
            }
        }
    }
    
    MatrixXd P_n(Q.rows(), Q.cols());
    for(int i = 0; i < P_n.rows(); ++i) {
        for(int j = 0; j < P_n.cols(); ++j) {
            P_n(i, j) = 0;
            for(int k = 0; k < P_n_temp.cols(); ++k) {
                P_n(i, j) += P_n_temp(i, k) * Q_inv(k, j);
            }
        }
    }
    
    // Hiển thị toàn bộ ma trận P^n
    cout << "\nMa tran P^" << steps << ":" << endl;
    cout << fixed << setprecision(6);
    
    // In header (tên các trạng thái đích)
    cout << setw(15) << " ";
    for(int j = 0; j < P_n.cols(); ++j) {
        cout << setw(12) << states[j].substr(0, 10);
    }
    cout << endl;
    cout << string(15 + 12 * P_n.cols(), '-') << endl;
    
    // In từng hàng với tên trạng thái
    for(int i = 0; i < P_n.rows(); ++i) {
        cout << setw(15) << states[i].substr(0, 13) << "|";
        for(int j = 0; j < P_n.cols(); ++j) {
            cout << setw(12) << P_n(i, j);
        }
        cout << endl;
    }
    cout << endl;
    
    // Tìm trạng thái ban đầu
    int initialStateIndex = -1;
    for(int i = 0; i < initialState.size(); ++i) {
        if(initialState(i) == 1.0) {
            initialStateIndex = i;
            break;
        }
    }
    
    return P_n(targetState, initialStateIndex);
}

// Hàm đọc từ file
void inputFromFile(const string& filename, vector<string>& states, MatrixXd& P) {
    ifstream file("../Chap5/" + filename);
    if(!file.is_open()) {
        throw runtime_error("Khong the mo file: " + filename);
    }
    
    int n;
    file >> n;
    file.ignore();
    
    states.resize(n);
    for(int i = 0; i < n; ++i)
        getline(file, states[i]);
    
    P.resize(n, n);
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            file >> P(i, j);
        }
    }
    
    file.close();
    
    cout << "\nDANH SACH TRANG THAI" << endl;
    for(int i = 0; i < n; ++i) {
        cout << i << ". " << states[i] << endl;
    }
    cout << "\nMa tran chuyen trang thai:" << endl;
    cout << P << endl;
}

int main() {
    vector<string> states;
    MatrixXd P;
    VectorXd initialState;
    
    //Nhập bằng file
    string filename;
    cout << "Nhap ten file: ";
    cin >> filename;
    inputFromFile(filename, states, P);
    
    // Inital State
    int initState;
    cout << "\nChon trang thai ban dau (0-" << (states.size()-1) << "): ";
    cin >> initState;
    initialState = VectorXd::Zero(states.size());
    initialState(initState) = 1.0;
    cout << "Trang thai ban dau: " << states[initState] << endl;
    
    //Target State
    int targetState;
    cout << "\nChon trang thai muc tieu (0-" << (states.size()-1) << "): ";
    cin >> targetState;
    
    int steps;
    cout << "Nhap so buoc can tinh: ";
    cin >> steps;

    
    for(int i = 1; i <= steps; ++i) {
        cout << "\n========== BUOC " << i << " ==========" << endl;
        double probability = calculateProbability(P, initialState, i, states, targetState);
        
        int initStateIndex = -1;
        for(int j = 0; j < initialState.size(); ++j) {
            if(initialState(j) == 1.0) {
                initStateIndex = j;
                break;
            }
        }
        
        cout << ">>> Xac suat tu \"" << states[initStateIndex] 
            << "\" den \"" << states[targetState] 
            << "\" sau " << i << " buoc: " << probability << endl;
    }

    return 0;
}