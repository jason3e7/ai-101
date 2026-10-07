// ZeroJudge d379 (UVa 446) - 十六進位相加減, 輸出 13 位二進位與十進位結果
#include <cstdio>
void pb(int v){ char s[14]; for(int i=12;i>=0;i--){ s[12-i]=((v>>i)&1)?'1':'0'; } s[13]=0; printf("%s",s); }
int main(){
    int N; if(scanf("%d",&N)!=1) return 0;
    while(N--){
        int a,b; char op;
        scanf("%x %c %x",&a,&op,&b);
        int res = (op=='+')? a+b : a-b;
        pb(a); printf(" %c ",op); pb(b); printf(" = %d\n", res);
    }
    return 0;
}
