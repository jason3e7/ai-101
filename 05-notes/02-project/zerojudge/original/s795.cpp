// ZeroJudge s795 - 森林巡守站: 樹的最小權重支配集 (每點被自身或相鄰點的巡守站覆蓋)
// 樹 DP 3 狀態: 0=選自己, 1=未選但被某子覆蓋, 2=未選且尚未覆蓋(須靠父親)
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF = 1e18;
int main(){
    int N; scanf("%d",&N);
    vector<ll> C(N+1);
    for (int i=1;i<=N;i++) scanf("%lld",&C[i]);
    vector<vector<int>> adj(N+1);
    for (int i=0;i<N-1;i++){ int u,v; scanf("%d %d",&u,&v); adj[u].push_back(v); adj[v].push_back(u); }
    // BFS 求順序與父親
    vector<int> par(N+1,0), order; order.reserve(N);
    vector<char> vis(N+1,0);
    { queue<int> q; q.push(1); vis[1]=1; par[1]=0;
      while(!q.empty()){ int u=q.front();q.pop(); order.push_back(u);
        for(int w:adj[u]) if(!vis[w]){ vis[w]=1; par[w]=u; q.push(w); } } }
    vector<array<ll,3>> dp(N+1);
    for (int i=N-1;i>=0;i--) {
        int v = order[i];
        ll d0 = C[v], d2 = 0, d1 = 0;
        bool hasChild=false; bool some0=false; ll bestExtra=INF;
        for (int c : adj[v]) if (c != par[v]) {
            hasChild=true;
            d0 += min({dp[c][0],dp[c][1],dp[c][2]});
            ll mn = min(dp[c][0], dp[c][1]);
            d2 += mn;
            if (dp[c][0] <= dp[c][1]) some0=true;
            else bestExtra = min(bestExtra, dp[c][0]-dp[c][1]);
        }
        if (!hasChild) { d1 = INF; d2 = 0; }
        else { d1 = d2 + (some0 ? 0 : bestExtra); }
        dp[v] = {d0, d1, d2};
    }
    printf("%lld\n", min(dp[1][0], dp[1][1]));
    return 0;
}
