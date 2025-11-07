// Độ phức tạp: O(n*logn)

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <cmath>
#include <limits>
#include <cfloat>
#include <iomanip>

using namespace std;
pair<int, int> mid;
int quad(pair<int, int> p) {
    if (p.first >= 0 && p.second >= 0)
        return 1;
    if (p.first <= 0 && p.second >= 0)
        return 2;
    if (p.first <= 0 && p.second <= 0)
        return 3;
    return 4;
}
int orientation(pair<int, int> a, pair<int, int> b, pair<int, int> c) {
    int res = (b.second - a.second) * (c.first - b.first) -
              (c.second - b.second) * (b.first - a.first);

    if (res == 0)
        return 0; // Thẳng hàng
    if (res > 0)
        return 1; // cùng chiều kim đồng hồ
    return -1;    // ngược chiều kim đồng hồ
}
bool compare(pair<int, int> p1, pair<int, int> q1) {
    pair<int, int> p = make_pair(p1.first - mid.first, p1.second - mid.second);
    pair<int, int> q = make_pair(q1.first - mid.first, q1.second - mid.second);
    int one = quad(p);
    int two = quad(q);
    if (one != two)
        return (one < two);
    return (p.second * q.first < q.second * p.first);
}
vector<pair<int, int>> merger(vector<pair<int, int>> a, vector<pair<int, int>> b) {
    int n1 = a.size(), n2 = b.size();
    int ia = 0, ib = 0;
    for (int i = 1; i < n1; i++)
        if (a[i].first > a[ia].first)
            ia = i;
    for (int i = 1; i < n2; i++)
        if (b[i].first < b[ib].first)
            ib = i;
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
    vector<pair<int, int>> ret;
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

vector<pair<int, int>> bruteHull(vector<pair<int, int>> a) {
    set<pair<int, int>> s;
    for (int i = 0; i < a.size(); i++) {
        for (int j = i + 1; j < a.size(); j++) {
            int x1 = a[i].first, x2 = a[j].first;
            int y1 = a[i].second, y2 = a[j].second;
            int a1 = y1 - y2;
            int b1 = x2 - x1;
            int c1 = x1 * y2 - y1 * x2;
            int pos = 0, neg = 0;
            for (int k = 0; k < a.size(); k++) {
                if (a1 * a[k].first + b1 * a[k].second + c1 <= 0)
                    neg++;
                if (a1 * a[k].first + b1 * a[k].second + c1 >= 0)
                    pos++;
            }
            if (pos == a.size() || neg == a.size()) {
                s.insert(a[i]);
                s.insert(a[j]);
            }
        }
    }
    vector<pair<int, int>> ret;
    for (auto e : s)
        ret.push_back(e);

    mid = {0, 0};
    int n = ret.size();
    for (int i = 0; i < n; i++) {
        mid.first += ret[i].first;
        mid.second += ret[i].second;
        ret[i].first *= n;
        ret[i].second *= n;
    }
    sort(ret.begin(), ret.end(), compare);
    for (int i = 0; i < n; i++)
        ret[i] = make_pair(ret[i].first / n, ret[i].second / n);

    return ret;
}

vector<pair<int, int>> divide(vector<pair<int, int>> a) {
    if (a.size() <= 5)
        return bruteHull(a);

    vector<pair<int, int>> left, right;
    for (int i = 0; i < a.size() / 2; i++)
        left.push_back(a[i]);
    for (int i = a.size() / 2; i < a.size(); i++)
        right.push_back(a[i]);

    vector<pair<int, int>> left_hull = divide(left);
    vector<pair<int, int>> right_hull = divide(right);

    return merger(left_hull, right_hull);
}

// Tính diện tích bao đống
double polygonArea(const vector<pair<int, int>>& points) {
    double area = 0.0;
    int n = points.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += (points[i].first * points[j].second) - (points[j].first * points[i].second);
    }
    return abs(area) / 2.0;
}

// Tính khoảng cách 2 điểm
double distance(pair<int, int> p1, pair<int, int> p2) {
    return sqrt((p1.first - p2.first) * (p1.first - p2.first) +
                (p1.second - p2.second) * (p1.second - p2.second));
}

// Tính khoảng cách ngắn nhất giữa 2 điểm trong hai đa giác
double shortestDistance(const vector<pair<int, int>>& points) {
    double minDist = DBL_MAX;
    pair<int, int> p1, p2; // Biến để lưu hai điểm có khoảng cách nhỏ nhất
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
    // In tọa độ hai điểm có khoảng cách nhỏ nhất 
    cout << "(" << p1.first << ", " << p1.second << ") va (" 
         << p2.first << ", " << p2.second << ")." << endl;
    return minDist;
}

