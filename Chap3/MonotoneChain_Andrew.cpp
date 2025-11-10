// Độ phức tạp: O(n*logn)

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <cmath>
#include <limits>
#include <ctime> 
#define Max 1000000000
using namespace std;

struct Point {
    double x, y;
    bool operator < (const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
    bool operator == (const Point &p) const {
        return abs(x - p.x) < 1e-9 && abs(y - p.y) < 1e-9;
    }
};

// Tích có hướng của OA và OB
double cross(const Point &O, const Point &A, const Point &B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Khoảng cách giữa 2 điểm
double distance(const Point &A, const Point &B) {
    return sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
}

// Hàm đệ quy tìm cặp điểm gần nhất (Divide and Conquer)
pair<Point, Point> find_min_dist_recursive(vector<Point> &Px, vector<Point> &Py) {
    int n = Px.size();
    
    // Base case: nếu số điểm <= 3
    if (n <= 3) {
        double min_dist = Max;
        pair<Point, Point> res;
        for (int i = 0; i < n - 1; i++) {
            for (int j = i + 1; j < n; j++) {
                double d = distance(Px[i], Px[j]);
                if (d < min_dist) {
                    min_dist = d;
                    res = {Px[i], Px[j]};
                }
            }
        }
        return res;
    }
    
    // Chia đôi tập điểm
    int mid = n / 2;
    Point mid_point = Px[mid];
    
    vector<Point> Pxl(Px.begin(), Px.begin() + mid);
    vector<Point> Pxr(Px.begin() + mid, Px.end());
    
    vector<Point> Pyl, Pyr;
    for (auto &p : Py) {
        if (p.x <= mid_point.x) 
            Pyl.push_back(p);
        else 
            Pyr.push_back(p);
    }
    
    // Đệ quy tìm ở 2 bên
    auto d_left = find_min_dist_recursive(Pxl, Pyl);
    auto d_right = find_min_dist_recursive(Pxr, Pyr);
    
    double min_dist = min(distance(d_left.first, d_left.second), 
                        distance(d_right.first, d_right.second));
    pair<Point, Point> res = (distance(d_left.first, d_left.second) < 
                            distance(d_right.first, d_right.second)) ? d_left : d_right;
    
    // Kiểm tra các điểm gần đường phân chia
    vector<Point> strip;
    for (auto &p : Py) {
        if (abs(p.x - mid_point.x) < min_dist) 
            strip.push_back(p);
    }
    
    int m = strip.size();
    for (int i = 0; i < m; ++i) {
        for (int j = i + 1; j < m && (strip[j].y - strip[i].y) < min_dist; ++j) {
            double d = distance(strip[i], strip[j]);
            if (d < min_dist) {
                min_dist = d;
                res = {strip[i], strip[j]};
            }
        }
    }
    
    return res;
}

// Tìm 2 điểm có khoảng cách ngắn nhất
pair<Point, Point> find_min_dist(vector<Point> &P) {
    vector<Point> Px = P, Py = P;
    sort(Px.begin(), Px.end(), [](const Point &a, const Point &b) { 
        return a.x < b.x; 
    });
    sort(Py.begin(), Py.end(), [](const Point &a, const Point &b) { 
        return a.y < b.y; 
    });
    
    return find_min_dist_recursive(Px, Py);
}

// Tìm cạnh ngắn nhất trong bao lồi
pair<Point, Point> find_min_edge(const vector<Point> &P) {
    double min_dist = Max;
    pair<Point, Point> res;
    int n = P.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        double d = distance(P[i], P[j]);
        if (d < min_dist) {
            min_dist = d;
            res = {P[i], P[j]};
        }
    }
    return res;
}

// Sinh n điểm ngẫu nhiên
vector<Point> generate_unique_points(int n, int max_range) {
    srand(time(0));
    set<pair<double, double>> points_set;
    vector<Point> points;
    
    while (points.size() < n) {
        double x = (rand() % 2 == 0 ? 1 : -1) * (rand() % max_range);
        double y = (rand() % 2 == 0 ? 1 : -1) * (rand() % max_range);
        
        if (points_set.find({x, y}) == points_set.end()) {
            points_set.insert({x, y});
            points.push_back({x, y});
        }
    }
    return points;
}

// Tìm bao lồi
vector<Point> convex_hull(vector<Point> P) {
    int n = P.size(), k = 0;
    if (n <= 3) return P;
    
    vector<Point> res(2 * n);
    sort(P.begin(), P.end());
    
    // Build lower hull
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(res[k-2], res[k-1], P[i]) <= 0) 
            k--;
        res[k++] = P[i];
    }
    
    // Build upper hull
    for (int i = n - 1, t = k + 1; i > 0; --i) {
        while (k >= t && cross(res[k-2], res[k-1], P[i-1]) <= 0) 
            k--;
        res[k++] = P[i-1];
    }
    
    res.resize(k - 1);
    return res;
}

// Tính diện tích bao lồi
double poly_area(const vector<Point> &P) {
    int n = P.size();
    double area = 0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += P[i].x * P[j].y - P[i].y * P[j].x;
    }
    return abs(area) / 2.0;
}

