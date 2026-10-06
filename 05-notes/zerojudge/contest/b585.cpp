// ZeroJudge b585 - 來開派對: 最大子集 S, 每人在 S 中熟識>=2 且不熟識>=2
// 剝殼: 反覆移除違反條件者 (違反者在 S 任何子集也違反, 故移除安全), 剩下即最大合法集
#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n;
    while (cin >> n && n != 0) {
        vector<vector<int>> a(n, vector<int>(n));
        for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) cin >> a[i][j];
        vector<bool> in(n, true);
        bool changed = true;
        while (changed) {
            changed = false;
            for (int i = 0; i < n; i++) if (in[i]) {
                int acq = 0, non = 0;
                for (int j = 0; j < n; j++) if (j != i && in[j]) {
                    if (a[i][j]) acq++; else non++;
                }
                if (acq < 2 || non < 2) { in[i] = false; changed = true; }
            }
        }
        int cnt = 0; for (int i = 0; i < n; i++) if (in[i]) cnt++;
        cout << cnt << "\n";
    }
    return 0;
}
