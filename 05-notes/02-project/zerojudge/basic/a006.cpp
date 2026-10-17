// ZeroJudge a006 - 一元二次方程式 ax^2+bx+c=0 (根均為整數, 大者在前)
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

int main() {
    long long a, b, c;
    while (cin >> a >> b >> c) {
        long long D = b * b - 4 * a * c;
        if (D < 0) { cout << "No real root\n"; continue; }
        long long s = (long long)sqrtl((long double)D);
        while (s > 0 && s * s > D) s--;
        while ((s + 1) * (s + 1) <= D) s++;
        if (D == 0) {
            cout << "Two same roots x=" << (-b / (2 * a)) << "\n";
        } else {
            long long r1 = (-b + s) / (2 * a), r2 = (-b - s) / (2 * a);
            cout << "Two different roots x1=" << max(r1, r2) << " , x2=" << min(r1, r2) << "\n";
        }
    }
    return 0;
}
