// Độ phức tạp: O(n*logh)

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm> 
#include <utility>   
#include <set>
#include <cmath>
#include <limits>
#include <ctime> 
#define RIGHT_TURN -1  
#define LEFT_TURN 1  
#define COLLINEAR 0  
using namespace std;

class Point{
    public:
    int x;
    int y;
    Point (int newx=0,int newy=0){
        x=newx;
        y=newy;
    }
    friend bool operator== (const Point& p1,const Point& p2){
        return (p1.x==p2.x && p1.y==p2.y);
    }
    friend bool operator!= (const Point& p1,const Point& p2){
        return (!(p1.x==p2.x && p1.y==p2.y));
    }
    friend ostream& operator<<(ostream& output,const Point& p){
        output<<"("<<p.x<<","<<p.y<<")";
        return output;
    }
} p0;

// Hàm xác định hướng của 3 điểm (p, q, r)
int orientation(Point p, Point q, Point r){
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0)? -1: 1;
}

// Hàm tính khoảng cách giữa 2 điểm
double dist(Point p1, Point p2){
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

// Hàm tìm cạnh nhỏ nhất trong một đa giác
pair<Point, Point> find_min_edge(vector<Point> v){
    double min_dist = numeric_limits<double>::max();
    int n = v.size();
    pair<Point, Point> res;
    for (int i = 0; i < n; i++){
        int j = (i + 1) % n;
        double distance = dist(v[i], v[j]);
        if(distance < min_dist){
            min_dist = distance;
            res = {v[i], v[j]};
        }
    }
    return res;
}

// Hàm tìm cặp điểm gần nhất trong một tập hợp các điểm
pair<Point, Point> find_closet_points(vector<Point> v, int n){
    double min_dist = numeric_limits<double>::max();
    pair<Point, Point> closet_pair;
    for (int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            double distance = dist(v[i], v[j]);
            if(distance < min_dist){
                min_dist = distance;
                closet_pair = {v[i], v[j]};
            }
        }
    }
    return closet_pair;
}

// Hàm so sánh cho qsort để sắp xếp các điểm dựa trên góc cực với một điểm toàn cục p0
int compare(const void *vp1, const void *vp2){
    Point *p1 = (Point *)vp1;
    Point *p2 = (Point *)vp2;
    int orient = orientation(p0, *p1, *p2);
    if (orient == 0)
        return (dist(p0, *p2) >= dist(p0, *p1))? -1 : 1;
    return (orient == 1)? -1: 1;
}

// Hàm tìm tiếp tuyến từ một điểm đến một bao lồi
int tangent(vector<Point> v,Point p){
    int l=0;
    int r= v.size();
    int l_before = orientation(p, v[0], v[v.size()-1]);
    int l_after = orientation(p, v[0], v[(l + 1) % v.size()]);
    while (l < r){
        int c = ((l + r)>>1);
        int c_before = orientation(p, v[c], v[(c - 1) % v.size()]);
        int c_after = orientation(p, v[c], v[(c + 1) % v.size()]);
        int c_side = orientation(p, v[l], v[c]);
        if ((c_before != RIGHT_TURN) && (c_after != RIGHT_TURN))
        return c;
        else if ((c_side == LEFT_TURN) && (l_after == RIGHT_TURN || l_before == l_after) || (c_side == RIGHT_TURN && c_before == RIGHT_TURN))
        r = c;
        else
        l = c + 1 ;
        l_before = -c_after; 
        l_after = orientation(p, v[l], v[(l + 1) % v.size()]);
    }
    return l;
}

// Hàm tìm điểm có tọa độ y nhỏ nhất trong tất cả các bao lồi
pair<int,int> extreme_hullpt_pair(vector<vector<Point> >& hulls){
    int h= 0,p= 0;
    for (int i=0; i<hulls.size(); ++i){
        int min_index=0, min_y = hulls[i][0].y;
        for(int j=1; j< hulls[i].size(); ++j){
        if(hulls[i][j].y < min_y){
            min_y=hulls[i][j].y;
            min_index=j;
        }
        }
        if(hulls[i][min_index].y < hulls[h][p].y){
        h=i;
        p=min_index;
        }   
    }
    return make_pair(h,p);
}

// Hàm tìm điểm tiếp theo trong bao lồi của một tập hợp các bao lồi
pair<int,int> next_hullpt_pair(vector<vector<Point> >& hulls, pair<int,int> lPoint){
    Point p = hulls[lPoint.first][lPoint.second];
    pair<int,int> next = make_pair(lPoint.first, (lPoint.second + 1) % hulls[lPoint.first].size());
    for (int h=0; h< hulls.size(); h++){
        if(h != lPoint.first){
        int s= tangent(hulls[h],p);
        Point q= hulls[next.first][next.second];
        Point r= hulls[h][s];
        int t= orientation(p,q,r);
        if( t== RIGHT_TURN || (t==COLLINEAR) && dist(p,r)>dist(p,q))
            next = make_pair(h,s);
        }
    }
    return next;
}    

// Hàm trợ giúp cho Graham Scan, đảm bảo các điểm tạo thành một rẽ trái
vector<Point> keep_left (vector<Point>& v,Point p){
    while(v.size()>1 && orientation(v[v.size()-2],v[v.size()-1],p) != LEFT_TURN)
        v.pop_back();
    if(!v.size() || v[v.size()-1] != p)
        v.push_back(p);
    return v;
}

