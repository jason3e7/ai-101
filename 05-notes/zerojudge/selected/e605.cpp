// ZeroJudge e605 (UVa 10189) - Minesweeper: 填入每格周圍地雷數. 測資間空行.
#include <cstdio>
#include <vector>
#include <string>
using namespace std;
int main(){
    int n,m,k=0;
    bool first=true;
    while(scanf("%d %d",&n,&m)==2 && (n||m)){
        vector<string> g(n);
        for(int i=0;i<n;i++){ char buf[128]; scanf("%s",buf); g[i]=buf; }
        if(!first) printf("\n");
        first=false;
        printf("Field #%d:\n", ++k);
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                if(g[i][j]=='*'){ putchar('*'); continue; }
                int c=0;
                for(int di=-1;di<=1;di++)for(int dj=-1;dj<=1;dj++){
                    if(!di&&!dj) continue;
                    int ni=i+di,nj=j+dj;
                    if(ni>=0&&ni<n&&nj>=0&&nj<m&&g[ni][nj]=='*') c++;
                }
                putchar('0'+c);
            }
            putchar('\n');
        }
    }
    return 0;
}
