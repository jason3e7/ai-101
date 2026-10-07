// ZeroJudge s796 - 蜂蜜工廠: 每訂單區間[L,R]佔1天, 1天1單, 最大化選中訂單酬勞
// 貪心: 酬勞大到小, 每單指派 <=R 的最晚空閒天(>=L); DSU 指向往下最近空閒天
#include <bits/stdc++.h>
using namespace std;
int N, M;
vector<int> par;
int find_(int x){ while(par[x]!=x){ par[x]=par[par[x]]; x=par[x]; } return x; }
int main(){
    scanf("%d %d",&N,&M);
    vector<array<long long,3>> job(M); // {P, L, R}
    for (int i=0;i<M;i++){ long long L,R,P; scanf("%lld %lld %lld",&L,&R,&P); job[i]={P,L,R}; }
    sort(job.begin(), job.end(), [](auto&a,auto&b){ return a[0]>b[0]; });
    par.resize(N+1); for(int d=0;d<=N;d++) par[d]=d; // 0 = 無空閒
    long long ans=0;
    for (auto& j : job) {
        long long P=j[0], L=j[1], R=j[2];
        int d = find_((int)R);        // <=R 的最晚空閒天
        if (d >= L) { ans += P; par[d] = d-1; } // 佔用, 指向前一天
    }
    printf("%lld\n", ans);
    return 0;
}
