// ZeroJudge a147 - Print it all: 輸出所有 0<i<n 且不被 7 整除的數, n=0 結束
#include <iostream>
using namespace std;

int main() {
    int n;
    while (cin >> n && n != 0) {
        bool first = true;
        for (int i = 1; i < n; i++) {
            if (i % 7 == 0) continue;
            if (!first) cout << " ";
            cout << i; first = false;
        }
        cout << "\n";
    }
    return 0;
}
