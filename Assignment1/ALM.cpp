#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <functional>
using namespace std;

const double EPS = 1e-6;
const int MAX_OUTER = 50;
const int MAX_INNER = 1000;
const double STEP = 1e-6;

//Hàm mục tiêu
double f(const vector<double>& X)
{
    double x = X[0];
    double y = X[1];
    double z = X[2];
    
    return x * x + y * y + z * z;
}

//Hàm ràng buộc
vector<function<double(const vector<double>&)>> hfuncs = {
    [](const vector<double>& X) { return X[0] + X[1] + X[2] - 1; }
};

//Đạo hàm
vector<double> numericalDerivative(function<double(const vector<double>&)> func,
                                const vector<double>& X, double step = STEP)
{
    int n = X.size();
    vector<double> grad(n);
    for (int i = 0; i < n; ++i)
    {
        vector<double> X_plus = X, X_minus = X;
        X_plus[i] += step;
        X_minus[i] -= step;
        grad[i] = (func(X_plus) - func(X_minus)) / (2 * step);
    }
    return grad;
}

//gradientMoment
vector<double> gradientMoment(function<double(const vector<double>&)> L,
                        vector<double> X)
{
    double alpha = 0.1;
    int n = X.size();

    for (int iter = 0; iter < MAX_INNER; ++iter)
    {
        vector<double> grad = numericalDerivative(L, X);
        double gnorm = 0.0;
        for (double g : grad) gnorm += g * g;
        gnorm = sqrt(gnorm);
        if (gnorm < 1e-8) break;

        double f_current = L(X);
        bool found = false;
        vector<double> newX = X;

        // Backtracking line search
        for (int ls = 0; ls < 20; ++ls)
        {
            for (int i = 0; i < n; ++i)
                newX[i] = X[i] - alpha * grad[i];

            double f_new = L(newX);
            if (f_new < f_current - 1e-4 * alpha * gnorm * gnorm)
            {
                found = true;
                break;
            }
            alpha *= 0.5;
        }

        if (!found) alpha = 0.01;
        else alpha = min(alpha * 1.1, 1.0);

        X = newX;
    }
    return X;
}

int main()
{
    cout << fixed << setprecision(6);

    vector<double> X = {1.0, 2.0, 1.5};  // initial guess (x, y, z)
    double lambda = 0.0;
    double r = 1.0;

    cout << setw(8) << "Iter"
         << setw(12) << "lambda"
         << setw(10) << "r"
         << setw(15) << "x"
         << setw(15) << "y"
         << setw(15) << "z"
         << setw(15) << "h(x)" << endl;
    cout << string(85, '-') << endl;

    for (int outer = 1; outer <= MAX_OUTER; ++outer)
    {
        // Build augmented Lagrangian L(X)
        function<double(const vector<double>&)> L = [&](const vector<double>& Xv) {
            double val = f(Xv);
            for (auto& h : hfuncs)
            {
                double hv = h(Xv);
                val += lambda * hv + 0.5 * r * hv * hv;
            }
            return val;
        };

        // Minimize L(X, λ, r)
        vector<double> newX = gradientMoment(L, X);
        double h_val = hfuncs[0](newX);
        double f_val = f(newX);

        cout << setw(8) << outer
             << setw(12) << lambda
             << setw(10) << r
             << setw(15) << newX[0]
             << setw(15) << newX[1]
             << setw(15) << newX[2]
             << setw(15) << h_val << endl;

        // Update λ and r
        lambda += r * h_val;
        if (fabs(h_val) > 1e-3)
            r *= 1.5;

        // Check convergence
        if (fabs(h_val) < EPS)
        {
            cout << "\n Converged after " << outer << " iterations.\n";
            cout << "Minimum at: (" << newX[0] << ", " << newX[1] << ", " << newX[2] << ")\n";
            cout << "f(x) = " << f_val << endl;
            return 0;
        }

        X = newX;
    }

    cout << "\n Did not converge after " << MAX_OUTER << " iterations.\n";
    return 0;
}
