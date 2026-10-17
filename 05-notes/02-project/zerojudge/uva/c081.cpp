// ZeroJudge c081 / UVa102 - Ecological Bin Packing
// 3 桶 3 色, 枚舉 6 種顏色指派, 最小搬移; 平手取字典序最小排列字串
#include <bits/stdc++.h>
using namespace std;
int idx(char c){ return c=='B'?0 : c=='G'?1 : 2; } // B,G,C
int main(){
    // 6 種排列, 已按字典序排好
    const char* perm[6] = {"BCG","BGC","CBG","CGB","GBC","GCB"};
    long long bin[3][3];
    while (scanf("%lld %lld %lld %lld %lld %lld %lld %lld %lld",
        &bin[0][0],&bin[0][1],&bin[0][2],
        &bin[1][0],&bin[1][1],&bin[1][2],
        &bin[2][0],&bin[2][1],&bin[2][2]) == 9) {
        long long total = 0;
        for (int i=0;i<3;i++) for(int j=0;j<3;j++) total += bin[i][j];
        long long best = LLONG_MAX; const char* bestP = perm[0];
        for (int p=0;p<6;p++){
            long long keep = 0;
            for (int i=0;i<3;i++) keep += bin[i][ idx(perm[p][i]) ];
            long long moves = total - keep;
            if (moves < best){ best = moves; bestP = perm[p]; } // 字典序已排, 嚴格小才換
        }
        printf("%s %lld\n", bestP, best);
    }
    return 0;
}
