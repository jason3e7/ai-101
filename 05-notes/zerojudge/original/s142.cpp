// ZeroJudge s142 - 最大正方形: 全 1 最大正方形邊長 (DP)
#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,m; scanf("%d %d",&n,&m);
    vector<vector<int>> g(n+1, vector<int>(m+1,0)), dp(n+1, vector<int>(m+1,0));
    int best=0;
    for(int i=1;i<=n;i++) for(int j=1;j<=m;j++){
        scanf("%d",&g[i][j]);
        if(g[i][j]) { dp[i][j]=min({dp[i-1][j],dp[i][j-1],dp[i-1][j-1]})+1; best=max(best,dp[i][j]); }
    }
    printf("%d\n", best);
    return 0;
}
