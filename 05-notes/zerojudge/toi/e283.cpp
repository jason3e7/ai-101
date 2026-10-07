// ZeroJudge e283 - 小崴的特殊編碼: 4-bit 序列 -> A~F 解碼
#include <bits/stdc++.h>
using namespace std;
int main(){
    map<string,char> m = {
        {"0101",'A'},{"0111",'B'},{"0010",'C'},{"1101",'D'},{"1000",'E'},{"1100",'F'}
    };
    int n;
    while (cin >> n) {
        string out;
        for (int i = 0; i < n; i++) {
            int a, b, c, d; cin >> a >> b >> c >> d;
            string key; key += ('0'+a); key += ('0'+b); key += ('0'+c); key += ('0'+d);
            out += m[key];
        }
        cout << out << "\n";
    }
    return 0;
}
