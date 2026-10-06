// ZeroJudge a013 - 羅馬數字
// 每行兩個羅馬數字, 輸出差的絕對值 (羅馬數字), 0 輸出 ZERO; # 結束
#include <iostream>
#include <string>
using namespace std;

int rval(char c) {
    switch (c) {
        case 'I': return 1; case 'V': return 5; case 'X': return 10;
        case 'L': return 50; case 'C': return 100; case 'D': return 500;
        case 'M': return 1000;
    }
    return 0;
}
int parse(const string& s) {
    int t = 0, n = s.size();
    for (int i = 0; i < n; i++) {
        int v = rval(s[i]);
        if (i + 1 < n && rval(s[i + 1]) > v) t -= v; else t += v;
    }
    return t;
}
string toRoman(int n) {
    int vals[] = {1000,900,500,400,100,90,50,40,10,9,5,4,1};
    string sym[] = {"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
    string r;
    for (int i = 0; i < 13; i++) while (n >= vals[i]) { r += sym[i]; n -= vals[i]; }
    return r;
}
int main() {
    string t1, t2;
    while (cin >> t1) {
        if (t1 == "#") break;
        cin >> t2;
        int d = parse(t1) - parse(t2);
        if (d < 0) d = -d;
        cout << (d == 0 ? "ZERO" : toRoman(d)) << "\n";
    }
    return 0;
}
