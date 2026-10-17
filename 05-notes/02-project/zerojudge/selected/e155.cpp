// ZeroJudge e155 (UVa 10935) - Throwing cards: 丟頂牌, 再把新頂牌移到底. 輸出丟棄順序與剩牌.
#include <cstdio>
#include <deque>
using namespace std;
int main(){
    int n;
    while(scanf("%d",&n)==1 && n){
        deque<int> q;
        for(int i=1;i<=n;i++) q.push_back(i);
        printf("Discarded cards:");
        bool firstd=true;
        while(q.size()>=2){
            int top=q.front(); q.pop_front();
            printf("%s %d", firstd?"":",", top); firstd=false;
            q.push_back(q.front()); q.pop_front();
        }
        printf("\nRemaining card: %d\n", q.front());
    }
    return 0;
}
