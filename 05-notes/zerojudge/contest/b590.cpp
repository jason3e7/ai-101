// ZeroJudge b590 - 單位分數分解 (POJ 1980): 把 p/q 分解成 <=n 個單位分數(分母<=a)的方法數
// DFS, 分母非遞減避免重複; 剪枝
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;

ll A, cnt;
void dfs(ll num, ll den, int terms, ll start) {
    if (num == 0) { cnt++; return; }
    if (terms == 0) return;
    ll lo = max(start, (den + num - 1) / num);   // 1/d <= num/den -> d >= ceil(den/num)
    for (ll d = lo; d <= A; d++) {
        if (num * d > (ll)terms * den) break;     // 剩餘無法用 terms 個 <=1/d 湊到
        ll nn = num * d - den, nd = den * d;
        if (nn != 0) { ll g = __gcd(nn, nd); nn /= g; nd /= g; }
        dfs(nn, nd, terms - 1, d);
    }
}
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    ll p, q, a; int n;
    while (cin >> p >> q >> a >> n) {
        if (p == 0 && q == 0 && a == 0 && n == 0) break;
        A = a; cnt = 0;
        ll g = __gcd(p, q); p /= g; q /= g;
        dfs(p, q, n, 1);
        cout << cnt << "\n";
    }
    return 0;
}
