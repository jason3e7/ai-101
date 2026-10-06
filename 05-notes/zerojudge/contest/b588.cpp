// ZeroJudge b588 - 撿石頭遊戲 (3 堆, 可從1/2/3堆取等量正數, 最後取者勝)
// DP: win[x][y][z] = 有任一步走到 lose 狀態. 依座標遞增, 任何合法步都走到更小的已算狀態.
#include <iostream>
#include <algorithm>
using namespace std;

static bool win[101][101][101];

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    for (int x = 0; x <= 100; x++)
      for (int y = 0; y <= 100; y++)
        for (int z = 0; z <= 100; z++) {
            bool w = false;
            for (int k = 1; k <= x && !w; k++) if (!win[x-k][y][z]) w = true;
            for (int k = 1; k <= y && !w; k++) if (!win[x][y-k][z]) w = true;
            for (int k = 1; k <= z && !w; k++) if (!win[x][y][z-k]) w = true;
            int m;
            m = min(x, y); for (int k = 1; k <= m && !w; k++) if (!win[x-k][y-k][z]) w = true;
            m = min(x, z); for (int k = 1; k <= m && !w; k++) if (!win[x-k][y][z-k]) w = true;
            m = min(y, z); for (int k = 1; k <= m && !w; k++) if (!win[x][y-k][z-k]) w = true;
            m = min({x, y, z}); for (int k = 1; k <= m && !w; k++) if (!win[x-k][y-k][z-k]) w = true;
            win[x][y][z] = w;
        }
    int x, y, z;
    while (cin >> x && x != 0) { cin >> y >> z; cout << (win[x][y][z] ? "w" : "l") << "\n"; }
    return 0;
}
