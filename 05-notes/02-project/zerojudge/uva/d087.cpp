// ZeroJudge d087 / UVa107 - The Cat in the Hat
// H=(N+1)^K (原貓高), W=N^K (工作貓數). 求非工作貓數與總高度.
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll ipow(ll base, int e){ ll r=1; for(int i=0;i<e;i++) r*=base; return r; }
int main(){
    ll H, W;
    while (scanf("%lld %lld", &H, &W) == 2 && (H || W)) {
        int K = 0; ll N = 0;
        for (int k = 1; k <= 40 && !K; k++) {
            ll approx = (ll)llround(pow((double)W, 1.0/k));
            for (ll dn = -1; dn <= 1; dn++) {
                ll n = approx + dn;
                if (n < 1) continue;
                if (ipow(n, k) == W && ipow(n + 1, k) == H) { K = k; N = n; break; }
            }
        }
        // 非工作貓 = sum_{i=0}^{K-1} N^i
        ll nonwork;
        if (N == 1) nonwork = K;
        else nonwork = (ipow(N, K) - 1) / (N - 1);
        // 總高度 = sum_{i=0}^{K} N^i * (N+1)^(K-i)
        ll sumH = 0;
        for (int i = 0; i <= K; i++) sumH += ipow(N, i) * ipow(N + 1, K - i);
        printf("%lld %lld\n", nonwork, sumH);
    }
    return 0;
}
