// ZeroJudge a148 - You Cannot Pass?!: 平均 > 59 過關; 被當輸出 yes 否則 no
#include <iostream>
using namespace std;

int main() {
    int n;
    while (cin >> n) {
        long long sum = 0;
        for (int i = 0; i < n; i++) { int x; cin >> x; sum += x; }
        cout << (sum > 59LL * n ? "no" : "yes") << "\n";
    }
    return 0;
}
