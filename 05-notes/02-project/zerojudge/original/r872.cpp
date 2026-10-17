// ZeroJudge r872 - 桿: 選底座 A_j 後反覆乘 k(>=2), 問做出長度 L 的方法數
// f(n)=有序因數分解數(每因數>=2), f(1)=1; ans[L]=sum_{A|L, A在集合} f(L/A)
#include <bits/stdc++.h>
using namespace std;
const int MX = 100000;
long long f[MX+1], ans_[MX+1];
int main(){
    int N; scanf("%d",&N);
    vector<char> present(MX+1,0);
    for (int i=0;i<N;i++){ int a; scanf("%d",&a); present[a]=1; }
    f[1]=1;
    for (int x=1;x<=MX;x++) if (f[x]) for (long long m=2*x; m<=MX; m+=x) f[m]+=f[x];
    for (int A=1;A<=MX;A++) if (present[A]) for (int L=A; L<=MX; L+=A) ans_[L]+=f[L/A];
    int Q; scanf("%d",&Q);
    for (int i=0;i<Q;i++){ int L; scanf("%d",&L); printf("%lld%c", ans_[L], i+1<Q?' ':'\n'); }
    return 0;
}
