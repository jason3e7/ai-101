// ZeroJudge d424 / UVa105 - The Skyline Problem
// 高度陣列掃描: 每棟建築塗 [L,R) 高度取 max, 再掃出變化點
#include <bits/stdc++.h>
using namespace std;
int h[10001];
int main(){
    int L, H, R, maxR = 0;
    while (scanf("%d %d %d", &L, &H, &R) == 3) {
        for (int x = L; x < R; x++) if (H > h[x]) h[x] = H;
        if (R > maxR) maxR = R;
    }
    vector<int> out;
    int prev = 0;
    for (int x = 0; x <= maxR; x++) {
        if (h[x] != prev) { out.push_back(x); out.push_back(h[x]); prev = h[x]; }
    }
    for (size_t i = 0; i < out.size(); i++) printf("%d%c", out[i], i+1<out.size()?' ':'\n');
    if (out.empty()) printf("\n");
    return 0;
}
