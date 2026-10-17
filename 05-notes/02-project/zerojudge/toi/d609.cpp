// ZeroJudge d609 - Necklace (USACO Beads): 環形項鍊斷一處, 兩端各收同色(w 可當任意色), 求最多
#include <bits/stdc++.h>
using namespace std;
string s; int N;
int collect(int start, int dir){
    int cnt = 0, color = 0;
    for (int step = 0; step < N; step++) {
        char ch = s[((start + dir*step) % N + N) % N];
        if (ch == 'w') cnt++;
        else if (color == 0) { color = ch; cnt++; }
        else if (ch == color) cnt++;
        else break;
    }
    return cnt;
}
int main(){
    while (cin >> N >> s) {
        int best = 0;
        for (int i = 0; i < N; i++) {
            int a = collect(i, +1);        // 從 i 往右
            int b = collect((i - 1 + N) % N, -1); // 從 i-1 往左
            best = max(best, min(N, a + b));
        }
        cout << best << "\n";
    }
    return 0;
}
