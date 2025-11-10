#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// Hàm f(X) 
double f(const vector<double>& X)
{
    double x = X[0];
    double y = X[1];
    double z = X[2];
    
    return 0.5 * (x*x + 2*y*y + 3*z*z) 
           + 0.1 * sin(x + y) 
           + 0.1 * cos(y - z);
}

// Hàm tính đạo hàm số học 
double numericalDerivative(const vector<double>& X, int i, double (*f)(const vector<double>&), double h = 1e-6)
{
    vector<double> X_plus = X;
    vector<double> X_minus = X;
    X_plus[i] += h;
    X_minus[i] -= h;
    return (f(X_plus) - f(X_minus)) / (2*h);
}

// === Gradient Descent with Momentum ===
vector<double> gradientDescentWithMomentum(
    vector<double> initialX, 
    double learningRate,  // Đây là gamma
    double alpha,         // Đây là hệ số động lượng
    int maxIterations, 
    double (*f)(const vector<double>&)
)
{
    int n = initialX.size();
    vector<double> X = initialX;
    
    // Khởi tạo vector vận tốc (momentum)
    vector<double> v(n, 0.0); 

    for (int iteration = 1; iteration <= maxIterations; ++iteration)
    {
        // 1. Tính Gradient
        vector<double> grad(n);
        for (int i = 0; i < n; ++i)
            grad[i] = numericalDerivative(X, i, f);

        vector<double> newX(n);
        for (int i = 0; i < n; ++i)
        {
            // 2. Cập nhật vận tốc: v_k = alpha * v_(k-1) + gamma * g_k
            v[i] = alpha * v[i] + learningRate * grad[i];
            
            // 3. Cập nhật vị trí: X_(k+1) = X_k - v_k
            newX[i] = X[i] - v[i];
        }

        // In ra mỗi 100 vòng để dễ theo dõi
        if (iteration % 100 == 0)
        {
            cout << "Iteration " << setw(5) << iteration << ": ";
            for (double xi : X) cout << fixed << setprecision(6) << xi << " ";
            cout << ", Function Value = " << f(X) << endl;
        }

        // Kiểm tra hội tụ 
        double maxChange = 0;
        for (int i = 0; i < n; ++i)
            maxChange = max(maxChange, fabs(newX[i] - X[i]));

        if (maxChange < 1e-8)
        {
            cout << "\n Convergence after " << iteration << " loop." << endl;
            break;
        }

        X = newX;
    }

    return X;
}

int main()
{
    vector<double> initialX = {-1.0, 2.0, 1.0};
    double gamma = 0.01;        // Tốc độ học (Learning Rate)
    double alpha = 0.9;         // Hệ số động lượng (Momentum)
    int maxIterations = 10000;

    cout << "Gradient Descent with Momentum for n-dimensional function:" << endl;
    cout << "Initial X: ";
    for (double xi : initialX) cout << xi << " ";
    cout << endl;
    cout << "Learning Rate (gamma) = " << gamma << ", Momentum (alpha) = " << alpha << endl;
    
    vector<double> result = gradientDescentWithMomentum(initialX, gamma, alpha, maxIterations, f);

    cout << "Minimum at: ";
    for (double xi : result) cout << fixed << setprecision(8) << xi << " ";
    cout << "\nFunction value = " << f(result) << endl;

    return 0;
}