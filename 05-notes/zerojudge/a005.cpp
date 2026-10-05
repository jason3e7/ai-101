// ZeroJudge a005 - Eva 的回家作業
// 給前四項 (等差或等比), 輸出前五項
#include <iostream>
using namespace std;

int main() {
    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        long long a, b, c, d, e;
        cin >> a >> b >> c >> d;
        if (b - a == c - b && c - b == d - c) e = d + (b - a);  // 等差
        else e = d * (b / a);                                   // 等比
        cout << a << " " << b << " " << c << " " << d << " " << e << "\n";
    }
    return 0;
}
