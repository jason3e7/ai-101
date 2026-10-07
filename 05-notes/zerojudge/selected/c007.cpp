// ZeroJudge c007 (UVa 272) - TeX Quotes: 把每個 " 交替換成 `` 與 ''
#include <cstdio>
int main(){
    int c; bool open=true;
    while((c=getchar())!=EOF){
        if(c=='"'){ putchar(open?'`':'\''); putchar(open?'`':'\''); open=!open; }
        else putchar(c);
    }
    return 0;
}
