// ZeroJudge s111 - Bitonic Path: 最短雙調環路 (去程 x 遞增, 回程 x 遞減), O(n^2) DP
#include <bits/stdc++.h>
using namespace std;
int n;
vector<double> X, Y;
double dist(int i,int j){ double dx=X[i]-X[j], dy=Y[i]-Y[j]; return sqrt(dx*dx+dy*dy); }
int main(){
    scanf("%d",&n);
    X.resize(n+1); Y.resize(n+1);
    for(int i=1;i<=n;i++) scanf("%lf %lf",&X[i],&Y[i]);
    if(n==1){ printf("0.00\n"); return 0; }
    vector<vector<double>> dp(n+1, vector<double>(n+1, 0));
    dp[1][2]=dist(1,2);
    for(int j=3;j<=n;j++){
        for(int i=1;i<j;i++){
            if(i<j-1) dp[i][j]=dp[i][j-1]+dist(j-1,j);
            else { double best=1e18; for(int k=1;k<i;k++) best=min(best, dp[k][i]+dist(k,j)); dp[i][j]=best; }
        }
    }
    printf("%.2f\n", dp[n-1][n]+dist(n-1,n));
    return 0;
}
