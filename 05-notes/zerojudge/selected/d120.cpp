// ZeroJudge d120 (UVa 10699) - 相異質因數個數. n<=1e6, 試除. 讀到 0 結束.
#include <cstdio>
int main(){
    long long n;
    while(scanf("%lld",&n)==1 && n!=0){
        long long x=n, cnt=0;
        for(long long p=2;p*p<=x;p++){ if(x%p==0){ cnt++; while(x%p==0) x/=p; } }
        if(x>1) cnt++;
        printf("%lld : %lld\n", n, cnt);
    }
    return 0;
}
