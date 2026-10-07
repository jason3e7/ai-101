// ZeroJudge a518 (UVa 12468) - Zapping: 100 台循環, 最少按鍵 = min(d, 100-d), d=|a-b|
#include <cstdio>
#include <cstdlib>
int main(){
    int a,b;
    while(scanf("%d %d",&a,&b)==2){
        if(a==-1&&b==-1) break;
        int d=abs(a-b);
        printf("%d\n", d<100-d?d:100-d);
    }
    return 0;
}
