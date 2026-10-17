// ZeroJudge a059 - 完全平方和: [a,b] 間所有完全平方數之和 (Case 格式)
#include <iostream>
using namespace std;

int main() {
    int T;
    if (!(cin >> T)) return 0;
    for (int t = 1; t <= T; t++) {
        long long a, b, sum = 0;
        cin >> a >> b;
        for (long long k = 0; k * k <= b; k++)
            if (k * k >= a) sum += k * k;
        cout << "Case " << t << ": " << sum << "\n";
    }
    return 0;
}
