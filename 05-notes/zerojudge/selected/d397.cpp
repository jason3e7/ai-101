// ZeroJudge d397 (UVa 147) - Dollars: 用紐幣面額組合金額的方法數(順序不計)
// 面額(換成「分」): 5,10,20,50,100,200,500,1000,2000,5000,10000. 金額<=300.00.
// 無限背包計數 DP. 答案最大 ~1.8e14 用 long long. 輸入金額轉整數分避免浮點誤差.
#include <cstdio>
#include <cstring>
using namespace std;
int main(){
    const int MAX=30000;
    static long long ways[MAX+1];
    int coins[]={5,10,20,50,100,200,500,1000,2000,5000,10000};
    ways[0]=1;
    for(int c: coins) for(int v=c; v<=MAX; v++) ways[v]+=ways[v-c];
    char buf[64];
    while(scanf("%63s",buf)==1){
        // 解析 d.cc -> 分
        long long dollars=0; int cents=0; int i=0; bool dot=false; int fc=0;
        for(; buf[i]; i++){
            if(buf[i]=='.'){ dot=true; continue; }
            int d=buf[i]-'0';
            if(!dot) dollars=dollars*10+d;
            else { cents=cents*10+d; fc++; }
        }
        long long total=dollars*100+cents;
        if(total==0) break;
        double amt=total/100.0;
        printf("%6.2f%17lld\n", amt, ways[total]);
    }
    return 0;
}
