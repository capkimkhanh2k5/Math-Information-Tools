// Độ phức tạp: O(n*logn)

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <cmath>
#include <limits>
#include <cfloat>
#include <iomanip>
#include <ctime>

using namespace std;

struct Point {
    int x, y;
    
    bool operator < (const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
    
    bool operator == (const Point &p) const {
        return x == p.x && y == p.y;
    }
    
    friend ostream& operator<<(ostream& output, const Point& p) {
        output << "(" << p.x << "," << p.y << ")";
        return output;
    }
};

Point mid;

// Xác định góc phần tư của điểm
int quad(Point p) {
    if (p.x >= 0 && p.y >= 0)
        return 1;
    if (p.x <= 0 && p.y >= 0)
        return 2;
    if (p.x <= 0 && p.y <= 0)
        return 3;
    return 4;
}

// Tính định hướng của 3 điểm
int orientation(Point a, Point b, Point c) {
    int res = (b.y - a.y) * (c.x - b.x) - (c.y - b.y) * (b.x - a.x);

    if (res == 0)
        return 0; // Thẳng hàng
    if (res > 0)
        return 1; // cùng chiều kim đồng hồ
    return -1;    // ngược chiều kim đồng hồ
}

// Hàm so sánh để sắp xếp các điểm theo góc cực
bool compare(Point p1, Point q1) {
    Point p = {p1.x - mid.x, p1.y - mid.y};
    Point q = {q1.x - mid.x, q1.y - mid.y};
    int one = quad(p);
    int two = quad(q);
    if (one != two)
        return (one < two);
    return (p.y * q.x < q.y * p.x);
}

// Hợp nhất 2 bao lồi
vector<Point> merger(vector<Point> a, vector<Point> b) {
    int n1 = a.size(), n2 = b.size();
    int ia = 0, ib = 0;
    
    // Tìm điểm phải nhất của a và trái nhất của b
    for (int i = 1; i < n1; i++)
        if (a[i].x > a[ia].x)
            ia = i;
    for (int i = 1; i < n2; i++)
        if (b[i].x < b[ib].x)
            ib = i;
    
    // Tìm tiếp tuyến trên
    int inda = ia, indb = ib;
    bool done = 0;
    while (!done) {
        done = 1;
        while (orientation(b[indb], a[inda], a[(inda + 1) % n1]) >= 0)
            inda = (inda + 1) % n1;

        while (orientation(a[inda], b[indb], b[(n2 + indb - 1) % n2]) <= 0) {
            indb = (n2 + indb - 1) % n2;
            done = 0;
        }
    }
    int uppera = inda, upperb = indb;
    
    // Tìm tiếp tuyến dưới
    inda = ia, indb = ib;
    done = 0;
    while (!done) {
        done = 1;
        while (orientation(a[inda], b[indb], b[(indb + 1) % n2]) >= 0)
            indb = (indb + 1) % n2;

        while (orientation(b[indb], a[inda], a[(n1 + inda - 1) % n1]) <= 0) {
            inda = (n1 + inda - 1) % n1;
            done = 0;
        }
    }
    int lowera = inda, lowerb = indb;
    
    // Xây dựng bao lồi mới
    vector<Point> ret;
    int ind = uppera;
    ret.push_back(a[uppera]);
    while (ind != lowera) {
        ind = (ind + 1) % n1;
        ret.push_back(a[ind]);
    }
    ind = lowerb;
    ret.push_back(b[lowerb]);
    while (ind != upperb) {
        ind = (ind + 1) % n2;
        ret.push_back(b[ind]);
    }
    return ret;
}

// Tìm bao lồi bằng brute force (cho tập điểm nhỏ)
vector<Point> bruteHull(vector<Point> a) {
    set<Point> s;
    for (int i = 0; i < a.size(); i++) {
        for (int j = i + 1; j < a.size(); j++) {
            int x1 = a[i].x, x2 = a[j].x;
            int y1 = a[i].y, y2 = a[j].y;
            int a1 = y1 - y2;
            int b1 = x2 - x1;
            int c1 = x1 * y2 - y1 * x2;
            int pos = 0, neg = 0;
            for (int k = 0; k < a.size(); k++) {
                if (a1 * a[k].x + b1 * a[k].y + c1 <= 0)
                    neg++;
                if (a1 * a[k].x + b1 * a[k].y + c1 >= 0)
                    pos++;
            }
            if (pos == a.size() || neg == a.size()) {
                s.insert(a[i]);
                s.insert(a[j]);
            }
        }
    }
    
    vector<Point> ret;
    for (auto e : s)
        ret.push_back(e);

    // Sắp xếp các điểm theo góc cực
    mid = {0, 0};
    int n = ret.size();
    for (int i = 0; i < n; i++) {
        mid.x += ret[i].x;
        mid.y += ret[i].y;
        ret[i].x *= n;
        ret[i].y *= n;
    }
    sort(ret.begin(), ret.end(), compare);
    for (int i = 0; i < n; i++)
        ret[i] = {ret[i].x / n, ret[i].y / n};

    return ret;
}

// Thuật toán Divide and Conquer
vector<Point> divide(vector<Point> a) {
    if (a.size() <= 5)
        return bruteHull(a);

    vector<Point> left, right;
    for (int i = 0; i < a.size() / 2; i++)
        left.push_back(a[i]);
    for (int i = a.size() / 2; i < a.size(); i++)
        right.push_back(a[i]);

    vector<Point> left_hull = divide(left);
    vector<Point> right_hull = divide(right);

    return merger(left_hull, right_hull);
}

// Tính diện tích bao lồi
double polygonArea(const vector<Point>& points) {
    double area = 0.0;
    int n = points.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += (points[i].x * points[j].y) - (points[j].x * points[i].y);
    }
    return abs(area) / 2.0;
}

