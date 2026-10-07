// ZeroJudge a055 / POJ2182 - Lost Cows: 由「前方較小者個數」還原排列
// 由後往前, 第 i 頭牌 = 第 (a[i]+1) 小的未用號碼 (Fenwick + 倍增找第 k 小)
#include <bits/stdc++.h>
using namespace std;
int N;
vector<int> bit_;
void upd(int i, int v){ for(; i<=N; i+=i&-i) bit_[i]+=v; }
int kth(int k){ // 找前綴和第一次達到 k 的位置
    int pos=0, LOG=1; while((1<<LOG)<=N) LOG++;
    for(int b=LOG; b>=0; b--){ int np=pos+(1<<b); if(np<=N && bit_[np]<k){ pos=np; k-=bit_[np]; } }
    return pos+1;
}
int main(){
    scanf("%d", &N);
    vector<int> a(N+1), brand(N+1);
    a[1]=0;
    for(int i=2;i<=N;i++) scanf("%d",&a[i]);
    bit_.assign(N+1,0);
    for(int i=1;i<=N;i++) upd(i,1);
    for(int i=N;i>=1;i--){ int x=kth(a[i]+1); brand[i]=x; upd(x,-1); }
    for(int i=1;i<=N;i++) printf("%d\n", brand[i]);
    return 0;
}
