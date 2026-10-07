// ZeroJudge d760 (UVa 10330) - Power Transmission: 有節點容量的最大流
// 拆點: 每個變電裝置 v -> in_v(=v) 與 out_v(=N+v), 中間邊容量=裝置容量.
// 電線 (i,j,C): out_i -> in_j 容量 C (有向). 超級源 S->in_b(INF, 連發電廠者),
// out_d->超級匯 T(INF, 連 Dhaka 者). 求 S->T 最大流. 多組測資讀到 EOF.
#include <bits/stdc++.h>
using namespace std;
const int INF=1e9;
struct Dinic{
    struct E{int to,cap,rev;};
    vector<vector<E>> g; vector<int> level,it; int n;
    void init(int n_){ n=n_; g.assign(n+1,{}); }
    void add(int u,int v,int c){ g[u].push_back({v,c,(int)g[v].size()}); g[v].push_back({u,0,(int)g[u].size()-1}); }
    bool bfs(int s,int t){ level.assign(n+1,-1); queue<int>q; level[s]=0; q.push(s);
        while(!q.empty()){int u=q.front();q.pop(); for(auto&e:g[u]) if(e.cap>0&&level[e.to]<0){level[e.to]=level[u]+1;q.push(e.to);} }
        return level[t]>=0; }
    int dfs(int u,int t,int f){ if(u==t) return f; for(int&i=it[u];i<(int)g[u].size();i++){ E&e=g[u][i];
        if(e.cap>0&&level[e.to]==level[u]+1){ int d=dfs(e.to,t,min(f,e.cap)); if(d>0){e.cap-=d; g[e.to][e.rev].cap+=d; return d;} } } return 0; }
    long long maxflow(int s,int t){ long long fl=0; while(bfs(s,t)){ it.assign(n+1,0); int f; while((f=dfs(s,t,INF))>0) fl+=f; } return fl; }
};
int main(){
    int N;
    while(scanf("%d",&N)==1){
        vector<int> cap(N+1);
        for(int i=1;i<=N;i++) scanf("%d",&cap[i]);
        int S=2*N+1, T=2*N+2;
        Dinic d; d.init(2*N+2);
        for(int i=1;i<=N;i++) d.add(i, N+i, cap[i]);   // in->out = 節點容量
        int M; scanf("%d",&M);
        for(int e=0;e<M;e++){ int i,j,c; scanf("%d %d %d",&i,&j,&c); d.add(N+i, j, c); } // out_i->in_j
        int B,D; scanf("%d %d",&B,&D);
        for(int b=0;b<B;b++){ int x; scanf("%d",&x); d.add(S, x, INF); }
        for(int dd=0;dd<D;dd++){ int x; scanf("%d",&x); d.add(N+x, T, INF); }
        printf("%lld\n", d.maxflow(S,T));
    }
    return 0;
}
