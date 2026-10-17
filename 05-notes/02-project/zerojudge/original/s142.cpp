// ZeroJudge s142 - 最大正方形: 全 1 最大正方形邊長
// 記憶體限制僅 10MB -> 不存整個矩陣, 用滾動一維 dp (O(m) 空間), 邊讀邊算
#include <cstdio>
static int dp[1002];
inline int rd() {           // 讀下一個 0/1 (跳過空白換行)
    int c = getchar_unlocked();
    while (c != '0' && c != '1') { if (c == EOF) return -1; c = getchar_unlocked(); }
    return c - '0';
}
int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;
    int best = 0;
    for (int i = 0; i < n; i++) {
        int prev = 0;                 // dp[i-1][j-1]
        for (int j = 1; j <= m; j++) {
            int up = dp[j];           // dp[i-1][j]
            int c = rd();
            if (c == 1) {
                int v = up;
                if (dp[j-1] < v) v = dp[j-1];
                if (prev < v) v = prev;
                dp[j] = v + 1;
            } else dp[j] = 0;
            prev = up;
            if (dp[j] > best) best = dp[j];
        }
    }
    printf("%d\n", best);
    return 0;
}
