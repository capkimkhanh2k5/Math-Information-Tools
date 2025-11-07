#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

//Hàm mục tiêu
double f(const vector<double>& X)
{
    double x = X[0];
    double y = X[1];
    double z = X[2];
    
    return 0.5 * (x*x + 2*y*y + 3*z*z) 
           + 0.1 * sin(x + y) 
           + 0.1 * cos(y - z);
}

// Đạo hàm số học với step h nhỏ
double numericalDerivative(const vector<double>& X, int i, double (*f)(const vector<double>&), double h = 1e-6)
{
    vector<double> X_plus = X;
    vector<double> X_minus = X;
    X_plus[i] += h;
    X_minus[i] -= h;
    return (f(X_plus) - f(X_minus)) / (2*h);
}

// Gradient Descent n biến
vector<double> gradientDescentND(vector<double> initialX, double learningRate, int maxIterations, double (*f)(const vector<double>&))
{
    int n = initialX.size();
    vector<double> X = initialX;

    for (int iteration = 1; iteration <= maxIterations; ++iteration)
    {
        vector<double> grad(n);
        for (int i = 0; i < n; ++i)
            grad[i] = numericalDerivative(X, i, f);

        vector<double> newX(n);
        for (int i = 0; i < n; ++i)
            newX[i] = X[i] - learningRate * grad[i];

        // In ra mỗi 100 vòng để dễ theo dõi
        if (iteration % 100 == 0)
        {
            cout << "Iteration " << iteration << ": ";
            for (double xi : X) cout << fixed << setprecision(6) << xi << " ";
            cout << ", Function Value = " << f(X) << endl;
        }

        // Kiểm tra hội tụ
        double maxChange = 0;
        for (int i = 0; i < n; ++i)
            maxChange = max(maxChange, fabs(newX[i] - X[i]));

        if (maxChange < 1e-8)  // chuẩn hội tụ nhỏ hơn
            break;

        X = newX;
    }

    return X;
}

int main()
{
    vector<double> initialX = {-1.0, 2.0, 1.0};
    double gamma = 0.01;     //hệ số học
    int maxIterations = 10000;

    cout << "Gradient Descent for n-dimensional function:" << endl;
    vector<double> result = gradientDescentND(initialX, gamma, maxIterations, f);

    cout << "Minimum at: ";
    for (double xi : result) cout << xi << " ";
    cout << "\nFunction value = " << f(result) << endl;

    return 0;
}
