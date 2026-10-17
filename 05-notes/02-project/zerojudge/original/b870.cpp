// ZeroJudge b870 - Wickerbottom: K 個滅火器放整數點, 最小化「樹到最近滅火器」距離的最大值
// 二分搜距離 D, 貪心: 排序後每次把滅火器放在 (未覆蓋最左樹 + D), 覆蓋到 +D
#include <bits/stdc++.h>
using namespace std;
int main(){
    int T; if (scanf("%d",&T)!=1) return 0;
    while (T--) {
        long long n, k; scanf("%lld %lld",&n,&k);
        vector<long long> a(n);
        for (auto& x : a) scanf("%lld",&x);
        sort(a.begin(), a.end());
        auto ok = [&](long long D)->bool{
            long long used = 0, i = 0;
            while (i < n) {
                long long cover = a[i] + D;      // 滅火器放在 a[i]+D (整數點)
                used++;
                while (i < n && a[i] <= cover + D) i++;  // 覆蓋到 cover+D
                if (used > k) return false;
            }
            return used <= k;
        };
        long long lo = 0, hi = 2000000000LL;
        while (lo < hi) { long long m=(lo+hi)/2; if (ok(m)) hi=m; else lo=m+1; }
        printf("%lld\n", lo);
    }
    return 0;
}
