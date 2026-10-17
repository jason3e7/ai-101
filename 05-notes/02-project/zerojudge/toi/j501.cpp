// ZeroJudge j501 - 上菜-1: 出菜 c_k 分給點該菜且序號最小、尚未上菜的客人
#include <bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n;
    while (cin >> n) {
        vector<int> c(n), d(n);
        vector<vector<int>> byDish(n + 1);
        for (int i = 0; i < n; i++) {
            cin >> c[i] >> d[i];
            byDish[d[i]].push_back(i + 1);   // 客人序號遞增加入
        }
        vector<int> ptr(n + 1, 0);
        string out;
        for (int k = 0; k < n; k++) {
            int dish = c[k];
            int cust = byDish[dish][ptr[dish]++];
            out += to_string(cust);
            out += (k + 1 < n) ? ' ' : '\n';
        }
        cout << out;
    }
    return 0;
}
