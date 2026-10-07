// ZeroJudge c015 (UVa 10018) - Reverse and Add: 反轉相加到迴文, 輸出次數與迴文
#include <cstdio>
typedef unsigned long long u64;
u64 rev(u64 x){ u64 r=0; while(x){ r=r*10+x%10; x/=10; } return r; }
bool pal(u64 x){ return x==rev(x); }
int main(){
    int N; if(scanf("%d",&N)!=1) return 0;
    while(N--){
        u64 p; scanf("%llu",&p);
        int c=0;
        do { p+=rev(p); c++; } while(!pal(p));  // 至少做一次相加(即使起始已是迴文)
        printf("%d %llu\n", c, p);
    }
    return 0;
}
