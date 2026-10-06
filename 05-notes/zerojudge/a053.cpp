// ZeroJudge a053 - Sagit's 計分程式
// 0~10 每題6分, 11~20 每題2分, 21~40 每題1分, >40 一律100分
#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    int n;
    while (cin >> n) {
        int s;
        if (n > 40) s = 100;
        else s = min(n,10)*6 + max(0,min(n,20)-10)*2 + max(0,min(n,40)-20)*1;
        cout << s << "\n";
    }
    return 0;
}
