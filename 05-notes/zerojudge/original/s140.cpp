// ZeroJudge s140 - 排隊買飲料: 兩機每 a/b 秒一杯, 求第 n 杯最早時間 (mod 1e6+7)
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int main(){
    ll n,a,b; cin >> n >> a >> b;
    ll lo=0, hi=n*max(a,b);
    while(lo<hi){ ll t=(lo+hi)/2; ll cnt=t/a+t/b; if(cnt>=n) hi=t; else lo=t+1; }
    cout << (lo % 1000007) << "\n";
    return 0;
}
