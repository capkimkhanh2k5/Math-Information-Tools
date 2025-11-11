#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

//Định nghĩa hàm f(x, y, z)
double f(const vector<double>& X)
{
    double x = X[0];
    double y = X[1];
    double z = X[2];
    
    return 0.5 * (x*x + 2*y*y + 3*z*z) 
           + 0.1 * sin(x + y) 
           + 0.1 * cos(y - z);
}

//Đạo hàm gần đúng (sai phân trung tâm)
double numericalDerivative(const vector<double>& X, int i, double (*f)(const vector<double>&), double h = 1e-6)
{
    vector<double> X_plus = X;
    vector<double> X_minus = X;
    X_plus[i] += h;
    X_minus[i] -= h;
    return (f(X_plus) - f(X_minus)) / (2*h);
}

//Đạo hàm bậc 2 gần đúng (Hessian phần tử chéo)
double numericalSecondDerivative(const vector<double>& X, int i, double (*f)(const vector<double>&), double h = 1e-5)
{
    vector<double> X_plus = X;
    vector<double> X_minus = X;
    X_plus[i] += h;
    X_minus[i] -= h;
    return (f(X_plus) - 2*f(X) + f(X_minus)) / (h*h);
}

// Newton Method ND (Hessian chéo giả định độc lập)
vector<double> newtonND(vector<double> X0, double (*f)(const vector<double>&),
                        int maxIterations = 1000, double eps = 1e-8)
{
    int n = X0.size();

    for (int iter = 1; iter <= maxIterations; ++iter)
    {
        vector<double> grad(n), hessianDiag(n);

        // Gradient và Hessian chéo
        for (int i = 0; i < n; ++i)
        {
            grad[i] = numericalDerivative(X0, i, f);
            hessianDiag[i] = numericalSecondDerivative(X0, i, f);
        }

        vector<double> newX(n);
        bool diverged = false;

        for (int i = 0; i < n; ++i)
        {
            if (fabs(hessianDiag[i]) < 1e-10)
            {
                diverged = true;
                break;
            }
            newX[i] = X0[i] - grad[i] / hessianDiag[i];
        }

        if (diverged)
        {
            cout << "The zero-element Hessian cannot converge at step" << iter << endl;
            break;
        }

        if(iter % 100 == 0){
            cout << "Iteration " << setw(4) << iter << ": ";
            for (double xi : X0) cout << setw(10) << fixed << setprecision(6) << xi << " ";
            cout << " f = " << f(X0) << endl;
        }

        // Kiểm tra hội tụ
        double maxChange = 0;
        for (int i = 0; i < n; ++i)
            maxChange = max(maxChange, fabs(newX[i] - X0[i]));

        if (maxChange < eps)
        {
            cout << "\n Convergence after " << iter << " loop.\n";
            return newX;
        }

        X0 = newX;
    }

    cout << "\n Không hội tụ sau " << maxIterations << " vòng lặp.\n";
    return X0;
}

int main()
{
    vector<double> initialX = {-1.0, 2.0, 1.0};
    cout << "Newton Method for n-dimensional function (numerical derivatives):\n";

    vector<double> result = newtonND(initialX, f, 1000, 1e-8);

    cout << "\n Minimum at: ";
    for (double xi : result) cout << xi << " ";
    cout << "\n Function value = " << f(result) << endl;

    return 0;
}