// Tìm cạnh bao đóng ngắn nhất
double smallestEdge(const vector<pair<int, int>>& points) {
    double minEdge = DBL_MAX;
    int n = points.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        minEdge = min(minEdge, distance(points[i], points[j]));
    }
    return minEdge;
}

// Kiểm tra điểm có nằm trong hàm bao đóng hay không
bool pointInConvexHull(const vector<pair<int, int>>& hull, pair<int, int> point) {
    int n = hull.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        if (orientation(hull[i], hull[j], point) > 0)
            << randomPoint.first << ", " << randomPoint.second
    }
    return true;
}

bool is_In_Vector(const vector<Point>& v, const Point& p) {
int n = v.size();
for (int i = 0; i < n; i++) {
if (v[i] == p) {
return true;
}
}
return false;
}
vector<Point> elements_In_A_And_Not_In_B(const vector<Point>& A, const vector<Point>& B) {
vector<Point> result;
for (const auto& point : A) {
if(!is_In_Vector(B,point)){
result.push_back(point);
}
}
return result;
}
int main(){
vector<Point> v = {{3,5}, {6,8},{9,10}, {11,2},{4,9},{7,8},{9,9},{10,3},{11,15},{34,5},{32,1},{25,10},{18,8},{7,18},{10,14}};
cout << "Original Points: \n";
for (auto p : v) cout << p << "\t";
cout << endl;
vector<Point> output = chansalgorithm(v);
cout << "Convex hull: \n";
for(auto p : output) cout << p << "\t";
cout << "\n";
cout << "Polygon area: " << poly_area(output) << endl;
pair<Point, Point> min_edge = find_min_edge(output);
cout << "Shortest edge in Convex Hull: (" << min_edge.first.x << ", " << min_edge.first.y << ") and (" 
<< min_edge.second.x << ", " << min_edge.second.y << ")\n";
cout << "Length of shortest edge: " << dist(min_edge.first, min_edge.second) << endl;
cout << "Points inside Convex Hull: \n";
vector<Point> points_inside = elements_In_A_And_Not_In_B(v, output);
for (auto p : points_inside) cout << p << "\t";
cout << endl;
return 0;
}


// Sinh n điểm ngẫu nhiên
vector<pair<int, int>> generate_unique_points(int n, int max_range) 
{
	set<pair<double, double>> points_set;
	vector<pair<int, int>> points;
	
	while (points.size() < n) 
	{
		double x = (pow(-1, rand() % 2))*(rand() % max_range);
		double y = (pow(-1, rand() % 2))*(rand() % max_range);
		
		if (points_set.find(make_pair(x, y)) == points_set.end()) {
			points_set.insert(make_pair(x, y));
			pair<int, int> P;
			P.first = x;
			P.second = y;
			points.push_back(P);
		}	
	} 	
	return points;
} 

// Nhập các điểm
vector<pair<int, int>> enterPoints(int n) 
{
	vector<pair<int, int>> points;
	cout << "Nhap cac diem:\n";
	while (points.size() < n) 
	{
        pair<int, int> P;
        cout << "Diem thu " << points.size() + 1 << ":";
        cin >> P.first >> P.second;
        points.push_back(P);
	} 	
	return points;
} 

int main() {


    vector<pair<int, int>> a = {
        {3,5}, {6,8},{9,10}, {11,2},{4,9},{7,8},{9,9},{10,3},
        {11,15},{34,5},{32,1},{25,10},{18,8},{7,18},{10,14}
    };

    int n = a.size();
    // int max_range = 20;
    // srand(time(0));
    // vector<pair<int, int>> a = generate_unique_points(n, max_range);
    // vector<pair<int, int>> a = enterPoints(n);

    
    cout << "Cac diem:\n";
    for(auto p : a){
        cout << "(" << p.first << ", " << p.second << ")" << setw(3);
    }
    cout << endl;
    sort(a.begin(), a.end());
    vector<pair<int, int>> ans = divide(a);
    cout << "Bao Loi la \n";
    for (auto e : ans)
        cout << "(" << e.first << ", " << e.second << ")\t" << endl;
    cout << "Dien tich cua Bao Loi la: " << polygonArea(ans) << endl;
    cout << "2 diem co khoang cach ngan nhat la: " << shortestDistance(a) << endl;
    cout << "Canh be nhat cua bao loi la: " << smallestEdge(ans) << endl;
    cout << "Cac diem nam trong bao loi la:";
    vector
    return 0;
}
