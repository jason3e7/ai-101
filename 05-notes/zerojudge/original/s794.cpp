// ZeroJudge s794 - 1A2B. 關鍵: 某猜測各 (A,B) 桶的「大小」只取決於其數字重數結構(可由數字重標號對稱得證),
// 故可用小表 O(1) 查任一提示的候選數量, 挑「最小桶」的提示當基準(免額外掃描), 只對基準猜測建完整 bucket。
#include <bits/stdc++.h>
using namespace std;
static int D0[9000],D1[9000],D2[9000],D3[9000];
static int C[9000][10];
static int GD[9000][4], GC[9000][4], GN[9000];
static int STRUCT[9000];           // 0..4
static int SZTAB[5][40];           // 結構 -> code -> 數量
static char ibuf[1<<25]; int ipos=0, ilen=0;
inline int rdInt(){ while(ipos<ilen && (ibuf[ipos]<'0'||ibuf[ipos]>'9')) ipos++; int x=0;
    while(ipos<ilen && ibuf[ipos]>='0'&&ibuf[ipos]<='9'){ x=x*10+(ibuf[ipos]-'0'); ipos++; } return x; }
inline int codeOf(int A,int B){ return A*8+B; }
inline void outc(int g,int s,int&A,int&com){
    A=(D0[g]==D0[s])+(D1[g]==D1[s])+(D2[g]==D2[s])+(D3[g]==D3[s]);
    const int*cs=C[s]; const int*gd=GD[g]; const int*gc=GC[g]; int gn=GN[g]; com=0;
    for(int k=0;k<gn;k++){ int c=gc[k], e=cs[gd[k]]; com += c<e?c:e; }
}
int structId(int g){ // 由 g 的數字重數 pattern 判結構
    int m[4]={0,0,0,0}, k=GN[g]; for(int i=0;i<k;i++) m[i]=GC[g][i];
    sort(m,m+4,greater<int>());
    if(m[0]==4) return 0;              // [4]
    if(m[0]==3) return 1;              // [3,1]
    if(m[0]==2&&m[1]==2) return 2;     // [2,2]
    if(m[0]==2) return 3;              // [2,1,1]
    return 4;                          // [1,1,1,1]
}
int main(){
    ilen=fread(ibuf,1,sizeof(ibuf),stdin);
    for(int n=1000;n<=9999;n++){ int idx=n-1000,x=n; int d3=x%10;x/=10;int d2=x%10;x/=10;int d1=x%10;x/=10;int d0=x%10;
        D0[idx]=d0;D1[idx]=d1;D2[idx]=d2;D3[idx]=d3; C[idx][d0]++;C[idx][d1]++;C[idx][d2]++;C[idx][d3]++; }
    for(int idx=0;idx<9000;idx++){ int k=0; for(int d=0;d<10;d++) if(C[idx][d]){ GD[idx][k]=d; GC[idx][k]=C[idx][d]; k++; } GN[idx]=k; }
    for(int g=0;g<9000;g++) STRUCT[g]=structId(g);
    // 建結構->code 數量表: 每結構取一代表猜測掃一次
    int rep[5]={1111-1000,1112-1000,1122-1000,1123-1000,1234-1000};
    for(int st=0;st<5;st++){ int g=rep[st]; for(int s=0;s<9000;s++){ int A,com; outc(g,s,A,com); SZTAB[st][codeOf(A,com-A)]++; } }
    int T=rdInt();
    vector<int> off(T+1,0), Hg, HA, HB;
    vector<string> sig(T);
    Hg.reserve(T*2); HA.reserve(T*2); HB.reserve(T*2);
    for(int t=0;t<T;t++){
        int n=rdInt(); off[t+1]=off[t]+n;
        char sb[300]; int sp=0;
        for(int i=0;i<n;i++){ int g=rdInt()-1000; int A=rdInt(), B=rdInt();
            Hg.push_back(g); HA.push_back(A); HB.push_back(B); sp+=sprintf(sb+sp,"%d,%d,%d;",g,A,B); }
        sig[t].assign(sb,sp);
    }
    vector<int> baseG(T), baseI(T);
    for(int t=0;t<T;t++){ int b0=off[t], n=off[t+1]-off[t]; int best=INT_MAX, bi=b0;
        for(int i=0;i<n;i++){ int h=b0+i; int s=SZTAB[STRUCT[Hg[h]]][codeOf(HA[h],HB[h])]; if(s<best){best=s; bi=h;} }
        baseI[t]=bi; baseG[t]=Hg[bi]; }
    vector<vector<int>> byG(9000);
    for(int t=0;t<T;t++) byG[baseG[t]].push_back(t);
    vector<string> res(T);
    vector<vector<int>> cand(40);
    for(int g=0; g<9000; g++){ if(byG[g].empty()) continue;
        for(auto&v:cand) v.clear();
        for(int s=0;s<9000;s++){ int A,com; outc(g,s,A,com); cand[codeOf(A,com-A)].push_back(s); }
        unordered_map<string,string> memo; memo.reserve(byG[g].size()*2);
        for(int t: byG[g]){
            auto mit=memo.find(sig[t]); if(mit!=memo.end()){ res[t]=mit->second; continue; }
            int b0=off[t], n=off[t+1]-off[t], bi=baseI[t];
            vector<int>& L = cand[codeOf(HA[bi],HB[bi])];
            int cnt=0, found=-1;
            for(int s : L){ bool ok=true;
                for(int i=0;i<n;i++){ int hidx=b0+i; if(hidx==bi) continue; int gg=Hg[hidx];
                    int A,com; outc(gg,s,A,com);
                    if(A!=HA[hidx] || com-A!=HB[hidx]){ ok=false; break; } }
                if(ok){ cnt++; found=s; if(cnt>=2) break; } }
            if(cnt==0) res[t]="Impossible"; else if(cnt>=2) res[t]="Not Sure";
            else { char tt[8]; sprintf(tt,"%04d",found+1000); res[t]=tt; }
            memo.emplace(sig[t], res[t]);
        }
    }
    string out; out.reserve(T*6); for(int t=0;t<T;t++){ out+=res[t]; out+='\n'; }
    fwrite(out.data(),1,out.size(),stdout); return 0;
}