// Hàm orientation cho kiểu Point
int orientation(const Point &a, const Point &b, const Point &c) {
    double val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    
    if (abs(val) < 1e-9) return 0; // Thẳng hàng
    return (val > 0) ? 1 : -1;     // 1: cùng chiều kim đồng hồ, -1: ngược chiều
}

// Kiểm tra một điểm có nằm trong bao lồi
bool pointInConvexHull(const vector<Point>& hull, const Point& point) {
    int n = hull.size();
    if (n < 3) return false;
    
    // Kiểm tra hướng của bao lồi (ngược chiều kim đồng hồ)
    // Nếu điểm p nằm trong bao lồi, thì với mọi cạnh của bao lồi,
    // điểm p phải nằm cùng phía với tâm bao lồi
    
    int first_orientation = orientation(hull[0], hull[1], point);
    
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int orient = orientation(hull[i], hull[j], point);
        
        // Nếu điểm nằm trên cạnh (thẳng hàng), coi như không nằm trong
        if (orient == 0) return false;
        
        // Tất cả các orientation phải cùng dấu (cùng phía)
        // Với bao lồi ngược chiều kim đồng hồ (Andrew's algorithm), 
        // điểm bên trong sẽ có orientation âm (-1) với tất cả các cạnh
        if (orient > 0) return false;
    }
    
    return true;
}

// Tìm các điểm nằm trong bao lồi
vector<Point> pointsInsideHull(const vector<Point>& allPoints, const vector<Point>& hull) {
    vector<Point> insidePoints;
    set<pair<double, double>> hullSet;
    
    // Lưu tất cả các điểm trên bao lồi
    for (const auto& p : hull) {
        hullSet.insert({p.x, p.y});
    }
    
    // Kiểm tra từng điểm
    for (const auto& point : allPoints) {
        // Nếu điểm không phải đỉnh bao lồi
        if (hullSet.find({point.x, point.y}) == hullSet.end()) {
            // Kiểm tra xem có nằm trong bao lồi không
            if (pointInConvexHull(hull, point)) {
                insidePoints.push_back(point);
            }
        }
    }
    return insidePoints;
}

int main() {
    // Có thể dùng điểm cố định:
    vector<Point> P = {
        {3,5}, {6,8}, {9,10}, {11,2}, {4,9}, {7,8}, {9,9}, {10,3},
        {11,15}, {34,5}, {32,1}, {25,10}, {18,8}, {7,18}, {10,14}
    };

    // Hoặc sinh ngẫu nhiên:
    // int n = 15;
    // int max_range = 20;
    // vector<Point> P = generate_unique_points(n, max_range);
    
    cout << "Danh sach cac diem dau vao:\n";
    for (auto &p : P) 
        cout << "(" << p.x << ", " << p.y << ")\t";
    cout << "\n\n";
    
    // Tìm bao lồi
    vector<Point> res = convex_hull(P);
    cout << "Bao Loi la:\n";
    for (auto &p : res) 
        cout << "(" << p.x << ", " << p.y << ")\n";
    
    // Tính diện tích
    cout << "\nDien tich cua Bao Loi la: " << poly_area(res) << endl;
    
    // Tìm 2 điểm gần nhất trong tập điểm
    pair<Point, Point> res_min_dist = find_min_dist(P);
    cout << "\n2 diem co khoang cach ngan nhat trong tap diem la:\n";
    cout << "(" << res_min_dist.first.x << ", " << res_min_dist.first.y << ") va "
        << "(" << res_min_dist.second.x << ", " << res_min_dist.second.y << ")\n";
    cout << "Voi khoang cach la: " << distance(res_min_dist.first, res_min_dist.second) << endl;
    
    // Tìm cạnh ngắn nhất trong bao lồi
    pair<Point, Point> res_min_edge = find_min_edge(res);
    cout << "\nCanh ngan nhat cua bao loi la:\n";
    cout << "(" << res_min_edge.first.x << ", " << res_min_edge.first.y << ") va "
        << "(" << res_min_edge.second.x << ", " << res_min_edge.second.y << ")\n";
    cout << "Voi do dai: " << distance(res_min_edge.first, res_min_edge.second) << endl;
    
    // Tìm các điểm nằm trong bao lồi
    cout << "\nCac diem nam trong bao loi:\n";
    vector<Point> insidePoints = pointsInsideHull(P, res);
    if (insidePoints.empty()) {
        cout << "Khong co diem nao nam trong bao loi.\n";
    } else {
        for (auto &p : insidePoints) {
            cout << "(" << p.x << ", " << p.y << ")\t";
        }
        cout << endl;
    }
    
    // Kiểm tra một điểm cụ thể
    Point testPoint = {10, 10};
    if (pointInConvexHull(res, testPoint)) {
        cout << "\nDiem (" << testPoint.x << ", " << testPoint.y << ") nam trong bao loi.\n";
    } else {
        cout << "\nDiem (" << testPoint.x << ", " << testPoint.y << ") nam ngoai bao loi.\n";
    }
    
    return 0;
}