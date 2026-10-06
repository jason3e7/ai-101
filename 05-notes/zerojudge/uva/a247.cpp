// ZeroJudge a247 / UVa106 - Fermat vs. Pythagoras
// 第一數: 互質(primitive)畢氏三元組數 (z<=N)
// 第二數: [1,N] 內不屬於任何三元組(可非互質, 但三邊都<=N)的整數個數
// 關鍵: 一個數 i 的「最小斜邊」minHyp[i] >= i; i 在 N 內被用到 <=> minHyp[i]<=N
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 1000000;
static int minHyp[MAXN + 1];   // i 所屬三元組中最小的斜邊; 0 = 從未出現
static int primPref[MAXN + 1]; // 以 c 為斜邊的 primitive 三元組數, 之後轉前綴
int gcd_(int a,int b){ while(b){int t=a%b;a=b;b=t;} return a; }
inline void upd(int i, int c){ if (!minHyp[i] || c < minHyp[i]) minHyp[i] = c; }
int main(){
    for (int m = 2; (long long)m*m <= MAXN; m++) {
        for (int n = 1; n < m; n++) {
            if (((m ^ n) & 1) == 0) continue;
            if (gcd_(m, n) != 1) continue;
            long long c = (long long)m*m + (long long)n*n;
            if (c > MAXN) break;
            int a = m*m - n*n, b = 2*m*n, cc = (int)c;
            primPref[cc]++;
            for (int k = 1; (long long)k*cc <= MAXN; k++) {
                int kc = k*cc;
                upd(k*a, kc); upd(k*b, kc); upd(kc, kc);
            }
        }
    }
    // minHyp[i] >= i, 所以「被用到(minHyp<=N)的數字個數」= 對 minHyp 值做前綴
    static int byVal[MAXN + 2];
    for (int i = 1; i <= MAXN; i++) if (minHyp[i]) byVal[minHyp[i]]++;
    for (int i = 1; i <= MAXN; i++) { byVal[i] += byVal[i-1]; primPref[i] += primPref[i-1]; }
    int N;
    while (scanf("%d", &N) == 1)
        printf("%d %d\n", primPref[N], N - byVal[N]);
    return 0;
}
