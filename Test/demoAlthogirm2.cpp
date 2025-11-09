#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <cmath>
#include <complex>
#include <map>
#include <set>
#include <bitset>
#include <climits>
using namespace std;

const double PI = acos(-1.0);
typedef complex<double> cd;

// ==================== 1. FFT (Fast Fourier Transform) ====================
class FFT {
public:
    void fft(vector<cd>& a, bool invert) {
        int n = a.size();
        if (n == 1) return;
        
        vector<cd> a0(n/2), a1(n/2);
        for (int i = 0; i < n/2; i++) {
            a0[i] = a[2*i];
            a1[i] = a[2*i+1];
        }
        
        fft(a0, invert);
        fft(a1, invert);
        
        double ang = 2 * PI / n * (invert ? -1 : 1);
        cd w(1), wn(cos(ang), sin(ang));
        
        for (int i = 0; i < n/2; i++) {
            a[i] = a0[i] + w * a1[i];
            a[i + n/2] = a0[i] - w * a1[i];
            if (invert) {
                a[i] /= 2;
                a[i + n/2] /= 2;
            }
            w *= wn;
        }
    }
    
    vector<long long> multiply(vector<int>& a, vector<int>& b) {
        vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        int n = 1;
        while (n < a.size() + b.size()) n <<= 1;
        fa.resize(n);
        fb.resize(n);
        
        fft(fa, false);
        fft(fb, false);
        
        for (int i = 0; i < n; i++) {
            fa[i] *= fb[i];
        }
        
        fft(fa, true);
        
        vector<long long> result(n);
        for (int i = 0; i < n; i++) {
            result[i] = round(fa[i].real());
        }
        return result;
    }
};

// ==================== 2. Heavy-Light Decomposition ====================
class HLD {
private:
    vector<vector<int>> adj;
    vector<int> parent, depth, heavy, head, pos;
    vector<long long> tree;
    int n, cur_pos;
    
    int dfs(int v, int p = -1) {
        int size = 1, max_subtree = 0;
        parent[v] = p;
        
        for (int u : adj[v]) {
            if (u == p) continue;
            depth[u] = depth[v] + 1;
            int subtree = dfs(u, v);
            size += subtree;
            if (subtree > max_subtree) {
                max_subtree = subtree;
                heavy[v] = u;
            }
        }
        return size;
    }
    
    void decompose(int v, int h, int p = -1) {
        head[v] = h;
        pos[v] = cur_pos++;
        
        if (heavy[v] != -1) {
            decompose(heavy[v], h, v);
        }
        
        for (int u : adj[v]) {
            if (u != p && u != heavy[v]) {
                decompose(u, u, v);
            }
        }
    }
    
    void updateTree(int idx, long long val) {
        idx += n;
        tree[idx] = val;
        while (idx > 1) {
            idx /= 2;
            tree[idx] = tree[2*idx] + tree[2*idx+1];
        }
    }
    
    long long queryTree(int l, int r) {
        l += n; r += n;
        long long sum = 0;
        while (l <= r) {
            if (l % 2 == 1) sum += tree[l++];
            if (r % 2 == 0) sum += tree[r--];
            l /= 2; r /= 2;
        }
        return sum;
    }
    
public:
    HLD(int n) : n(n), adj(n), parent(n), depth(n), heavy(n, -1), 
                 head(n), pos(n), tree(2*n), cur_pos(0) {}
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    void build(int root = 0) {
        depth[root] = 0;
        dfs(root);
        decompose(root, root);
    }
    
    void update(int v, long long val) {
        updateTree(pos[v], val);
    }
    
    long long query(int u, int v) {
        long long sum = 0;
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            sum += queryTree(pos[head[u]], pos[u]);
            u = parent[head[u]];
        }
        if (depth[u] > depth[v]) swap(u, v);
        sum += queryTree(pos[u], pos[v]);
        return sum;
    }
};

