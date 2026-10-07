// ZeroJudge j056 (UVa 11650) - Mirror Clock: 由鏡中時刻反推真實時刻
// 鏡像 = (720 - t) mod 720 分鐘(12小時制). 真實 = 鏡像(給定).
#include <cstdio>
int main(){
    int T; scanf("%d",&T);
    while(T--){
        int h,m; scanf("%d:%d",&h,&m);
        int g=(h%12)*60+m;
        int r=(720-g)%720;
        int hh=r/60; if(hh==0) hh=12;
        printf("%02d:%02d\n", hh, r%60);
    }
    return 0;
}
