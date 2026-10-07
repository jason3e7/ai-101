// ZeroJudge c249 - 穩定湮滅配對 = 穩定婚姻 (Gale-Shapley), |粒子提親
#include <bits/stdc++.h>
using namespace std;
int main(){
    int T; if(scanf("%d",&T)!=1) return 0;
    while(T--){
        int N; scanf("%d",&N);
        vector<vector<int>> pref(N+1, vector<int>(N)); // |粒子 i 的偏好 (O 編號, 高到低)
        for(int i=1;i<=N;i++) for(int j=0;j<N;j++) scanf("%d",&pref[i][j]);
        vector<vector<int>> orank(N+1, vector<int>(N+1,0)); // O 粒子 j 對 |粒子 的排名 (小=偏好)
        for(int j=1;j<=N;j++) for(int k=0;k<N;k++){ int ip; scanf("%d",&ip); orank[j][ip]=k; }
        vector<int> next_(N+1,0);         // |粒子 i 下個要提親的偏好索引
        vector<int> matchO(N+1,0);        // O 粒子 j 目前配對的 |粒子 (0=空)
        vector<int> matchI(N+1,0);        // |粒子 i 目前配對的 O 粒子
        queue<int> free_;
        for(int i=1;i<=N;i++) free_.push(i);
        while(!free_.empty()){
            int i=free_.front();
            if(next_[i]>=N){ free_.pop(); continue; } // 理論上不會發生 (完整偏好)
            int o=pref[i][next_[i]++];
            if(matchO[o]==0){ matchO[o]=i; matchI[i]=o; free_.pop(); }
            else {
                int cur=matchO[o];
                if(orank[o][i] < orank[o][cur]){ // o 較喜歡 i
                    matchO[o]=i; matchI[i]=o; free_.pop();
                    matchI[cur]=0; free_.push(cur);
                } // 否則 i 繼續提親 (留在佇列)
            }
        }
        for(int i=1;i<=N;i++) printf("%d %d\n", i, matchI[i]);
    }
    return 0;
}