// ==================== 3. Manacher's Algorithm (Tìm Palindrome dài nhất) ====================
class Manacher {
public:
    string longestPalindrome(string s) {
        string t = "#";
        for (char c : s) {
            t += c;
            t += "#";
        }
        
        int n = t.length();
        vector<int> p(n, 0);
        int c = 0, r = 0;
        int maxLen = 0, centerIndex = 0;
        
        for (int i = 0; i < n; i++) {
            int mirror = 2*c - i;
            
            if (i < r) {
                p[i] = min(r - i, p[mirror]);
            }
            
            int a = i + (1 + p[i]);
            int b = i - (1 + p[i]);
            while (a < n && b >= 0 && t[a] == t[b]) {
                p[i]++;
                a++; b--;
            }
            
            if (i + p[i] > r) {
                c = i;
                r = i + p[i];
            }
            
            if (p[i] > maxLen) {
                maxLen = p[i];
                centerIndex = i;
            }
        }
        
        int start = (centerIndex - maxLen) / 2;
        return s.substr(start, maxLen);
    }
    
    vector<int> getAllPalindromes(string s) {
        string t = "#";
        for (char c : s) {
            t += c;
            t += "#";
        }
        
        int n = t.length();
        vector<int> p(n, 0);
        int c = 0, r = 0;
        
        for (int i = 0; i < n; i++) {
            int mirror = 2*c - i;
            if (i < r) p[i] = min(r - i, p[mirror]);
            
            int a = i + (1 + p[i]);
            int b = i - (1 + p[i]);
            while (a < n && b >= 0 && t[a] == t[b]) {
                p[i]++;
                a++; b--;
            }
            
            if (i + p[i] > r) {
                c = i;
                r = i + p[i];
            }
        }
        
        return p;
    }
};

// ==================== 4. Tarjan's Algorithm (Tìm Strongly Connected Components) ====================
class Tarjan {
private:
    vector<vector<int>> adj;
    vector<int> ids, low, on_stack;
    vector<bool> visited;
    vector<vector<int>> sccs;
    int id, n;
    
    void dfs(int at, vector<int>& stack) {
        stack.push_back(at);
        on_stack[at] = 1;
        visited[at] = true;
        ids[at] = low[at] = id++;
        
        for (int to : adj[at]) {
            if (!visited[to]) dfs(to, stack);
            if (on_stack[to]) low[at] = min(low[at], low[to]);
        }
        
        if (ids[at] == low[at]) {
            vector<int> scc;
            while (true) {
                int node = stack.back();
                stack.pop_back();
                on_stack[node] = 0;
                scc.push_back(node);
                if (node == at) break;
            }
            sccs.push_back(scc);
        }
    }
    
public:
    Tarjan(int n) : n(n), adj(n), ids(n), low(n), on_stack(n), 
                    visited(n, false), id(0) {}
    
    void addEdge(int from, int to) {
        adj[from].push_back(to);
    }
    
    vector<vector<int>> findSCCs() {
        vector<int> stack;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                dfs(i, stack);
            }
        }
        return sccs;
    }
};

// ==================== 5. Suffix Automaton ====================
class SuffixAutomaton {
private:
    struct State {
        int len, link;
        map<char, int> next;
    };
    
    vector<State> st;
    int last, sz;
    
public:
    SuffixAutomaton(const string& s) {
        sz = 1;
        last = 0;
        st.resize(2 * s.length());
        st[0].len = 0;
        st[0].link = -1;
        
        for (char c : s) {
            extend(c);
        }
    }
    
    void extend(char c) {
        int cur = sz++;
        st[cur].len = st[last].len + 1;
        
        int p = last;
        while (p != -1 && !st[p].next.count(c)) {
            st[p].next[c] = cur;
            p = st[p].link;
        }
        
        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = sz++;
                st[clone].len = st[p].len + 1;
                st[clone].next = st[q].next;
                st[clone].link = st[q].link;
                
                while (p != -1 && st[p].next[c] == q) {
                    st[p].next[c] = clone;
                    p = st[p].link;
                }
                
                st[q].link = st[cur].link = clone;
            }
        }
        
        last = cur;
    }
    
    bool contains(const string& s) {
        int v = 0;
        for (char c : s) {
            if (!st[v].next.count(c)) return false;
            v = st[v].next[c];
        }
        return true;
    }
    
    long long countDistinctSubstrings() {
        long long ans = 0;
        for (int i = 1; i < sz; i++) {
            ans += st[i].len - st[st[i].link].len;
        }
        return ans;
    }
};

