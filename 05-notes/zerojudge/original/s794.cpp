// ZeroJudge s794 - 1A2B: 由 N 個提示反推 4 位秘密數(1000-9999); 枚舉所有候選驗證
#include <bits/stdc++.h>
using namespace std;
void digs(int x, int d[4]){ d[3]=x%10;x/=10;d[2]=x%10;x/=10;d[1]=x%10;x/=10;d[0]=x%10; }
int main(){
    int T; if(scanf("%d",&T)!=1) return 0;
    while (T--) {
        int n; scanf("%d",&n);
        vector<int> gd(n); vector<int> gA(n), gB(n);
        for (int i=0;i<n;i++){ char buf[16]; scanf("%s",buf); int g=atoi(buf); char h[16]; scanf("%s",h);
            // h like "2A2B"
            gd[i]=g; gA[i]=h[0]-'0'; gB[i]=h[2]-'0'; }
        int found=-1; int cnt=0;
        for (int s=1000; s<=9999; s++) {
            int sd[4]; digs(s,sd);
            int sc[10]={0}; for(int t=0;t<4;t++) sc[sd[t]]++;
            bool okAll=true;
            for (int i=0;i<n && okAll;i++) {
                int gdd[4]; digs(gd[i],gdd);
                int A=0; for(int t=0;t<4;t++) if(sd[t]==gdd[t]) A++;
                int gc[10]={0}; for(int t=0;t<4;t++) gc[gdd[t]]++;
                int common=0; for(int d=0;d<10;d++) common+=min(sc[d],gc[d]);
                int B=common-A;
                if (A!=gA[i] || B!=gB[i]) okAll=false;
            }
            if (okAll) { cnt++; found=s; if(cnt>1) break; }
        }
        if (cnt==0) printf("Impossible\n");
        else if (cnt>1) printf("Not Sure\n");
        else printf("%04d\n", found);
    }
    return 0;
}
