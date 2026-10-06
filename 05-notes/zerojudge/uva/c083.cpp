// ZeroJudge c083 / UVa130 - Roman Roulette
// 殺第 k 人 -> 由被殺者左鄰算 k 得埋葬者, 埋葬者站到被殺者位置. 直到剩一人.
// 以 start=1 模擬求存活位置, 再用旋轉對稱回推要從誰算起才讓 1 存活.
#include <bits/stdc++.h>
using namespace std;
int nxt[105], prv_[105], person[105];
int main(){
    int n, k;
    while (scanf("%d %d", &n, &k) == 2 && (n || k)) {
        for (int i = 0; i < n; i++) { person[i] = i + 1; nxt[i] = (i+1)%n; prv_[i] = (i-1+n)%n; }
        int size = n, cur = 0; // 從 node0 (人 1) 開始算
        while (size > 1) {
            int kill = cur;
            for (int s = (k-1)%size; s > 0; s--) kill = nxt[kill]; // 第 k 人
            int aliveBury = size - 1;
            int bury = nxt[kill];                                   // 被殺者左鄰
            for (int s = (k-1)%aliveBury; s > 0; s--) bury = nxt[bury]; // 從不會碰到 kill
            person[kill] = person[bury];       // 埋葬者站到被殺者位置
            nxt[prv_[bury]] = nxt[bury];       // 移除埋葬者原位
            prv_[nxt[bury]] = prv_[bury];
            size--;
            cur = nxt[kill];                   // 下次從新站位者的左鄰算
        }
        int survivor = person[cur];
        int ans = (n - (survivor - 1)) % n + 1;
        printf("%d\n", ans);
    }
    return 0;
}