// ==================== 6. Treap (Cartesian Tree) ====================
class Treap {
private:
    struct Node {
        int key, priority, size;
        Node *left, *right;
        
        Node(int k) : key(k), priority(rand()), size(1), left(nullptr), right(nullptr) {}
    };
    
    Node* root;
    
    int getSize(Node* t) { return t ? t->size : 0; }
    
    void updateSize(Node* t) {
        if (t) t->size = 1 + getSize(t->left) + getSize(t->right);
    }
    
    void split(Node* t, int key, Node*& l, Node*& r) {
        if (!t) {
            l = r = nullptr;
        } else if (key < t->key) {
            split(t->left, key, l, t->left);
            r = t;
            updateSize(r);
        } else {
            split(t->right, key, t->right, r);
            l = t;
            updateSize(l);
        }
    }
    
    void merge(Node*& t, Node* l, Node* r) {
        if (!l || !r) {
            t = l ? l : r;
        } else if (l->priority > r->priority) {
            merge(l->right, l->right, r);
            t = l;
        } else {
            merge(r->left, l, r->left);
            t = r;
        }
        updateSize(t);
    }
    
    void insert(Node*& t, Node* item) {
        if (!t) {
            t = item;
        } else if (item->priority > t->priority) {
            split(t, item->key, item->left, item->right);
            t = item;
        } else {
            insert(item->key < t->key ? t->left : t->right, item);
        }
        updateSize(t);
    }
    
    void erase(Node*& t, int key) {
        if (!t) return;
        if (t->key == key) {
            Node* temp = t;
            merge(t, t->left, t->right);
            delete temp;
        } else {
            erase(key < t->key ? t->left : t->right, key);
            updateSize(t);
        }
    }
    
    void inorder(Node* t, vector<int>& result) {
        if (!t) return;
        inorder(t->left, result);
        result.push_back(t->key);
        inorder(t->right, result);
    }
    
public:
    Treap() : root(nullptr) { srand(time(0)); }
    
    void insert(int key) {
        Node* item = new Node(key);
        insert(root, item);
    }
    
    void erase(int key) {
        erase(root, key);
    }
    
    int size() { return getSize(root); }
    
    vector<int> getElements() {
        vector<int> result;
        inorder(root, result);
        return result;
    }
};

// ==================== 7. Miller-Rabin Primality Test ====================
class MillerRabin {
private:
    long long power(long long a, long long d, long long n) {
        long long res = 1;
        a %= n;
        while (d > 0) {
            if (d & 1) res = (__int128)res * a % n;
            a = (__int128)a * a % n;
            d >>= 1;
        }
        return res;
    }
    
    bool check(long long a, long long d, long long n, int r) {
        long long x = power(a, d, n);
        if (x == 1 || x == n - 1) return true;
        
        for (int i = 0; i < r - 1; i++) {
            x = (__int128)x * x % n;
            if (x == n - 1) return true;
        }
        return false;
    }
    
public:
    bool isPrime(long long n) {
        if (n < 2) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0) return false;
        
        long long d = n - 1;
        int r = 0;
        while (d % 2 == 0) {
            d /= 2;
            r++;
        }
        
        vector<long long> witnesses = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
        for (long long a : witnesses) {
            if (a >= n) continue;
            if (!check(a, d, n, r)) return false;
        }
        return true;
    }
};

// ==================== DEMO ====================
void demoFFT() {
    cout << "\n=== FFT (NHAN DA THUC NHANH) ===\n";
    vector<int> a = {1, 2, 3};  // 1 + 2x + 3x^2
    vector<int> b = {4, 5};     // 4 + 5x
    
    FFT fft;
    vector<long long> result = fft.multiply(a, b);
    
    cout << "Da thuc A: 1 + 2x + 3x^2\n";
    cout << "Da thuc B: 4 + 5x\n";
    cout << "Tich A*B: ";
    for (int i = 0; i < a.size() + b.size() - 1; i++) {
        cout << result[i] << " ";
    }
    cout << "\n";
}

