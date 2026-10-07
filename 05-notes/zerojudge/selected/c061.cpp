// ZeroJudge c061 (UVa 530) - 組合數 C(n,m) < 2^31. 迭代乘除避免溢位. 讀到 0 0 結束.
#include <cstdio>
int main(){
    long long n,m;
    while(scanf("%lld %lld",&n,&m)==2 && (n||m)){
        if(m>n-m) m=n-m;
        long long C=1;
        for(long long i=1;i<=m;i++) C=C*(n-m+i)/i;
        printf("%lld\n", C);
    }
    return 0;
}
