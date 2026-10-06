// ZeroJudge b598 - Minimize the Number of Coins
// 四種硬幣湊出 price, 最小化總枚數; 平手依序最小化第1、2、3種硬幣數; 湊不出輸出 0
#include <iostream>
#include <vector>
using namespace std;

struct St { int total, c[4]; bool valid; };

// a 比 b 好?
bool better(const St& a, const St& b) {
    if (!b.valid) return a.valid;
    if (!a.valid) return false;
    if (a.total != b.total) return a.total < b.total;
    for (int i = 0; i < 3; i++) if (a.c[i] != b.c[i]) return a.c[i] < b.c[i];
    return false;
}
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int v[4], price;
    while (cin >> v[0] && v[0] != 0) {
        cin >> v[1] >> v[2] >> v[3] >> price;
        vector<St> dp(price + 1);
        for (auto& s : dp) { s.valid = false; }
        dp[0] = {0, {0,0,0,0}, true};
        for (int a = 1; a <= price; a++) {
            for (int j = 0; j < 4; j++) {
                if (a >= v[j] && dp[a - v[j]].valid) {
                    St cand = dp[a - v[j]];
                    cand.total++; cand.c[j]++;
                    if (better(cand, dp[a])) dp[a] = cand;
                }
            }
        }
        if (dp[price].valid) {
            cout << dp[price].total;
            for (int i = 0; i < 4; i++) cout << " " << dp[price].c[i];
            cout << "\n";
        } else cout << "0\n";
    }
    return 0;
}
