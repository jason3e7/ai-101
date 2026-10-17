// ZeroJudge b584 - 過橋問題 (手電筒, 一次最多兩人, 兩人取較慢者時間)
// 排序後每輪處理最慢兩人, 取兩策略最小; 基底 1/2/3 人
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int m;
    while (cin >> m && m != 0) {
        vector<long long> t(m);
        for (auto& x : t) cin >> x;
        sort(t.begin(), t.end());
        long long total = 0;
        int i = m - 1;
        while (i >= 3) {
            total += min(t[0] + 2 * t[1] + t[i], 2 * t[0] + t[i] + t[i - 1]);
            i -= 2;
        }
        if (i == 2) total += t[0] + t[1] + t[2];
        else if (i == 1) total += t[1];
        else if (i == 0) total += t[0];
        cout << total << "\n";
    }
    return 0;
}
