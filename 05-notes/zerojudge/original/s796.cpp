// ZeroJudge s796 - 蜂蜜工廠: 單位工作、區間[L,R]、一天一單, 最大化總酬勞 (區間點指派最大權重)
// matroid 貪心(酬勞大到小). canAdd: 由窗擴張可達區間判斷能否加入(正確,快).
// 能加入時: 先試快速左移鏈, 再右移鏈 (多數情況), 皆失敗才退回 Kuhn (僅罕見的混向路徑).
#include <bits/stdc++.h>
using namespace std;
const int INF = INT_MAX, NEG = INT_MIN;
int N, M, SZ;
vector<int> occ, jLs, jRs, segMinL, segMaxR, segFree, vis; int stamp=0;
inline void updMinL(int d,int v){ d+=SZ-1; segMinL[d]=v; for(d>>=1;d;d>>=1) segMinL[d]=min(segMinL[2*d],segMinL[2*d+1]); }
inline void updMaxR(int d,int v){ d+=SZ-1; segMaxR[d]=v; for(d>>=1;d;d>>=1) segMaxR[d]=max(segMaxR[2*d],segMaxR[2*d+1]); }
inline void updFree(int d,int v){ d+=SZ-1; segFree[d]=v; for(d>>=1;d;d>>=1) segFree[d]=max(segFree[2*d],segFree[2*d+1]); }
int qMinL(int nd,int ns,int ne,int lo,int hi){ if(hi<ns||ne<lo) return INF; if(lo<=ns&&ne<=hi) return segMinL[nd]; int md=(ns+ne)>>1; return min(qMinL(2*nd,ns,md,lo,hi),qMinL(2*nd+1,md+1,ne,lo,hi)); }
int qMaxR(int nd,int ns,int ne,int lo,int hi){ if(hi<ns||ne<lo) return NEG; if(lo<=ns&&ne<=hi) return segMaxR[nd]; int md=(ns+ne)>>1; return max(qMaxR(2*nd,ns,md,lo,hi),qMaxR(2*nd+1,md+1,ne,lo,hi)); }
int qAnyFree(int nd,int ns,int ne,int lo,int hi){ if(hi<ns||ne<lo||segFree[nd]==0) return -1; if(ns==ne) return ns; int md=(ns+ne)>>1; int l=qAnyFree(2*nd,ns,md,lo,hi); if(l!=-1)return l; return qAnyFree(2*nd+1,md+1,ne,lo,hi); }
int qRightFree(int nd,int ns,int ne,int lo,int hi){ if(hi<ns||ne<lo||segFree[nd]==0) return -1; if(ns==ne) return ns; int md=(ns+ne)>>1; int r=qRightFree(2*nd+1,md+1,ne,lo,hi); if(r!=-1)return r; return qRightFree(2*nd,ns,md,lo,hi); }
int qLeftFree(int nd,int ns,int ne,int lo,int hi){ if(hi<ns||ne<lo||segFree[nd]==0) return -1; if(ns==ne) return ns; int md=(ns+ne)>>1; int l=qLeftFree(2*nd,ns,md,lo,hi); if(l!=-1)return l; return qLeftFree(2*nd+1,md+1,ne,lo,hi); }
int qRightMinLE(int nd,int ns,int ne,int lo,int hi,int x){ if(hi<ns||ne<lo||segMinL[nd]>x) return -1; if(ns==ne) return ns; int md=(ns+ne)>>1; int r=qRightMinLE(2*nd+1,md+1,ne,lo,hi,x); if(r!=-1)return r; return qRightMinLE(2*nd,ns,md,lo,hi,x); }
int qLeftMaxGE(int nd,int ns,int ne,int lo,int hi,int x){ if(hi<ns||ne<lo||segMaxR[nd]<x) return -1; if(ns==ne) return ns; int md=(ns+ne)>>1; int l=qLeftMaxGE(2*nd,ns,md,lo,hi,x); if(l!=-1)return l; return qLeftMaxGE(2*nd+1,md+1,ne,lo,hi,x); }
inline void occupy(int d,int job){ occ[d]=job; updMinL(d,jLs[job]); updMaxR(d,jRs[job]); updFree(d,0); }
inline void vacate(int d){ occ[d]=-1; updMinL(d,INF); updMaxR(d,NEG); updFree(d,1); }
bool dfs(int i){
    int L=jLs[i], R=jRs[i];
    int f=qAnyFree(1,1,SZ,L,R);
    if(f!=-1){ occupy(f,i); return true; }
    for(int d=L; d<=R; d++){
        if(vis[d]==stamp) continue; vis[d]=stamp;
        int j=occ[d];
        occ[d]=i; updMinL(d,jLs[i]); updMaxR(d,jRs[i]);
        if(dfs(j)) return true;
        occ[d]=j; updMinL(d,jLs[j]); updMaxR(d,jRs[j]);
    }
    return false;
}
bool canAdd(int L,int R){
    int lo=L, hi=R;
    while(true){
        if(qAnyFree(1,1,SZ,lo,hi)!=-1) return true;
        int nlo=min(lo,qMinL(1,1,SZ,lo,hi)), nhi=max(hi,qMaxR(1,1,SZ,lo,hi));
        if(nlo==lo && nhi==hi) return false;
        lo=nlo; hi=nhi;
    }
}
int main(){
    if(scanf("%d %d",&N,&M)!=2) return 0;
    vector<array<long long,3>> jobs(M);
    for(int i=0;i<M;i++){ long long L,R,P; scanf("%lld %lld %lld",&L,&R,&P); jobs[i]={P,L,R}; }
    sort(jobs.begin(),jobs.end(),[](const array<long long,3>&a,const array<long long,3>&b){return a[0]>b[0];});
    occ.assign(N+1,-1); jLs.assign(M,0); jRs.assign(M,0);
    SZ=1; while(SZ<max(N,1)) SZ<<=1;
    segMinL.assign(2*SZ,INF); segMaxR.assign(2*SZ,NEG); segFree.assign(2*SZ,0); vis.assign(N+1,0);
    for(int d=1;d<=N;d++) updFree(d,1);
    long long total=0; int placed=0;
    for(int i=0;i<M && placed<N;i++){
        int L=(int)jobs[i][1], R=(int)jobs[i][2]; long long P=jobs[i][0];
        jLs[i]=L; jRs[i]=R;
        int fd=qRightFree(1,1,SZ,L,R);
        if(fd!=-1){ occupy(fd,i); total+=P; placed++; continue; }
        bool done=false;
        int f=qRightFree(1,1,SZ,1,L-1);
        if(f!=-1){ int ff=f;
            while(true){ int e=qRightMinLE(1,1,SZ,ff+1,R,ff); if(e==-1)break; int j=occ[e]; vacate(e); occupy(ff,j);
                if(e>=L){ occupy(e,i); done=true; break; } ff=e; } }
        if(!done){ int g=qLeftFree(1,1,SZ,R+1,N);
            if(g!=-1){ int gg=g;
                while(true){ int e=qLeftMaxGE(1,1,SZ,L,gg-1,gg); if(e==-1)break; int j=occ[e]; vacate(e); occupy(gg,j);
                    if(e<=R){ occupy(e,i); done=true; break; } gg=e; } } }
        if(!done){                       // 兩方向鏈皆失敗: 判斷是否可行, 可行才用 Kuhn (罕見混向)
            if(!canAdd(L,R)) continue;
            ++stamp; dfs(i);
        }
        total+=P; placed++;
    }
    printf("%lld\n", total);
    return 0;
}
