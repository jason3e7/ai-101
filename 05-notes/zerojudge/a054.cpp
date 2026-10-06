// ZeroJudge a054 - 電話客服中心: 給身分證後9碼, 推回可能的第一個英文字母
#include <iostream>
#include <string>
using namespace std;

int main() {
    int m[26] = {10,11,12,13,14,15,16,17,34,18,19,20,21,22,35,23,24,25,26,27,28,29,32,30,31,33};
    string in;
    while (cin >> in) {
        int c = in[8] - '0';              // 給定的檢查碼
        string res;
        for (int k = 0; k < 26; k++) {
            int val = m[k];
            int sum = (val / 10) * 1 + (val % 10) * 9;
            int mult = 8;
            for (int i = 0; i < 8; i++) { sum += (in[i] - '0') * mult; mult--; }
            int chk = (10 - sum % 10) % 10;
            if (chk == c) res += char('A' + k);
        }
        cout << res << "\n";
    }
    return 0;
}
