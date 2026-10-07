// ZeroJudge s796 - 蜂蜜工廠: 單位工作、區間[L,R]、一天一單, 最大化總酬勞 (區間點指派最大權重)
// 依酬勞大到小的 matroid 貪心; 擴充路徑用「空閒天集合 set + 兩棵線段樹(占用日的 minL / maxR)」
// 沿增廣鏈每步 O(log): 左移(找能往左挪到空閒天的工作) 或 右移(對稱), 把空閒逐步推進窗內。
#include <bits/stdc++.h>
using namespace std;
const int INF = INT_MAX, NEG = INT_MIN;
int N, M;
vector<int> occ;               // day -> job id (-1)
vector<int> jLs, jRs;          // 排序後各工作窗
vector<int> segMinL, segMaxR;  // 線段樹 (1..N)
int SZ;
void updMinL(int d,int v){ d+=SZ-1; segMinL[d]=v; for(d>>=1;d;d>>=1) segMinL[d]=min(segMinL[2*d],segMinL[2*d+1]); }
void updMaxR(int d,int v){ d+=SZ-1; segMaxR[d]=v; for(d>>=1;d;d>>=1) segMaxR[d]=max(segMaxR[2*d],segMaxR[2*d+1]); }
// 區間 [lo,hi] 中, minL<=x 的最大 index; 無則 -1
int qRightMinLE(int node,int ns,int ne,int lo,int hi,int x){
    if(hi<ns||ne<lo||segMinL[node]>x) return -1;
    if(ns==ne) return ns;
    int mid=(ns+ne)/2;
    int r=qRightMinLE(2*node+1,mid+1,ne,lo,hi,x); if(r!=-1) return r;
    return qRightMinLE(2*node,ns,mid,lo,hi,x);
}
// 區間 [lo,hi] 中, maxR>=x 的最小 index; 無則 -1
int qLeftMaxGE(int node,int ns,int ne,int lo,int hi,int x){
    if(hi<ns||ne<lo||segMaxR[node]<x) return -1;
    if(ns==ne) return ns;
    int mid=(ns+ne)/2;
    int l=qLeftMaxGE(2*node,ns,mid,lo,hi,x); if(l!=-1) return l;
    return qLeftMaxGE(2*node+1,mid+1,ne,lo,hi,x);
}
set<int> freeDays;
void occupy(int d,int job){ occ[d]=job; freeDays.erase(d); updMinL(d,jLs[job]); updMaxR(d,jRs[job]); }
void vacate(int d){ occ[d]=-1; freeDays.insert(d); updMinL(d,INF); updMaxR(d,NEG); }
int main(){
    if(scanf("%d %d",&N,&M)!=2) return 0;
    vector<array<long long,3>> jobs(M);
    for(int i=0;i<M;i++){ long long L,R,P; scanf("%lld %lld %lld",&L,&R,&P); jobs[i]={P,L,R}; }
    sort(jobs.begin(),jobs.end(),[](const array<long long,3>&a,const array<long long,3>&b){return a[0]>b[0];});
    occ.assign(N+1,-1); jLs.assign(M,0); jRs.assign(M,0);
    SZ=1; while(SZ<N) SZ<<=1;
    segMinL.assign(2*SZ,INF); segMaxR.assign(2*SZ,NEG);
    for(int d=1;d<=N;d++) freeDays.insert(d);
    long long total=0; int placed=0;
    for(int i=0;i<M && placed<N;i++){
        int L=(int)jobs[i][1], R=(int)jobs[i][2]; long long P=jobs[i][0];
        jLs[i]=L; jRs[i]=R;
        // 直接: [L,R] 內任一空閒天
        auto it=freeDays.lower_bound(L);
        if(it!=freeDays.end() && *it<=R){ occupy(*it,i); total+=P; placed++; continue; }
        // 左向增廣: 空閒天 f < L, 把占用日工作往左挪到 f
        bool done=false;
        auto lit=freeDays.lower_bound(L);        // 第一個 >=L; 其前一個為 <L 的最大空閒天
        if(lit!=freeDays.begin()){
            int f=*prev(lit);
            while(true){
                int e=qRightMinLE(1,1,SZ,f+1,R,f);   // [f+1,R] 中能挪到 f (L_j<=f) 的最右占用日
                if(e==-1){ break; }
                int j=occ[e]; vacate(e); occupy(f,j);  // j: e -> f
                if(e>=L){ occupy(e,i); total+=P; placed++; done=true; break; }
                f=e;                                    // 空閒天推進到 e
            }
        }
        if(done) continue;
        // 右向增廣: 空閒天 g > R, 把占用日工作往右挪到 g
        auto git=freeDays.upper_bound(R);        // 第一個 >R
        if(git!=freeDays.end()){
            int g=*git;
            while(true){
                int e=qLeftMaxGE(1,1,SZ,L,g-1,g);     // [L,g-1] 中能挪到 g (R_j>=g) 的最左占用日
                if(e==-1){ break; }
                int j=occ[e]; vacate(e); occupy(g,j);  // j: e -> g
                if(e<=R){ occupy(e,i); total+=P; placed++; done=true; break; }
                g=e;
            }
        }
        // done or not
    }
    printf("%lld\n", total);
    return 0;
}
