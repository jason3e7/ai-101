// ZeroJudge d206 / UVa108 - Maximum Sum
// N*N 陣列最大子矩形和: 固定上下界壓成一維, Kadane. O(N^3)
#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    while (scanf("%d", &n) == 1) {
        vector<vector<int>> a(n, vector<int>(n));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) scanf("%d", &a[i][j]);
        long long best = LLONG_MIN;
        for (int top = 0; top < n; top++) {
            vector<long long> col(n, 0);
            for (int bot = top; bot < n; bot++) {
                for (int j = 0; j < n; j++) col[j] += a[bot][j];
                // 一維最大連續和 (至少一個元素)
                long long cur = 0, mx = LLONG_MIN;
                for (int j = 0; j < n; j++) {
                    cur = max(col[j], cur + col[j]);
                    mx = max(mx, cur);
                }
                best = max(best, mx);
            }
        }
        printf("%lld\n", best);
    }
    return 0;
}
