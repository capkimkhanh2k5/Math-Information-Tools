#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <cmath>
#include <limits>
#include <map>
#include <set>
using namespace std;

// ==================== 1. KMP (Knuth-Morris-Pratt) ====================
class KMP {
private:
    vector<int> computeLPS(const string& pattern) {
        int m = pattern.length();
        vector<int> lps(m, 0);
        int len = 0, i = 1;
        
        while (i < m) {
            if (pattern[i] == pattern[len]) {
                lps[i++] = ++len;
            } else {
                if (len != 0) len = lps[len - 1];
                else lps[i++] = 0;
            }
        }
        return lps;
    }
    
public:
    vector<int> search(const string& text, const string& pattern) {
        vector<int> result;
        int n = text.length(), m = pattern.length();
        vector<int> lps = computeLPS(pattern);
        
        int i = 0, j = 0;
        while (i < n) {
            if (text[i] == pattern[j]) {
                i++; j++;
            }
            
            if (j == m) {
                result.push_back(i - j);
                j = lps[j - 1];
            } else if (i < n && text[i] != pattern[j]) {
                if (j != 0) j = lps[j - 1];
                else i++;
            }
        }
        return result;
    }
};

// ==================== 2. Aho-Corasick (Tìm nhiều pattern) ====================
class AhoCorasick {
private:
    struct Node {
        map<char, int> children;
        int fail = 0;
        vector<int> output;
    };
    
    vector<Node> trie;
    vector<string> patterns;
    
public:
    void build(const vector<string>& pats) {
        patterns = pats;
        trie.clear();
        trie.push_back(Node());
        
        // Xây dựng trie
        for (int idx = 0; idx < patterns.size(); idx++) {
            int curr = 0;
            for (char c : patterns[idx]) {
                if (!trie[curr].children.count(c)) {
                    trie[curr].children[c] = trie.size();
                    trie.push_back(Node());
                }
                curr = trie[curr].children[c];
            }
            trie[curr].output.push_back(idx);
        }
        
        // Xây dựng fail links (BFS)
        queue<int> q;
        for (auto [c, next] : trie[0].children) {
            q.push(next);
        }
        
        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            
            for (auto [c, next] : trie[curr].children) {
                q.push(next);
                int fail = trie[curr].fail;
                
                while (fail && !trie[fail].children.count(c)) {
                    fail = trie[fail].fail;
                }
                
                if (trie[fail].children.count(c) && trie[fail].children[c] != next) {
                    trie[next].fail = trie[fail].children[c];
                } else {
                    trie[next].fail = 0;
                }
                
                // Copy output từ fail link
                trie[next].output.insert(trie[next].output.end(),
                    trie[trie[next].fail].output.begin(),
                    trie[trie[next].fail].output.end());
            }
        }
    }
    
    vector<pair<int, int>> search(const string& text) {
        vector<pair<int, int>> result; // {position, pattern_index}
        int curr = 0;
        
        for (int i = 0; i < text.length(); i++) {
            char c = text[i];
            
            while (curr && !trie[curr].children.count(c)) {
                curr = trie[curr].fail;
            }
            
            if (trie[curr].children.count(c)) {
                curr = trie[curr].children[c];
            }
            
            for (int idx : trie[curr].output) {
                result.push_back({i - patterns[idx].length() + 1, idx});
            }
        }
        return result;
    }
};

// ==================== 3. Convex Hull (Graham Scan) ====================
struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
    
    bool operator<(const Point& p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
};

class ConvexHull {
private:
    double cross(const Point& O, const Point& A, const Point& B) {
        return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
    }
    
public:
    vector<Point> graham(vector<Point> points) {
        int n = points.size();
        if (n < 3) return points;
        
        sort(points.begin(), points.end());
        
        vector<Point> hull;
        
        // Lower hull
        for (int i = 0; i < n; i++) {
            while (hull.size() >= 2 && 
                   cross(hull[hull.size()-2], hull[hull.size()-1], points[i]) <= 0) {
                hull.pop_back();
            }
            hull.push_back(points[i]);
        }
        
        // Upper hull
        int lower_size = hull.size();
        for (int i = n - 2; i >= 0; i--) {
            while (hull.size() > lower_size && 
                   cross(hull[hull.size()-2], hull[hull.size()-1], points[i]) <= 0) {
                hull.pop_back();
            }
            hull.push_back(points[i]);
        }
        
        hull.pop_back();
        return hull;
    }
};

// ==================== 4. Segment Tree với Lazy Propagation ====================
class SegmentTree {
private:
    vector<long long> tree, lazy;
    int n;
    
    void push(int v, int tl, int tr) {
        if (lazy[v] != 0) {
            tree[v] += (tr - tl + 1) * lazy[v];
            if (tl != tr) {
                lazy[2*v] += lazy[v];
                lazy[2*v+1] += lazy[v];
            }
            lazy[v] = 0;
        }
    }
    
    void updateRange(int v, int tl, int tr, int l, int r, long long val) {
        push(v, tl, tr);
        if (l > r) return;
        
        if (l == tl && r == tr) {
            lazy[v] += val;
            push(v, tl, tr);
            return;
        }
        
        int tm = (tl + tr) / 2;
        updateRange(2*v, tl, tm, l, min(r, tm), val);
        updateRange(2*v+1, tm+1, tr, max(l, tm+1), r, val);
        
        push(2*v, tl, tm);
        push(2*v+1, tm+1, tr);
        tree[v] = tree[2*v] + tree[2*v+1];
    }
    
