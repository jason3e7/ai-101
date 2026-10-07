// ZeroJudge s796 - 蜂蜜工廠: 每訂單區間[L,R]佔 1 天, 1 天 1 單, 最大化選中訂單酬勞
// 正解: 天數由大到小掃描; 訂單在其截止日 R 加入 max-heap(依酬勞);
//       每天放入「L<=t 且酬勞最大」的訂單; L>t 的訂單已無法安排 -> 丟棄
#include <bits/stdc++.h>
using namespace std;
int main(){
    int N, M;
    if (scanf("%d %d", &N, &M) != 2) return 0;
    vector<vector<pair<long long,int>>> byR(N + 1); // byR[R] = list of (P, L)
    for (int i = 0; i < M; i++) {
        long long L, R, P; scanf("%lld %lld %lld", &L, &R, &P);
        byR[R].push_back({P, (int)L});
    }
    priority_queue<pair<long long,int>> pq; // (profit, L)
    long long total = 0;
    for (int t = N; t >= 1; t--) {
        for (auto& job : byR[t]) pq.push(job);
        while (!pq.empty()) {
            auto top = pq.top();
            if (top.second > t) { pq.pop(); continue; } // L>t: 永遠放不下, 丟棄
            pq.pop(); total += top.first; break;          // 放這天, 一天一單
        }
    }
    printf("%lld\n", total);
    return 0;
}
