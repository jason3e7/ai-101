// ZeroJudge e294 - 小崴的新發現: 找最靠近 N 的「完全奇數」(每位都是奇數), 輸出 |K-N|
// 做法: 貪心構造 <=N 的最大完全奇數(floor) 與 >=N 的最小完全奇數(ceil), 取較近者
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;

ull floorOdd(const string& N) {
    int len = N.size();
    ull best = 0; bool has = false;
    // 同長度貪心: 前綴與 N 相同(須全奇), 在第 i 位放 < N[i] 的最大奇數, 其後補 9
    string pre;
    for (int i = 0; i < len; i++) {
        int ni = N[i] - '0';
        int d = -1;                       // < ni 的最大奇數
        for (int x = ni - 1; x >= 1; x--) if (x & 1) { d = x; break; }
        if (d >= 0) {
            string cand = pre + char('0' + d);
            for (int j = i + 1; j < len; j++) cand += '9';
            best = max(best, stoull(cand)); has = true;
        }
        if (ni & 1) pre += N[i];           // 可繼續 tight
        else { pre = ""; break; }          // 無法再對齊
        if (i == len - 1) { best = max(best, stoull(N)); has = true; } // N 本身全奇
    }
    // 較短: (len-1) 個 9
    if (len >= 2) { string s(len - 1, '9'); best = max(best, stoull(s)); has = true; }
    return has ? best : 0;
}
ull ceilOdd(const string& N) {
    int len = N.size();
    ull best = ULLONG_MAX;
    string pre;
    bool matched = true;
    for (int i = 0; i < len; i++) {
        int ni = N[i] - '0';
        int d = -1;                       // > ni 的最小奇數
        for (int x = ni + 1; x <= 9; x++) if (x & 1) { d = x; break; }
        if (d >= 0) {
            string cand = pre + char('0' + d);
            for (int j = i + 1; j < len; j++) cand += '1';
            best = min(best, stoull(cand));
        }
        if (ni & 1) pre += N[i];
        else { matched = false; break; }
        if (i == len - 1 && matched) best = min(best, stoull(N));
    }
    // 較長: (len+1) 個 1
    { string s(len + 1, '1'); best = min(best, stoull(s)); }
    return best;
}
int main() {
    string N;
    while (cin >> N) {
        ull n = stoull(N);
        ull lo = floorOdd(N), hi = ceilOdd(N);
        ull d1 = n - lo, d2 = hi - n;
        cout << min(d1, d2) << "\n";
    }
    return 0;
}
