// Độ phức tạp: O(n*h) h là số các điểm trong bao đóng

#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;

    bool operator <(const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
};
int orientation(Point p, Point q, Point r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0; // thẳng hàng
    return (val > 0) ? 1 : 2; // cùng / ngược chiều kim đồng hồ
}

// TÌm bao lồi
vector<Point> convexHull(Point points[], int n) {
    // There must be at least 3 points
    if (n < 3) return {};

    // Initialize Result
    vector<Point> hull;

    // Find the leftmost point
    int l = 0;
    for (int i = 1; i < n; i++)
        if (points[i].x < points[l].x)
            l = i;

    // Start from leftmost point, keep moving counterclockwise
    // until reach the start point again.
    int p = l, q;
    do {
        // Add current point to result
        hull.push_back(points[p]);

        // Search for a point 'q' such that orientation(p, q, x)
        // is counterclockwise for all points 'x'. The idea is to
        // keep track of last visited most counterclockwise point in q.
        q = (p + 1) % n;
        for (int i = 0; i < n; i++) {
            if (orientation(points[p], points[i], points[q]) == 2)
                q = i;
        }

        // Now q is the most counterclockwise with respect to p
        p = q;

    } while (p != l); // While we don't come to the first point

    return hull;
}

// Khoảng cách giữa 2 điểm
double distance(const Point &A, const Point &B) {
    return sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
}

// Tìm cạnh ngắn nhất
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

// Tìm 2 điểm có khoảng cách ngắn nhất
pair<Point, Point> find_closest_points(Point points[], int n) {
    double min_dist = DBL_MAX;
    pair<Point, Point> closest_pair;
    
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

// Kiểm tra một điểm có nằm trong bao lồi
bool is_inside_hull(const Point &pt, const vector<Point> &hull) {
    int n = hull.size();
    if (n < 3) return false;

    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        if (orientation(hull[i], hull[j], pt) != 2) // not counterclockwise
            return false;
    }
    return true;
}

// Sinh n điểm ngẫu nhiên
vector<Point> generate_random_points(int n, int max_range) {
    srand(time(0)); // Seed cho random
    set<pair<double, double>> points_set;
	vector<Point> points;
	
	while (points.size() < n) 
	{
		double x = (pow(-1, rand() % 2))*(rand() % max_range);
		double y = (pow(-1, rand() % 2))*(rand() % max_range);
		
		if (points_set.find(make_pair(x, y)) == points_set.end()) {
			points_set.insert(make_pair(x, y));
			Point P;
			P.x = x;
			P.y = y;
			points.push_back(P);
		}	
	} 	
	return points;
}

int main() {

    

    int n = 15; // Số lượng điểm ngẫu nhiên
    int max_range = 10;
    vector<Point> points = generate_random_points(n, max_range); // Danh sach cac diem dau vao

    

    cout << "Danh sach cac diem dau vao: \n";
    for (int i = 0; i < n; i++) {
        cout << "(" << points[i].x << ", " << points[i].y << ") ";
    }
	cout << endl;

    // Find convex hull
    vector<Point> hull = convexHull(points, n);

    cout << "\nBao Loi la \n";
    for (const auto &p : hull)
        cout << "(" << p.x << ", " << p.y << ")\n";

    // Calculate area of convex hull
    double area = poly_area(hull);
    cout << "\nDien tich cua Bao Loi la: "  << area << endl;

    // Find shortest edge in convex hull
    pair<Point, Point> min_edge = find_min_edge(hull);
    cout << "Canh be nhat cua bao loi la (" << min_edge.first.x << ", " << min_edge.first.y << ") va ("
         << min_edge.second.x << ", " << min_edge.second.y << ")\n";
    cout << "Khoang cach ngan nhat: " << distance(min_edge.first, min_edge.second) << endl;
     // Find two points with the shortest distance in the original set
    pair<Point, Point> closest_pair = find_closest_points(points, n);
    cout << "\n2 diem co khoang cach ngan nhat la (" << closest_pair.first.x << ", " << closest_pair.first.y << ") va ("
    << closest_pair.second.x << ", " << closest_pair.second.y << ")\n";
    cout << "Voi khoang cach la: " << distance(closest_pair.first, closest_pair.second) << endl;
    // Check if a random point is inside the convex hull
    Point test_point = {1, 2}; // example point
    if (is_inside_hull(test_point, hull)) {
        cout << "\nDiem (" << test_point.x << ", " << test_point.y << ") nam trong bao dong.\n";
    } else {
        cout << "\nDiem (" << test_point.x << ", " << test_point.y << ") nam ngoai bao dong.\n";
    }
    return 0;
}
