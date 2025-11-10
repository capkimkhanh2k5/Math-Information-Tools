// Độ phức tạp: O(n*logn)

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <cmath>
#include <limits>
#include <ctime>
#include <cfloat>
#include <stack>
using namespace std;

struct Point {
    int x, y;
    
    bool operator < (const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
    
    bool operator == (const Point &p) const {
        return x == p.x && y == p.y;
    }
};

Point p0; // Điểm có tọa độ y nhỏ nhất (hoặc x nhỏ nhất nếu y bằng nhau)

// Tính khoảng cách giữa 2 điểm
double distance(const Point &p1, const Point &p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

// Tính định hướng của 3 điểm (p, q, r)
int orientation(const Point &p, const Point &q, const Point &r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    
    if (val == 0) return 0;  // thẳng hàng
    return (val > 0) ? 2 : 1; // 2: cùng chiều, 1: ngược chiều kim đồng hồ
}

// Hàm so sánh để sắp xếp các điểm theo góc với p0
bool compare(const Point &p1, const Point &p2) {
    int orient = orientation(p0, p1, p2);
    
    if (orient == 0) {
        // Nếu thẳng hàng, điểm gần p0 hơn đứng trước
        return distance(p0, p1) < distance(p0, p2);
    }
    
    // Điểm có góc nhỏ hơn (ngược chiều kim đồng hồ) đứng trước
    return (orient == 1);
}

// Tìm điểm thứ 2 từ đỉnh stack
Point nextToTop(stack<Point> &S) {
    Point p = S.top();
    S.pop();
    Point res = S.top();
    S.push(p);
    return res;
}

// Thuật toán Graham Scan tìm bao lồi
vector<Point> grahamScan(vector<Point> points) {
    int n = points.size();
    if (n < 3) return {};
    
    // Tìm điểm có y nhỏ nhất (hoặc x nhỏ nhất nếu y bằng nhau)
    int min_idx = 0;
    int min_y = points[0].y;
    for (int i = 1; i < n; i++) {
        if (points[i].y < min_y || (points[i].y == min_y && points[i].x < points[min_idx].x)) {
            min_y = points[i].y;
            min_idx = i;
        }
    }
    
    // Đặt điểm có y nhỏ nhất vào vị trí đầu tiên
    swap(points[0], points[min_idx]);
    p0 = points[0];
    
    // Sắp xếp các điểm còn lại theo góc cực với p0
    sort(points.begin() + 1, points.end(), compare);
    
    // Loại bỏ các điểm trùng nhau về góc (giữ điểm xa nhất)
    int m = 1;
    for (int i = 1; i < n; i++) {
        while (i < n - 1 && orientation(p0, points[i], points[i + 1]) == 0)
            i++;
        points[m] = points[i];
        m++;
    }
    
    if (m < 3) return {};
    
    // Tạo bao lồi bằng stack
    stack<Point> S;
    S.push(points[0]);
    S.push(points[1]);
    S.push(points[2]);
    
    for (int i = 3; i < m; i++) {
        // Loại bỏ các điểm không tạo thành rẽ trái
        while (S.size() > 1 && orientation(nextToTop(S), S.top(), points[i]) != 1)
            S.pop();
        S.push(points[i]);
    }
    
    // Chuyển từ stack sang vector
    vector<Point> hull;
    while (!S.empty()) {
        hull.push_back(S.top());
        S.pop();
    }
    
    reverse(hull.begin(), hull.end());
    return hull;
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

// Tìm cạnh ngắn nhất trong bao lồi
pair<Point, Point> find_min_edge(const vector<Point> &P) {
    double min_dist = DBL_MAX;
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

// Tìm 2 điểm có khoảng cách ngắn nhất trong tập điểm
pair<Point, Point> find_closest_points(const vector<Point> &points) {
    double min_dist = DBL_MAX;
    pair<Point, Point> closest_pair;
    int n = points.size();
    
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double d = distance(points[i], points[j]);
            if (d < min_dist) {
                min_dist = d;
                closest_pair = {points[i], points[j]};
            }
        }
    }
    
    return closest_pair;
}

// Kiểm tra điểm có nằm trong bao lồi không
bool is_inside_hull(const Point &pt, const vector<Point> &hull) {
    int n = hull.size();
    if (n < 3) return false;
    
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        if (orientation(hull[i], hull[j], pt) != 1)
            return false;
    }
    return true;
}

// Tìm các điểm nằm trong bao lồi
vector<Point> points_inside_hull(const vector<Point> &allPoints, const vector<Point> &hull) {
    vector<Point> insidePoints;
    set<pair<int, int>> hullSet;
    
    for (const auto &p : hull) {
        hullSet.insert({p.x, p.y});
    }
    
    for (const auto &point : allPoints) {
        if (hullSet.find({point.x, point.y}) == hullSet.end() && is_inside_hull(point, hull)) {
            insidePoints.push_back(point);
        }
    }
    return insidePoints;
}

// Sinh n điểm ngẫu nhiên
vector<Point> generate_random_points(int n, int max_range) {
    srand(time(0));
    set<pair<int, int>> points_set;
    vector<Point> points;
    
    while (points.size() < n) {
        int x = (rand() % 2 == 0 ? 1 : -1) * (rand() % max_range);
        int y = (rand() % 2 == 0 ? 1 : -1) * (rand() % max_range);
        
        if (points_set.find({x, y}) == points_set.end()) {
            points_set.insert({x, y});
            points.push_back({x, y});
        }
    }
    return points;
}

int main() {
    // Dữ liệu mẫu
    vector<Point> points = {
        {3,5}, {6,8}, {9,10}, {11,2}, {4,9}, {7,8}, {9,9}, {10,3},
        {11,15}, {34,5}, {32,1}, {25,10}, {18,8}, {7,18}, {10,14}
    };
    
    // Hoặc sinh ngẫu nhiên:
    // int n = 15;
    // int max_range = 20;
    // vector<Point> points = generate_random_points(n, max_range);
    
    int n = points.size();
    
    cout << "Cac diem dau vao:\n";
    for (const auto &p : points) {
        cout << "(" << p.x << ", " << p.y << ") ";
    }
    cout << "\n\n";
    
    // Tìm bao lồi bằng Graham Scan
    vector<Point> hull = grahamScan(points);
    
    cout << "Bao Loi la:\n";
    for (const auto &p : hull) {
        cout << "(" << p.x << ", " << p.y << ")\n";
    }
    
    // Tính diện tích bao lồi
    double area = poly_area(hull);
    cout << "\nDien tich cua Bao Loi la: " << area << endl;
    
    // Tìm 2 điểm có khoảng cách ngắn nhất trong tập điểm
    pair<Point, Point> closest_pair = find_closest_points(points);
    cout << "\n2 diem co khoang cach ngan nhat trong tap diem:\n";
    cout << "(" << closest_pair.first.x << ", " << closest_pair.first.y << ") va ("
        << closest_pair.second.x << ", " << closest_pair.second.y << ")\n";
    cout << "Khoang cach: " << distance(closest_pair.first, closest_pair.second) << endl;
    
    // Tìm cạnh ngắn nhất trong bao lồi
    pair<Point, Point> min_edge = find_min_edge(hull);
    cout << "\nCanh ngan nhat cua bao loi:\n";
    cout << "(" << min_edge.first.x << ", " << min_edge.first.y << ") va ("
        << min_edge.second.x << ", " << min_edge.second.y << ")\n";
    cout << "Do dai: " << distance(min_edge.first, min_edge.second) << endl;
    
    // Tìm các điểm nằm trong bao lồi
    cout << "\nCac diem nam trong bao loi:\n";
    vector<Point> insidePoints = points_inside_hull(points, hull);
    if (insidePoints.empty()) {
        cout << "Khong co diem nao nam trong bao loi." << endl;
    } else {
        for (const auto &p : insidePoints) {
            cout << "(" << p.x << ", " << p.y << ") ";
        }
        cout << endl;
    }

    Point test_point = {10, 10};
    if (is_inside_hull(test_point, hull)) {
        cout << "\nDiem (" << test_point.x << ", " << test_point.y << ") nam trong bao loi.\n";
    } else {
        cout << "\nDiem (" << test_point.x << ", " << test_point.y << ") nam ngoai bao loi.\n";
    }
    
    return 0;
}