    long long queryRange(int v, int tl, int tr, int l, int r) {
        if (l > r) return 0;
        push(v, tl, tr);
        
        if (l == tl && r == tr) {
            return tree[v];
        }
        
        int tm = (tl + tr) / 2;
        return queryRange(2*v, tl, tm, l, min(r, tm)) +
               queryRange(2*v+1, tm+1, tr, max(l, tm+1), r);
    }
    
public:
    SegmentTree(int size) : n(size) {
        tree.resize(4 * n, 0);
        lazy.resize(4 * n, 0);
    }
    
    void update(int l, int r, long long val) {
        updateRange(1, 0, n-1, l, r, val);
    }
    
    long long query(int l, int r) {
        return queryRange(1, 0, n-1, l, r);
    }
};

// ==================== 5. Disjoint Set Union (DSU) với Path Compression ====================
class DSU {
private:
    vector<int> parent, rank;
    
public:
    DSU(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // Path compression
        }
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;
        
        // Union by rank
        if (rank[px] < rank[py]) swap(px, py);
        parent[py] = px;
        if (rank[px] == rank[py]) rank[px]++;
        return true;
    }
};

// ==================== 6. Maximum Flow (Dinic's Algorithm) ====================
class MaxFlow {
private:
    struct Edge {
        int to, rev;
        long long cap;
    };
    
    vector<vector<Edge>> graph;
    vector<int> level, iter;
    int n;
    
    bool bfs(int s, int t) {
        level.assign(n, -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto& e : graph[v]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }
    
    long long dfs(int v, int t, long long f) {
        if (v == t) return f;
        
        for (int& i = iter[v]; i < graph[v].size(); i++) {
            Edge& e = graph[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                long long d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    graph[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }
    
public:
    MaxFlow(int n) : n(n), graph(n), level(n), iter(n) {}
    
    void addEdge(int from, int to, long long cap) {
        graph[from].push_back({to, (int)graph[to].size(), cap});
        graph[to].push_back({from, (int)graph[from].size() - 1, 0});
    }
    
    long long maxFlow(int s, int t) {
        long long flow = 0;
        while (bfs(s, t)) {
            iter.assign(n, 0);
            long long f;
            while ((f = dfs(s, t, LLONG_MAX)) > 0) {
                flow += f;
            }
        }
        return flow;
    }
};

// ==================== MENU VÀ DEMO ====================
void demoKMP() {
    cout << "\n=== THUAT TOAN KMP ===\n";
    string text = "ABABDABACDABABCABAB";
    string pattern = "ABABCABAB";
    
    KMP kmp;
    vector<int> positions = kmp.search(text, pattern);
    
    cout << "Text: " << text << "\n";
    cout << "Pattern: " << pattern << "\n";
    cout << "Tim thay tai vi tri: ";
    for (int pos : positions) cout << pos << " ";
    cout << "\n";
}

void demoAhoCorasick() {
    cout << "\n=== THUAT TOAN AHO-CORASICK ===\n";
    vector<string> patterns = {"he", "she", "his", "hers"};
    string text = "ahishers";
    
    AhoCorasick ac;
    ac.build(patterns);
    auto results = ac.search(text);
    
    cout << "Text: " << text << "\n";
    cout << "Patterns: ";
    for (auto& p : patterns) cout << p << " ";
    cout << "\n\nKet qua:\n";
    for (auto [pos, idx] : results) {
        cout << "Vi tri " << pos << ": \"" << patterns[idx] << "\"\n";
    }
}

void demoConvexHull() {
    cout << "\n=== THUAT TOAN CONVEX HULL ===\n";
    vector<Point> points = {{0,0}, {1,1}, {2,2}, {0,2}, {2,0}, {1,0.5}};
    
    ConvexHull ch;
    vector<Point> hull = ch.graham(points);
    
    cout << "Cac diem:\n";
    for (auto& p : points) cout << "(" << p.x << ", " << p.y << ") ";
    cout << "\n\nConvex Hull:\n";
    for (auto& p : hull) cout << "(" << p.x << ", " << p.y << ") ";
    cout << "\n";
}

void demoSegmentTree() {
    cout << "\n=== SEGMENT TREE VOI LAZY PROPAGATION ===\n";
    SegmentTree st(10);
    
    st.update(0, 4, 5);
    st.update(2, 7, 3);
    
    cout << "Sau khi cap nhat [0,4] += 5 va [2,7] += 3:\n";
    cout << "Tong [0,4]: " << st.query(0, 4) << "\n";
    cout << "Tong [2,7]: " << st.query(2, 7) << "\n";
    cout << "Tong [5,9]: " << st.query(5, 9) << "\n";
}

void demoMaxFlow() {
    cout << "\n=== MAXIMUM FLOW (DINIC) ===\n";
    MaxFlow mf(6);
    
    // Xây dựng đồ thị
    mf.addEdge(0, 1, 16);
    mf.addEdge(0, 2, 13);
    mf.addEdge(1, 2, 10);
    mf.addEdge(1, 3, 12);
    mf.addEdge(2, 1, 4);
    mf.addEdge(2, 4, 14);
    mf.addEdge(3, 2, 9);
    mf.addEdge(3, 5, 20);
    mf.addEdge(4, 3, 7);
    mf.addEdge(4, 5, 4);
    
    long long maxflow = mf.maxFlow(0, 5);
    cout << "Max flow tu dinh 0 den dinh 5: " << maxflow << "\n";
}

int main() {
    cout << "========================================\n";
    cout << "   BO SUU TAP THUAT TOAN KHO TRONG C++\n";
    cout << "========================================\n";
    
    demoKMP();
    demoAhoCorasick();
    demoConvexHull();
    demoSegmentTree();
    demoMaxFlow();
    
    cout << "\n========================================\n";
    cout << "Cac thuat toan da duoc trinh dien!\n";
    cout << "========================================\n";
    
    return 0;