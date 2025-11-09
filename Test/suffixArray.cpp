#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class SuffixArray {
private:
    string text;
    int n;
    vector<int> sa;      // Suffix Array
    vector<int> lcp;     // LCP Array (Longest Common Prefix)
    vector<int> rank;    // Rank array
    
    // Hàm so sánh cho việc sắp xếp suffix
    struct Compare {
        const vector<int>& rank;
        int k;
        int n;
        
        Compare(const vector<int>& r, int k, int n) : rank(r), k(k), n(n) {}
        
        bool operator()(int i, int j) const {
            if (rank[i] != rank[j]) return rank[i] < rank[j];
            int ri = (i + k < n) ? rank[i + k] : -1;
            int rj = (j + k < n) ? rank[j + k] : -1;
            return ri < rj;
        }
    };
    
    // Xây dựng Suffix Array bằng thuật toán O(n log^2 n)
    void buildSuffixArray() {
        sa.resize(n);
        rank.resize(n);
        vector<int> tmp(n);
        
        // Khởi tạo rank ban đầu dựa trên ký tự đầu tiên
        for (int i = 0; i < n; i++) {
            sa[i] = i;
            rank[i] = text[i];
        }
        
        // Sắp xếp suffix theo từng bước k = 1, 2, 4, 8, ...
        for (int k = 1; k < n; k *= 2) {
            // Sắp xếp các suffix
            sort(sa.begin(), sa.end(), Compare(rank, k, n));
            
            // Cập nhật rank mới
            tmp[sa[0]] = 0;
            for (int i = 1; i < n; i++) {
                Compare cmp(rank, k, n);
                tmp[sa[i]] = tmp[sa[i-1]] + (cmp(sa[i-1], sa[i]) ? 1 : 0);
            }
            rank = tmp;
        }
    }
    
    // Xây dựng LCP Array bằng thuật toán Kasai O(n)
    void buildLCPArray() {
        lcp.resize(n);
        vector<int> inv(n);
        
        // Tạo inverse suffix array
        for (int i = 0; i < n; i++) {
            inv[sa[i]] = i;
        }
        
        int h = 0;
        for (int i = 0; i < n; i++) {
            if (inv[i] > 0) {
                int j = sa[inv[i] - 1];
                while (i + h < n && j + h < n && text[i + h] == text[j + h]) {
                    h++;
                }
                lcp[inv[i]] = h;
                if (h > 0) h--;
            }
        }
    }
    
public:
    SuffixArray(const string& s) : text(s), n(s.length()) {
        buildSuffixArray();
        buildLCPArray();
    }
    
    // Tìm vị trí xuất hiện của pattern trong text
    vector<int> search(const string& pattern) {
        vector<int> result;
        int m = pattern.length();
        
        // Tìm kiếm nhị phân tìm suffix đầu tiên >= pattern
        int left = 0, right = n;
        while (left < right) {
            int mid = (left + right) / 2;
            if (text.substr(sa[mid], m) < pattern)
                left = mid + 1;
            else
                right = mid;
        }
        int start = left;
        
        // Tìm suffix cuối cùng bắt đầu với pattern
        left = 0; right = n;
        while (left < right) {
            int mid = (left + right) / 2;
            if (text.substr(sa[mid], m) <= pattern)
                left = mid + 1;
            else
                right = mid;
        }
        int end = left;
        
        // Thu thập tất cả vị trí
        for (int i = start; i < end; i++) {
            if (text.substr(sa[i], m) == pattern) {
                result.push_back(sa[i]);
            }
        }
        
        return result;
    }
    
    // Tìm chuỗi con lặp lại dài nhất
    pair<int, string> longestRepeatedSubstring() {
        int maxLen = 0;
        int maxPos = 0;
        
        for (int i = 1; i < n; i++) {
            if (lcp[i] > maxLen) {
                maxLen = lcp[i];
                maxPos = sa[i];
            }
        }
        
        return {maxLen, text.substr(maxPos, maxLen)};
    }
    
    // Đếm số chuỗi con khác nhau
    long long countDistinctSubstrings() {
        long long total = (long long)n * (n + 1) / 2;
        long long duplicate = 0;
        
        for (int i = 1; i < n; i++) {
            duplicate += lcp[i];
        }
        
        return total - duplicate;
    }
    
    // In Suffix Array
    void printSuffixArray() {
        cout << "\n=== SUFFIX ARRAY ===\n";
        cout << "Index\tSA[i]\tLCP[i]\tSuffix\n";
        cout << "-----\t-----\t------\t------\n";
        for (int i = 0; i < n; i++) {
            cout << i << "\t" << sa[i] << "\t";
            if (i == 0) cout << "-\t";
            else cout << lcp[i] << "\t";
            cout << text.substr(sa[i]) << "\n";
        }
    }
};

int main() {
    string text;
    
    cout << "=== CHUONG TRINH SUFFIX ARRAY VA LCP ARRAY ===\n\n";
    cout << "Nhap chuoi: ";
    getline(cin, text);
    
    if (text.empty()) {
        text = "banana";
        cout << "Su dung chuoi mac dinh: " << text << "\n";
    }
    
    SuffixArray sa(text);
    
    // In Suffix Array
    sa.printSuffixArray();
    
    // Tìm chuỗi con lặp lại dài nhất
    auto lrs = sa.longestRepeatedSubstring();
    cout << "\n=== CHUOI CON LAP LAI DAI NHAT ===\n";
    cout << "Do dai: " << lrs.first << "\n";
    cout << "Chuoi: \"" << lrs.second << "\"\n";
    
    // Đếm số chuỗi con khác nhau
    cout << "\n=== SO CHUOI CON KHAC NHAU ===\n";
    cout << "Tong so: " << sa.countDistinctSubstrings() << "\n";
    
    // Tìm kiếm pattern
    cout << "\n=== TIM KIEM PATTERN ===\n";
    cout << "Nhap pattern can tim (Enter de bo qua): ";
    string pattern;
    getline(cin, pattern);
    
    if (!pattern.empty()) {
        vector<int> positions = sa.search(pattern);
        if (positions.empty()) {
            cout << "Khong tim thay \"" << pattern << "\" trong chuoi.\n";
        } else {
            cout << "Tim thay \"" << pattern << "\" tai cac vi tri: ";
            for (int pos : positions) {
                cout << pos << " ";
            }
            cout << "\n";
        }
    }
    
    return 0;
}