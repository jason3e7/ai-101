// ZeroJudge b673 - How Big Is It (UVa 10012): m<=8 圓皆觸底, 枚舉排列貪心放置, 求最小框寬
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

int main() {
    int m;
    while (scanf("%d", &m) == 1 && m != 0) {
        vector<double> r(m);
        for (auto& x : r) scanf("%lf", &x);
        vector<int> idx(m);
        for (int i = 0; i < m; i++) idx[i] = i;
        double best = 1e18;
        do {
            vector<double> x(m);
            for (int k = 0; k < m; k++) {
                double xk = r[idx[k]];
                for (int j = 0; j < k; j++)
                    xk = max(xk, x[j] + 2.0 * sqrt(r[idx[j]] * r[idx[k]]));
                x[k] = xk;
            }
            double L = 1e18, R = -1e18;
            for (int k = 0; k < m; k++) { L = min(L, x[k] - r[idx[k]]); R = max(R, x[k] + r[idx[k]]); }
            best = min(best, R - L);
        } while (next_permutation(idx.begin(), idx.end()));
        printf("%.3f\n", best);
    }
    return 0;
}
