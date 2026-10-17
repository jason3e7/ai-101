// ZeroJudge c073 / UVa101 - The Blocks Problem
// 機器手臂搬積木模擬: move/pile a onto/over b
#include <bits/stdc++.h>
using namespace std;
int n;
vector<int> pile_[30];
int pos_[30]; // block -> pile index

void locate(int a, int &p, int &h) {
    p = pos_[a];
    for (h = 0; h < (int)pile_[p].size(); h++)
        if (pile_[p][h] == a) return;
}
// 把 p 堆中高度 h 以上(不含 h)的積木歸位
void returnAbove(int p, int h) {
    for (int i = h + 1; i < (int)pile_[p].size(); i++) {
        int b = pile_[p][i];
        pile_[b].push_back(b);
        pos_[b] = b;
    }
    pile_[p].resize(h + 1);
}
int main() {
    while (scanf("%d", &n) == 1) {
        for (int i = 0; i < n; i++) { pile_[i].clear(); pile_[i].push_back(i); pos_[i] = i; }
        char op[10], prep[10];
        int a, b;
        while (scanf("%s", op) == 1) {
            if (op[0] == 'q') break;
            scanf("%d %s %d", &a, prep, &b);
            int pa, ha, pb, hb;
            locate(a, pa, ha); locate(b, pb, hb);
            if (pa == pb) continue; // 同堆(含 a==b) 不合法
            bool onto = (prep[1] == 'n'); // onto vs over
            bool moveOne = (op[0] == 'm'); // move vs pile
            if (moveOne) returnAbove(pa, ha); // move: 先歸位 a 上方
            if (onto) returnAbove(pb, hb);     // onto: 歸位 b 上方
            // 把 pa 堆 ha..end 這段搬到 pb 頂端
            for (int i = ha; i < (int)pile_[pa].size(); i++) {
                int blk = pile_[pa][i];
                pile_[pb].push_back(blk);
                pos_[blk] = pb;
            }
            pile_[pa].resize(ha);
        }
        for (int i = 0; i < n; i++) {
            printf("%d:", i);
            for (int x : pile_[i]) printf(" %d", x);
            printf("\n");
        }
    }
    return 0;
}
