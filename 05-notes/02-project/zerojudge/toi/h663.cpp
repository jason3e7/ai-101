// ZeroJudge h663 - 士兵排列: 最小化相鄰衝突總和 (開放 Hamiltonian path), 並輸出字典序最小排列
// bitmask DP: g[mask][last] = 已走訪 mask、目前停在 last, 完成剩餘所有點的最小額外成本
#include <bits/stdc++.h>
using namespace std;
const int INF = 1e9;
int N;
int cost_[17][17];
static int g[1<<17][17];
int main(){
    if (scanf("%d", &N) != 1) return 0;
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) scanf("%d", &cost_[i][j]);
    int FULL = (1<<N) - 1;
    for (int mask = FULL; mask >= 0; mask--) {
        for (int last = 0; last < N; last++) {
            if (!(mask & (1<<last))) continue;
            if (mask == FULL) { g[mask][last] = 0; continue; }
            int best = INF;
            for (int j = 0; j < N; j++) if (!(mask & (1<<j))) {
                int v = cost_[last][j] + g[mask | (1<<j)][j];
                if (v < best) best = v;
            }
            g[mask][last] = best;
        }
    }
    int globalMin = INF, start = 0;
    for (int s = 0; s < N; s++) if (g[1<<s][s] < globalMin) { globalMin = g[1<<s][s]; start = s; }
    // 貪心重建字典序最小 (節點 0-indexed, 輸出 +1)
    vector<int> seq;
    int mask = 1<<start, last = start;
    seq.push_back(start);
    while ((int)seq.size() < N) {
        for (int j = 0; j < N; j++) if (!(mask & (1<<j))) {
            if (cost_[last][j] + g[mask | (1<<j)][j] == g[mask][last]) { seq.push_back(j); mask |= (1<<j); last = j; break; }
        }
    }
    printf("%d\n", globalMin);
    for (int i = 0; i < N; i++) printf("%d%c", seq[i]+1, i+1<N?' ':'\n');
    return 0;
}
