// ZeroJudge c082 / UVa118 - Mutant Flatworld Explorers
// 機器人在格子世界 L/R/F; 掉出界留 scent, 後續機器人在 scent 格忽略致命指令
#include <bits/stdc++.h>
using namespace std;
int main(){
    int maxX, maxY;
    scanf("%d %d", &maxX, &maxY);
    static bool scent[55][55] = {};
    const int dx[4] = {0, 1, 0, -1};  // N,E,S,W
    const int dy[4] = {1, 0, -1, 0};
    const char* DIR = "NESW";
    int x, y; char d;
    char cmd[256];
    while (scanf("%d %d %c", &x, &y, &d) == 3) {
        scanf("%s", cmd);
        int dir = (d=='N'?0 : d=='E'?1 : d=='S'?2 : 3);
        bool lost = false;
        for (int i = 0; cmd[i] && !lost; i++) {
            if (cmd[i] == 'L') dir = (dir + 3) % 4;
            else if (cmd[i] == 'R') dir = (dir + 1) % 4;
            else { // F
                int nx = x + dx[dir], ny = y + dy[dir];
                if (nx < 0 || ny < 0 || nx > maxX || ny > maxY) {
                    if (scent[x][y]) continue;      // 已有標記, 忽略
                    scent[x][y] = true; lost = true; // 掉下去
                } else { x = nx; y = ny; }
            }
        }
        printf("%d %d %c%s\n", x, y, DIR[dir], lost ? " LOST" : "");
    }
    return 0;
}
