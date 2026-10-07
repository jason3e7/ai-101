// ZeroJudge h040 - 釣魚: 給朋友數 F_1..F_N 與目標竿 K, 求最小初始耐心 V0 (正整數)
// 總可達竿數 = V0 + V1 + ... , V_t = floor(V_{t-1} * min(log2(F_t+1),30)/30); V_t=0 即放棄
// f(V0) 單調 -> 對 V0 二分搜最小使 f(V0) >= K
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int N; vector<double> ratio_;
// 回傳 min(f(V0), cap) 以避免溢位 (cap 稍大於最大 K)
ll reach(ll V0, ll cap){
    ll total = V0;
    double v = (double)V0;
    for (int t = 0; t < N && total < cap; t++) {
        v = floor(v * ratio_[t]);
        if (v <= 0) break;
        total += (ll)v;
    }
    return total;
}
int main(){
    scanf("%d", &N);
    ratio_.resize(N);
    for (int i = 0; i < N; i++) { double F; scanf("%lf", &F); ratio_[i] = min(log2(F+1.0), 30.0) / 30.0; }
    int Q; scanf("%d", &Q);
    while (Q--) {
        ll K; scanf("%lld", &K);
        ll lo = 1, hi = K;  // V0=K 一定夠 (seg0 就有 K 竿)
        while (lo < hi) {
            ll mid = (lo + hi) / 2;
            if (reach(mid, K) >= K) hi = mid; else lo = mid + 1;
        }
        printf("%lld\n", lo);
    }
    return 0;
}
