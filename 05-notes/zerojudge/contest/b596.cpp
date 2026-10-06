// ZeroJudge b596 - Less is better: 凸包頂點數 (排除共線點), Andrew monotone chain
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;
struct P { ll x, y; };
ll cross(P O, P A, P B) { return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x); }

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n;
    while (cin >> n && n != 0) {
        vector<P> pts(n);
        for (auto& p : pts) cin >> p.x >> p.y;
        sort(pts.begin(), pts.end(), [](const P& a, const P& b) { return a.x < b.x || (a.x == b.x && a.y < b.y); });
        pts.erase(unique(pts.begin(), pts.end(), [](const P& a, const P& b) { return a.x == b.x && a.y == b.y; }), pts.end());
        int m = pts.size();
        if (m <= 2) { cout << m << "\n"; continue; }
        vector<P> h(2 * m);
        int k = 0;
        for (int i = 0; i < m; i++) { while (k >= 2 && cross(h[k-2], h[k-1], pts[i]) <= 0) k--; h[k++] = pts[i]; }
        int lower = k + 1;
        for (int i = m - 2; i >= 0; i--) { while (k >= lower && cross(h[k-2], h[k-1], pts[i]) <= 0) k--; h[k++] = pts[i]; }
        cout << (k - 1) << "\n";
    }
    return 0;
}
