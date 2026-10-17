// ZeroJudge d096 (UVa 913) - Joana 奇數列: 第 N 個奇數的列(N 個數)最後三數之和
// 第 k 列有 2k-1 個數, 末位置 = k^2, 末三值 2k^2-1,2k^2-3,2k^2-5, 和 = 6k^2-9. k=(N+1)/2.
#include <cstdio>
int main(){
    long long N;
    while(scanf("%lld",&N)==1){
        long long k=(N+1)/2;
        printf("%lld\n", 6*k*k-9);
    }
    return 0;
}
