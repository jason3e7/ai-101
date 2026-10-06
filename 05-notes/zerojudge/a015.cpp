// ZeroJudge a015 - 矩陣的翻轉 (transpose), 多組資料到 EOF
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int r, c;
    while (cin >> r >> c) {
        vector<vector<long long>> a(r, vector<long long>(c));
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++) cin >> a[i][j];
        for (int j = 0; j < c; j++) {
            for (int i = 0; i < r; i++) {
                if (i) cout << " ";
                cout << a[i][j];
            }
            cout << "\n";
        }
    }
    return 0;
}
