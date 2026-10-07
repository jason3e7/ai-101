// ZeroJudge s794 - 1A2B: 反推 4 位秘密數. 離線依第一提示猜測分組建 bucket, 再過濾. 緊湊版.
#include <bits/stdc++.h>
using namespace std;
static int D0[9000],D1[9000],D2[9000],D3[9000];
static int C[9000][10];
static int GD[9000][4], GC[9000][4], GN[9000];
// fast input
static char ibuf[1<<25]; int ipos=0, ilen=0;
inline int rdInt(){
    while(ipos<ilen && (ibuf[ipos]<'0'||ibuf[ipos]>'9')) ipos++;
    int x=0; while(ipos<ilen && ibuf[ipos]>='0'&&ibuf[ipos]<='9'){ x=x*10+(ibuf[ipos]-'0'); ipos++; } return x;
}
inline void rdHint(int&g,int&A,int&B){ // "gggg aAbB"
    g=rdInt(); // reads 4-digit
    // next: A then 'A' then B then 'B'
    A=rdInt(); B=rdInt();
}
int main(){
    ilen=fread(ibuf,1,sizeof(ibuf),stdin);
    for(int n=1000;n<=9999;n++){
        int idx=n-1000,x=n; int d3=x%10;x/=10;int d2=x%10;x/=10;int d1=x%10;x/=10;int d0=x%10;
        D0[idx]=d0;D1[idx]=d1;D2[idx]=d2;D3[idx]=d3;
        C[idx][d0]++;C[idx][d1]++;C[idx][d2]++;C[idx][d3]++;
    }
    for(int idx=0;idx<9000;idx++){ int k=0; for(int d=0;d<10;d++) if(C[idx][d]){ GD[idx][k]=d; GC[idx][k]=C[idx][d]; k++; } GN[idx]=k; }
    int T=rdInt();
    vector<int> off(T+1,0), Hg, HA, HB, g1of(T);
    vector<string> sig(T);
    Hg.reserve(T*2); HA.reserve(T*2); HB.reserve(T*2);
    for(int t=0;t<T;t++){
        int n=rdInt(); off[t+1]=off[t]+n;
        char sb[300]; int sp=0;
        for(int i=0;i<n;i++){ int g,A,B; rdHint(g,A,B); g-=1000;
            Hg.push_back(g); HA.push_back(A); HB.push_back(B);
            sp+=sprintf(sb+sp,"%d,%d,%d;",g,A,B); }
        g1of[t]=Hg[off[t]]; sig[t].assign(sb,sp);
    }
    vector<vector<int>> byG(9000);
    for(int t=0;t<T;t++) byG[g1of[t]].push_back(t);
    vector<string> res(T);
    static int candBuf[9000]; // not used
    vector<vector<int>> cand(40);
    for(int g=0; g<9000; g++){
        if(byG[g].empty()) continue;
        for(auto&v:cand) v.clear();
        const int *gd=GD[g], *gc=GC[g]; int gn=GN[g]; int g0=D0[g],g1=D1[g],g2=D2[g],g3=D3[g];
        for(int s=0;s<9000;s++){
            int A=(g0==D0[s])+(g1==D1[s])+(g2==D2[s])+(g3==D3[s]);
            const int*cs=C[s]; int com=0;
            for(int k=0;k<gn;k++){ int c=gc[k], e=cs[gd[k]]; com += c<e?c:e; }
            cand[A*8+(com-A)].push_back(s);
        }
        unordered_map<string,string> memo; memo.reserve(byG[g].size()*2);
        for(int t: byG[g]){
            auto mit=memo.find(sig[t]);
            if(mit!=memo.end()){ res[t]=mit->second; continue; }
            int b0=off[t], n=off[t+1]-off[t];
            vector<int>& L = cand[HA[b0]*8+HB[b0]];
            int cnt=0, found=-1;
            for(int s : L){
                bool ok=true;
                for(int i=1;i<n;i++){
                    int g2i=Hg[b0+i];
                    int A=(D0[g2i]==D0[s])+(D1[g2i]==D1[s])+(D2[g2i]==D2[s])+(D3[g2i]==D3[s]);
                    const int*gd2=GD[g2i], *gc2=GC[g2i]; int gn2=GN[g2i]; const int*cs=C[s]; int com=0;
                    for(int k=0;k<gn2;k++){ int c=gc2[k], e=cs[gd2[k]]; com += c<e?c:e; }
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
    fwrite(out.data(),1,out.size(),stdout);
    return 0;
}
