// ZeroJudge a536 (UVa 11689) - 收集空瓶換汽水
// e 原有空瓶 + f 街上撿的 = 總空瓶 E; 每 c 個空瓶換 1 瓶(喝完又變 1 空瓶).
// 每輪: 換 E/c 瓶, 剩 E%c 空瓶再加上剛喝完的 E/c 個空瓶. 直到 E<c.
#include <iostream>
using namespace std;
int main(){
    int N; if(!(cin>>N)) return 0;
    while(N--){
        long long e,f,c; cin>>e>>f>>c;
        long long E=e+f, drinks=0;
        while(E>=c){ long long k=E/c; drinks+=k; E=E%c+k; }
        cout<<drinks<<"\n";
    }
    return 0;
}
