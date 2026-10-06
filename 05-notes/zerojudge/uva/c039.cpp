// ZeroJudge c039 / UVa100 - The 3n+1 problem
// 對 [min(i,j), max(i,j)] 求最大 cycle length; 輸出保留原本 i j 順序
#include <cstdio>
using namespace std;
int cyc(long long n) {
    int c = 1;
    while (n != 1) { n = (n & 1) ? 3 * n + 1 : n / 2; c++; }
    return c;
}
int main() {
    long long i, j;
    while (scanf("%lld %lld", &i, &j) == 2) {
        long long lo = i < j ? i : j, hi = i < j ? j : i;
        int mx = 0;
        for (long long n = lo; n <= hi; n++) {
            int c = cyc(n);
            if (c > mx) mx = c;
        }
        printf("%lld %lld %d\n", i, j, mx);
    }
    return 0;
}
