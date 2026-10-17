// ZeroJudge a065 - 提款卡密碼: 7 個字母, 相鄰距離 (abs 差) 組成 6 位密碼
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

int main() {
    string s;
    while (cin >> s) {
        string r;
        for (int i = 0; i < 6; i++) r += to_string(abs(s[i] - s[i + 1]));
        cout << r << "\n";
    }
    return 0;
}
