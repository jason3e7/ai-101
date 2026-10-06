// ZeroJudge a058 - MOD3: 統計 n 個數字除以 3 餘 0/1/2 各有幾個
#include <iostream>
using namespace std;

int main() {
    int n;
    while (cin >> n) {
        int cnt[3] = {0, 0, 0};
        for (int i = 0; i < n; i++) { int x; cin >> x; cnt[x % 3]++; }
        cout << cnt[0] << " " << cnt[1] << " " << cnt[2] << "\n";
    }
    return 0;
}
