// ZeroJudge b591 - 最小容量造船: 最小化 y = max_i(a_i*x + b_i), x>=0 (凸, 三分搜)
#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;

int n;
vector<double> A, B;
double g(double x) {
    double mx = -1e18;
    for (int i = 0; i < n; i++) { double v = A[i] * x + B[i]; if (v > mx) mx = v; }
    return mx;
}
int main() {
    while (scanf("%d", &n) == 1 && n != 0) {
        A.assign(n, 0); B.assign(n, 0);
        for (int i = 0; i < n; i++) scanf("%lf %lf", &A[i], &B[i]);
        double lo = 0, hi = 1e7;
        for (int it = 0; it < 300; it++) {
            double m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3;
            if (g(m1) < g(m2)) hi = m2; else lo = m1;
        }
        double x = (lo + hi) / 2, y = g(x);
        if (y <= 1e-9) printf("0\n");
        else printf("%.3f %.3f\n", y, x);
    }
    return 0;
}