// Tính khoảng cách 2 điểm
double distance(Point p1, Point p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) +
                (p1.y - p2.y) * (p1.y - p2.y));
}

// Tìm 2 điểm có khoảng cách ngắn nhất
double shortestDistance(const vector<Point>& points) {
    double minDist = DBL_MAX;
    Point p1, p2;
    int n = points.size();

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dist = distance(points[i], points[j]);
            if (dist < minDist) {
                minDist = dist;
                p1 = points[i];
                p2 = points[j];
            }
        }
    }
    cout << "(" << p1.x << ", " << p1.y << ") va (" 
        << p2.x << ", " << p2.y << ")" << endl;
    return minDist;
}

// Tìm cạnh ngắn nhất trong bao lồi
double smallestEdge(const vector<Point>& points) {
    double minEdge = DBL_MAX;
    Point p1, p2;
    int n = points.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        double dist = distance(points[i], points[j]);
        if (dist < minEdge) {
            minEdge = dist;
            p1 = points[i];
            p2 = points[j];
        }
    }
    cout << "(" << p1.x << ", " << p1.y << ") va (" 
        << p2.x << ", " << p2.y << ")" << endl;
    return minEdge;
}

// Kiểm tra điểm có nằm trong bao lồi không
bool pointInConvexHull(const vector<Point>& hull, Point point) {
    int n = hull.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        if (orientation(hull[i], hull[j], point) > 0)
            return false;
    }
    return true;
}

// Tìm các điểm nằm trong bao lồi
vector<Point> pointsInsideHull(const vector<Point>& allPoints, 
                                const vector<Point>& hull) {
    vector<Point> insidePoints;
    set<Point> hullSet(hull.begin(), hull.end());
    
    for (const auto& point : allPoints) {
        if (hullSet.find(point) == hullSet.end() && pointInConvexHull(hull, point)) {
            insidePoints.push_back(point);
        }
    }
    return insidePoints;
}

// Sinh n điểm ngẫu nhiên
vector<Point> generate_unique_points(int n, int max_range) {
    srand(time(0));
    set<Point> points_set;
    vector<Point> points;
    
    while (points.size() < n) {
        int x = (rand() % 2 == 0 ? 1 : -1) * (rand() % max_range);
        int y = (rand() % 2 == 0 ? 1 : -1) * (rand() % max_range);
        
        Point p = {x, y};
        if (points_set.find(p) == points_set.end()) {
            points_set.insert(p);
            points.push_back(p);
        }
    }
    return points;
}

// Nhập các điểm từ bàn phím
vector<Point> enterPoints(int n) {
    vector<Point> points;
    cout << "Nhap cac diem:\n";
    for (int i = 0; i < n; i++) {
        Point P;
        cout << "Diem thu " << i + 1 << ": ";
        cin >> P.x >> P.y;
        points.push_back(P);
    }
    return points;
}

int main() {
    vector<Point> a = {
        {3,5}, {6,8}, {9,10}, {11,2}, {4,9}, {7,8}, {9,9}, {10,3},
        {11,15}, {34,5}, {32,1}, {25,10}, {18,8}, {7,18}, {10,14}
    };

    // Hoặc sinh ngẫu nhiên:
    // int n = 15;
    // int max_range = 20;
    // vector<Point> a = generate_unique_points(n, max_range);
    
    // Hoặc nhập từ bàn phím:
    // int n = 10;
    // vector<Point> a = enterPoints(n);

    cout << "Cac diem:\n";
    for(auto p : a) {
        cout << p << setw(3);
    }
    cout << "\n\n";
    
    sort(a.begin(), a.end());
    vector<Point> ans = divide(a);
    
    cout << "Bao Loi la:\n";
    for (auto e : ans)
        cout << e << endl;
    
    cout << "\nDien tich cua Bao Loi la: " << polygonArea(ans) << endl;
    
    cout << "\n2 diem co khoang cach ngan nhat trong tap diem:\n";
    double minDist = shortestDistance(a);
    cout << "Khoang cach: " << minDist << endl;
    
    cout << "\nCanh ngan nhat cua bao loi:\n";
    double minEdge = smallestEdge(ans);
    cout << "Do dai: " << minEdge << endl;
    
    cout << "\nCac diem nam trong bao loi:\n";
    vector<Point> insidePoints = pointsInsideHull(a, ans);
    if (insidePoints.empty()) {
        cout << "Khong co diem nao nam trong bao loi." << endl;
    } else {
        for (auto p : insidePoints) {
            cout << p << setw(3);
        }
        cout << endl;
    }
    

    Point test_point = {10, 10};
    if (pointInConvexHull(ans, test_point)) {
        cout << "\nDiem (" << test_point.x << ", " << test_point.y << ") nam trong bao loi.\n";
    } else {
        cout << "\nDiem (" << test_point.x << ", " << test_point.y << ") nam ngoai bao loi.\n";
    }
    
    return 0;
}