// Hàm tính bao lồi của một tập hợp các điểm bằng thuật toán Graham Scan
vector<Point> GrahamScan(vector<Point>& Points) {
    if (Points.size() <= 1) 
        return Points;
    qsort(&Points[0], Points.size(), sizeof(Point), compare);
    vector<Point> lower_hull;
    for (const auto& p : Points)
        lower_hull = keep_left(lower_hull, p);
    reverse(Points.begin(), Points.end());
    vector<Point> upper_hull;
    for (const auto& p : Points)
        upper_hull = keep_left(upper_hull, p);
    lower_hull.insert(lower_hull.end(), upper_hull.begin() + 1, upper_hull.end());
    return lower_hull;
}

// Hàm tính bao lồi của một tập hợp các điểm bằng thuật toán Chan
vector<Point> chansalgorithm(vector<Point> v) {
    for(int t = 0; t < v.size(); ++t) {
        for(int m = 1; m < (1 << (1 << t)); ++m) {
            vector<vector<Point>> hulls;
            for(int i = 0; i < v.size(); i += m) {
                vector<Point> chunk;
                if(v.begin() + i + m <= v.end())
                    chunk.assign(v.begin() + i, v.begin() + i + m);
                else
                    chunk.assign(v.begin() + i, v.end());
                hulls.push_back(GrahamScan(chunk)); 
            }
            vector<pair<int, int>> hull;
            hull.push_back(extreme_hullpt_pair(hulls)); 
            for(int i = 0; i < m; ++i) {
                pair<int, int> p = next_hullpt_pair(hulls, hull[hull.size() - 1]); 
                vector<Point> output;
                if(p == hull[0]) {
                    for(int j = 0; j < hull.size(); ++j) {
                        output.push_back(hulls[hull[j].first][hull[j].second]);
                    }
                    output.pop_back();
                    return output; 
                }
                hull.push_back(p);
            }
        }
    }
    return {};
}

// Hàm tạo ra một tập hợp các điểm ngẫu nhiên
vector<Point> generateRandomPoints(int numberOfPoints, int maxRange) {
    vector<Point> Points;
    srand(time(nullptr));
    set<int> existingYValues;
    while (Points.size() < numberOfPoints) {
        int x = rand() % maxRange;
        int y;
        do {
            y = rand() % maxRange; 
        } while (existingYValues.find(y) != existingYValues.end()); 
        existingYValues.insert(y);
        Points.push_back(Point{x, y});
    }
    return Points;
}

// Hàm tính diện tích của một đa giác
double poly_area(const vector<Point> &P) 
{
	int n = P.size();
	double area = 0;
	for (int i = 0; i < n; i++) 
	{
		int j = (i + 1) % n;
		area += P[i].x* P[j].y - P[i].y * P[j].x;
	}
	return abs(area) / 2.0;
}

// Hàm kiểm tra xem một điểm có nằm trong một bao lồi hay không
bool is_Point_in_convex_hull(const vector<Point>& hull, const Point& p) {
    int n = hull.size();
    if (n < 3) return false;
    for (int i = 0; i < n; ++i) {
        int next = (i + 1) % n; 
        int orient = orientation(hull[i], hull[next], p);
        if (orient == RIGHT_TURN) {
            return false;
        }
    }
    return true;
}

int main(){
    vector<Point> v = {
        {3,5}, {6,8},{9,10}, {11,2},{4,9},{7,8},{9,9},{10,3},{11,15},
        {34,5},{32,1},{25,10},{18,8},{7,18},{10,14}
    };

    int n = v.size();

    cout << "Cac diem: \n";
    for (auto p : v) cout << p << "\t";
    cout << endl;
    
    vector<Point> output = chansalgorithm(v);
    
    cout << "Bao loi: \n";
    for(auto p : output) cout << p << "\t";
    cout << "\n";
    
    cout << "Dien tich bao loi: " << poly_area(output) << endl;
    
    pair<Point, Point> min_edge = find_min_edge(output);
    cout << "\nCanh ngan nhat trong bao loi: (" << min_edge.first.x << ", " << min_edge.first.y << ") va (" 
        << min_edge.second.x << ", " << min_edge.second.y << ")\n";
    cout << "Voi chieu dai la: " << dist(min_edge.first, min_edge.second) << endl;
    
    pair<Point, Point> closet_points = find_closet_points(v, v.size());
    cout << "\nDiem gan nhat trong tap diem: (" << closet_points.first.x << ", " << closet_points.first.y << ") va (" 
        << closet_points.second.x << ", " << closet_points.second.y << ")\n";
    cout << "Chieu dai canh gan nhat la: " << dist(closet_points.first, closet_points.second) << endl;

    Point test_point = {10, 10};
    if (is_Point_in_convex_hull(output, test_point)) {
        cout << "\nDiem (" << test_point.x << ", " << test_point.y << ") nam trong bao loi.\n";
    } else {
        cout << "\nDiem (" << test_point.x << ", " << test_point.y << ") nam ngoai bao loi.\n";
    }
    
    return 0;
}