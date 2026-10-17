// ZeroJudge h664 - 河內塔: 最佳解第 S 步移動的圓盤號 = S 的末尾 0 個數 + 1
#include <bits/stdc++.h>
using namespace std;
int main(){
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        long long N, S;
        scanf("%lld %lld", &N, &S);
        printf("%d\n", __builtin_ctzll((unsigned long long)S) + 1);
    }
    return 0;
}
