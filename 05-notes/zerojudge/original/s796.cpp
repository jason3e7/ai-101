// ZeroJudge s796 - 蜂蜜工廠: 單位工作、區間[L,R]、一天一單, 最大化總酬勞
// = 區間點指派的最大權重 (transversal matroid): 依酬勞由大到小, 能安排就留 (可重排既有工作)
// 安排用 Kuhn 擴充路徑; DSU 快速找「<=R 的最晚空閒天」當直接指派的捷徑
#include <bits/stdc++.h>
using namespace std;
int N, M;
vector<int> occ;          // day -> job id (-1 空)
vector<int> jL, jR;       // 依酬勞排序後各工作的窗
vector<int> vis; int stamp = 0;
vector<int> par;          // DSU: 最晚 <= x 的空閒天
int consumed;
int findp(int x){ while(par[x]!=x){ par[x]=par[par[x]]; x=par[x]; } return x; }
bool dfs(int i){
    for (int d = jR[i]; d >= jL[i]; --d) {
        if (vis[d] == stamp) continue;
        vis[d] = stamp;
        if (occ[d] == -1) { occ[d] = i; consumed = d; return true; }
        if (dfs(occ[d])) { occ[d] = i; return true; }
    }
    return false;
}
int main(){
    if (scanf("%d %d", &N, &M) != 2) return 0;
    vector<array<long long,3>> jobs(M); // (P,L,R)
    for (int i=0;i<M;i++){ long long L,R,P; scanf("%lld %lld %lld",&L,&R,&P); jobs[i]={P,L,R}; }
    sort(jobs.begin(), jobs.end(), [](auto&a,auto&b){ return a[0]>b[0]; });
    occ.assign(N+1, -1); vis.assign(N+1, 0); jL.assign(M,0); jR.assign(M,0);
    par.assign(N+2, 0); for (int d=0; d<=N+1; d++) par[d]=d;  // par[0]=0 代表無空閒
    long long total = 0; int placed = 0;
    for (int i=0;i<M && placed<N;i++){
        jL[i] = (int)jobs[i][1]; jR[i] = (int)jobs[i][2];
        long long P = jobs[i][0];
        int d = findp(jR[i]);                 // 捷徑: 最晚 <= R 的空閒天
        if (d >= jL[i]) { occ[d]=i; par[d]=d-1; total += P; placed++; continue; }
        // [L,R] 已滿 -> Kuhn 擴充 (把既有工作往左挪)
        ++stamp;
        if (dfs(i)) { total += P; par[consumed] = consumed - 1; placed++; } // 僅消耗一個新空閒天
    }
    printf("%lld\n", total);
    return 0;
}