void demoHLD() {
    cout << "\n=== HEAVY-LIGHT DECOMPOSITION ===\n";
    HLD hld(7);
    hld.addEdge(0, 1);
    hld.addEdge(0, 2);
    hld.addEdge(1, 3);
    hld.addEdge(1, 4);
    hld.addEdge(2, 5);
    hld.addEdge(2, 6);
    hld.build();
    
    hld.update(0, 1);
    hld.update(1, 2);
    hld.update(2, 3);
    hld.update(3, 4);
    
    cout << "Truy van duong di tu 3 den 2: " << hld.query(3, 2) << "\n";
}

void demoManacher() {
    cout << "\n=== MANACHER'S ALGORITHM ===\n";
    string s = "babad";
    
    Manacher m;
    string longest = m.longestPalindrome(s);
    
    cout << "Chuoi: " << s << "\n";
    cout << "Palindrome dai nhat: " << longest << "\n";
}

void demoTarjan() {
    cout << "\n=== TARJAN'S ALGORITHM (SCC) ===\n";
    Tarjan tarjan(8);
    tarjan.addEdge(0, 1);
    tarjan.addEdge(1, 2);
    tarjan.addEdge(2, 0);
    tarjan.addEdge(2, 3);
    tarjan.addEdge(3, 4);
    tarjan.addEdge(4, 5);
    tarjan.addEdge(5, 3);
    tarjan.addEdge(6, 7);
    tarjan.addEdge(7, 6);
    
    auto sccs = tarjan.findSCCs();
    cout << "So luong SCC: " << sccs.size() << "\n";
    for (int i = 0; i < sccs.size(); i++) {
        cout << "SCC " << i+1 << ": ";
        for (int node : sccs[i]) cout << node << " ";
        cout << "\n";
    }
}

void demoSuffixAutomaton() {
    cout << "\n=== SUFFIX AUTOMATON ===\n";
    string s = "abcbc";
    SuffixAutomaton sa(s);
    
    cout << "Chuoi: " << s << "\n";
    cout << "So chuoi con khac nhau: " << sa.countDistinctSubstrings() << "\n";
    cout << "Chua 'bc': " << (sa.contains("bc") ? "Co" : "Khong") << "\n";
    cout << "Chua 'xyz': " << (sa.contains("xyz") ? "Co" : "Khong") << "\n";
}

void demoTreap() {
    cout << "\n=== TREAP (CARTESIAN TREE) ===\n";
    Treap treap;
    
    vector<int> values = {5, 2, 8, 1, 9, 3};
    for (int v : values) treap.insert(v);
    
    cout << "Them cac phan tu: ";
    for (int v : values) cout << v << " ";
    cout << "\n";
    
    cout << "Cac phan tu sau khi sap xep: ";
    auto elements = treap.getElements();
    for (int v : elements) cout << v << " ";
    cout << "\n";
}

void demoMillerRabin() {
    cout << "\n=== MILLER-RABIN PRIMALITY TEST ===\n";
    MillerRabin mr;
    
    vector<long long> numbers = {17, 100, 1000000007, 1000000009, 123456789};
    for (long long n : numbers) {
        cout << n << " la so " << (mr.isPrime(n) ? "NGUYEN TO" : "KHONG NGUYEN TO") << "\n";
    }
}

int main() {
    cout << "==============================================\n";
    cout << "   THUAT TOAN CUC KY KHO - PHAN 2\n";
    cout << "==============================================\n";
    
    demoFFT();
    demoHLD();
    demoManacher();
    demoTarjan();
    demoSuffixAutomaton();
    demoTreap();
    demoMillerRabin();
    
    cout << "\n==============================================\n";
    cout << "Tat ca thuat toan da duoc trinh dien!\n";
    cout << "==============================================\n";
    
    return 0;
}