// ZeroJudge s794 - 1A2B: 由提示反推 4 位秘密數. T 可達 1e5, 不能每測資掃 9000。
// 離線: 依「第一個提示的猜測」分組; 每個猜測只建一次 bucket(outcome -> 候選清單),
// 每測資從小清單出發, 再用其餘提示過濾 (命中 2 個即停, 只需分辨 0/1/>=2)。
#include <bits/stdc++.h>
using namespace std;
static int D[9000][4];
static int C[9000][10];
static int GD[9000][4]; static int GC[9000][4]; static int GN[9000];
inline int code(int A,int B){ return A*8+B; }
int main(){
    for(int n=1000;n<=9999;n++){
        int idx=n-1000,x=n; int d3=x%10;x/=10;int d2=x%10;x/=10;int d1=x%10;x/=10;int d0=x%10;
        D[idx][0]=d0;D[idx][1]=d1;D[idx][2]=d2;D[idx][3]=d3;
        C[idx][d0]++;C[idx][d1]++;C[idx][d2]++;C[idx][d3]++;
    }
    for(int idx=0;idx<9000;idx++){ int k=0; for(int d=0;d<10;d++) if(C[idx][d]){ GD[idx][k]=d; GC[idx][k]=C[idx][d]; k++; } GN[idx]=k; }
    int T; if(scanf("%d",&T)!=1) return 0;
    vector<int> off(T+1,0), Hg, HA, HB;
    vector<int> g1of(T);
    vector<string> sig(T);
    Hg.reserve(T*2); HA.reserve(T*2); HB.reserve(T*2);
    char buf[16],hb[16];
    for(int t=0;t<T;t++){
        int n; scanf("%d",&n);
        off[t+1]=off[t]+n;
        char sb[256]; int sp=0;
        for(int i=0;i<n;i++){ scanf("%s %s",buf,hb); int g=atoi(buf)-1000;
            Hg.push_back(g); HA.push_back(hb[0]-'0'); HB.push_back(hb[2]-'0');
            sp+=sprintf(sb+sp,"%d:%c%c|",g,hb[0],hb[2]); }
        g1of[t]=Hg[off[t]]; sig[t].assign(sb,sp);
    }
    // 依 g1 分組
    vector<vector<int>> byG(9000);
    for(int t=0;t<T;t++) byG[g1of[t]].push_back(t);
    vector<string> res(T);
    vector<vector<int>> cand(40);
    auto outc=[&](int g,int s,int&A,int&common){
        A=(D[g][0]==D[s][0])+(D[g][1]==D[s][1])+(D[g][2]==D[s][2])+(D[g][3]==D[s][3]);
        const int*cs=C[s]; const int*gd=GD[g]; const int*gc=GC[g]; int gn=GN[g]; common=0;
        for(int k=0;k<gn;k++){ int c=gc[k], e=cs[gd[k]]; common += c<e?c:e; }
    };
    for(int g=0; g<9000; g++){
        if(byG[g].empty()) continue;
        for(auto&v:cand) v.clear();
        for(int s=0;s<9000;s++){ int A,com; outc(g,s,A,com); int B=com-A; cand[code(A,B)].push_back(s); }
        unordered_map<string,string> memo;
        for(int t: byG[g]){
            auto mit=memo.find(sig[t]);
            if(mit!=memo.end()){ res[t]=mit->second; continue; }
            int b0=off[t], n=off[t+1]-off[t];
            int A1=HA[b0], B1=HB[b0];
            vector<int>& L = cand[code(A1,B1)];
            int cnt=0, found=-1;
            for(int s : L){
                bool ok=true;
                for(int i=1;i<n;i++){
                    int g2=Hg[b0+i], A,com; outc(g2,s,A,com);
                    if(A!=HA[b0+i] || com-A!=HB[b0+i]){ ok=false; break; }
                }
                if(ok){ cnt++; found=s; if(cnt>=2) break; }
            }
            if(cnt==0) res[t]="Impossible";
            else if(cnt>=2) res[t]="Not Sure";
            else { char tt[8]; sprintf(tt,"%04d",found+1000); res[t]=tt; }
            memo.emplace(sig[t], res[t]);
        }
    }
    string out; out.reserve(T*6);
    for(int t=0;t<T;t++){ out+=res[t]; out+='\n'; }
    fputs(out.c_str(), stdout);
    return 0;
}
