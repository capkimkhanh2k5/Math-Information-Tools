#include <iostream>
#include <fstream>
#include <sstream>
#include <Eigen/Dense>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>

using namespace std;
using namespace Eigen;

MatrixXd matrixPower(const MatrixXd& matrix, int n) {
    if (n < 1)
        throw invalid_argument("Exponent must be positive.");

    MatrixXd result = MatrixXd::Identity(matrix.rows(), matrix.cols());
    MatrixXd base = matrix;
    int exp = n;
    while (exp > 0) {
        if (exp % 2 == 1)
            result *= base;
        base *= base;
        exp /= 2;
    }
    return result;
}

double calculateProbability(const MatrixXd& P, int from, int to, int steps) {
    MatrixXd Pn = matrixPower(P, steps);
    return Pn(to, from);
}

void printMatrix(const MatrixXd& M, const vector<string>& stateNames, int step) {
    cout << "\n Transition matrix after " << step << " step(s):\n\n";
    cout << fixed << setprecision(4);

    cout << setw(12) << left << "";
    for (const auto& name : stateNames)
        cout << setw(12) << left << name;
    cout << "\n";

    for (int i = 0; i < M.rows(); ++i) {
        cout << setw(12) << left << stateNames[i];
        for (int j = 0; j < M.cols(); ++j)
            cout << setw(12) << left << M(i, j);
        cout << "\n";
    }
}

bool loadMatrixFromFile(const string& filename, vector<string>& stateNames, MatrixXd& P) {
    ifstream infile("../Chap5/" + filename);
    if (!infile.is_open()) {
        cerr << " Error: Cannot open file " << filename << endl;
        return false;
    }

    string line;
    vector<vector<double>> rows;
    bool firstLine = true;

    while (getline(infile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        if (firstLine) {
            string word;
            vector<string> tokens;
            while (ss >> word) tokens.push_back(word);

            bool isFirstRowNames = true;
            for (const auto& token : tokens) {
                if (isdigit(token[0]) || token[0] == '-' || token[0] == '.') {
                    isFirstRowNames = false;
                    break;
                }
            }

            if (isFirstRowNames) {
                stateNames = tokens;
                firstLine = false;
                continue;
            } else {
                ss.clear();
                ss.str(line);
            }
        }

        vector<double> values;
        double val;
        while (ss >> val) values.push_back(val);
        rows.push_back(values);
        firstLine = false;
    }

    infile.close();

    if (rows.empty()) {
        cerr << " Error: No numeric data found in " << filename << endl;
        return false;
    }

    int n = rows.size();
    P = MatrixXd(n, n);
    for (int i = 0; i < n; ++i) {
        if ((int)rows[i].size() != n) {
            cerr << " Error: Matrix must be square (" << n << "x" << n << ").\n";
            return false;
        }
        for (int j = 0; j < n; ++j)
            P(i, j) = rows[i][j];
    }

    // Default names if missing
    if (stateNames.empty()) {
        stateNames.resize(n);
        for (int i = 0; i < n; ++i)
            stateNames[i] = "State" + to_string(i);
    }

    return true;
}

int main() {
    vector<string> stateNames;
    MatrixXd P;
    string filename;

    cout << "Enter filename (e.g., matrix.txt): ";
    cin >> filename;

    if (!loadMatrixFromFile(filename, stateNames, P))
        return 1;

    cout << "\n Successfully loaded transition matrix (" << P.rows() << "x" << P.cols() << "):\n";
    cout << fixed << setprecision(6) << P << "\n";

    cout << "\nAvailable States:\n";
    for (int i = 0; i < stateNames.size(); ++i)
        cout << "  " << i << ": " << stateNames[i] << "\n";

    int fromIndex, toIndex, maxSteps;
    cout << "\nEnter the index of the starting state: ";
    cin >> fromIndex;
    cout << "Enter the index of the target state: ";
    cin >> toIndex;
    cout << "Enter the maximum number of steps to calculate: ";
    cin >> maxSteps;

    cout << "\n=== Markov Chain Simulation ===\n";
    cout << "From: " << stateNames[fromIndex] << "  To: " << stateNames[toIndex] << "\n";
    cout << "--------------------------------------------------------\n";

    for (int step = 1; step <= maxSteps; ++step) {
        MatrixXd Pn = matrixPower(P, step);
        printMatrix(Pn, stateNames, step);

        double prob = Pn(fromIndex, toIndex);
        cout << "\n Conclusion after " << step << " step(s): "
            << "Probability of going from [" << stateNames[fromIndex]
            << "] to [" << stateNames[toIndex] << "] = "
            << fixed << setprecision(6) << prob << "\n";
        cout << "--------------------------------------------------------\n";
    }

    cout << "\n Simulation complete.\n";
    return 0;
}
