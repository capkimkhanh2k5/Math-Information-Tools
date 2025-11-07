#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <functional>

using namespace std;

const double EPS = 1e-5;
const int MAX_ITER = 100;
const double STEP = 1e-6;

// Tinh toan gradient so
pair<double, double> gradient(function<double(double, double)> f, double x1, double x2) {
    double dfdx1 = (f(x1 + STEP, x2) - f(x1 - STEP, x2)) / (2 * STEP);
    double dfdx2 = (f(x1, x2 + STEP) - f(x1, x2 - STEP)) / (2 * STEP);
    return make_pair(dfdx1, dfdx2);
}

// Tim cuc tieu bang gradient descent voi line search
pair<double, double> minimize(function<double(double, double)> f, double x1, double x2) {
    double alpha = 0.1;  // Toc do hoc ban dau
    
    for (int i = 0; i < 1000; i++) {
        pair<double, double> grad = gradient(f, x1, x2);
        double gx1 = grad.first;
        double gx2 = grad.second;
        
        double gnorm = sqrt(gx1 * gx1 + gx2 * gx2);
        if (gnorm < 1e-8) break;  // Sai so cho viec tim cuc tieu ben trong
        
        // Tim kiem tuyen tinh backtracking
        double new_x1, new_x2;
        double f_current = f(x1, x2);
        bool found = false;
        
        for (int ls = 0; ls < 20; ls++) {
            new_x1 = x1 - alpha * gx1;
            new_x2 = x2 - alpha * gx2;
            double f_new = f(new_x1, new_x2);
            
            if (f_new < f_current - 0.001 * alpha * gnorm * gnorm) {
                found = true;
                break;
            }
            alpha *= 0.5;
        }
        
        if (!found) {
            alpha = 0.01;  // Reset alpha neu tim kiem that bai
            continue;
        }
        
        x1 = new_x1;
        x2 = new_x2;
        
        // Tang alpha neu co tien trien tot
        alpha = min(alpha * 1.1, 1.0);
    }
    return make_pair(x1, x2);
}

// Ham muc tieu f(x1, x2)
double f(double x1, double x2) {
    return 6 * x1 * x1 + 4 * x1 * x2 + 3 * x2 * x2;
}

// Rang buoc dang thuc h_i(x)
vector<function<double(double, double)>> hfuncs = {
    [](double x1, double x2) { return x1 + x2 - 5; }
};

// Rang buoc bat dang thuc g_i(x)
vector<function<double(double, double)>> gfuncs = {
    // Chưa có
};

// Phuong phap nhan tu Lagrange
int main() {
    cout << fixed << setprecision(5);

    double x1 = 0.0, x2 = 0.0;
    double lambda = 0.0;
    double rk = 1.0;

    cout << setw(10) << "lambda"
         << setw(10) << "r_i"
         << setw(15) << "x1*(i)"
         << setw(15) << "x2*(i)"
         << setw(15) << "h(x)" << endl;
    cout << string(65, '-') << endl;

    for (int iter = 0; iter < MAX_ITER; iter++) {
        // Xay dung ham Lagrangian mo rong
        function<double(double, double)> L = [&](double a, double b) {
            double val = f(a, b);
            for (size_t i = 0; i < hfuncs.size(); ++i) {
                double h = hfuncs[i](a, b);
                val += lambda * h + (rk / 2.0) * h * h;
            }
            for (size_t j = 0; j < gfuncs.size(); ++j) {
                double g = gfuncs[j](a, b);
                val += (rk / 2.0) * pow(max(0.0, g), 2);
            }
            return val;
        };

        // Tim cuc tieu L theo x1, x2
        pair<double, double> newx = minimize(L, x1, x2);
        double new_x1 = newx.first;
        double new_x2 = newx.second;

        // Tinh h(X)
        double h_val = hfuncs[0](new_x1, new_x2);
        double f_val = f(new_x1, new_x2);

        cout << setw(10) << lambda
             << setw(10) << rk
             << setw(15) << new_x1
             << setw(15) << new_x2
             << setw(15) << h_val << endl;

        // Cap nhat lambda
        lambda += rk * h_val;
        
        x1 = new_x1;
        x2 = new_x2;
    }

    cout << "\nKhong hoi tu sau " << MAX_ITER << " vong lap.\n";
    return 0;
}