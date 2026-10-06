// ZeroJudge a020 - 身分證檢驗 (台灣身分證字號檢查碼)
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

int main() {
    // A..Z 對應數字
    int m[26] = {10,11,12,13,14,15,16,17,34,18,19,20,21,22,35,23,24,25,26,27,28,29,32,30,31,33};
    string id;
    while (cin >> id) {
        int val = m[toupper(id[0]) - 'A'];
        long long sum = val / 10 + (val % 10) * 9;   // 十位*1 + 個位*9
        for (int i = 0; i < 8; i++) sum += (id[1 + i] - '0') * (8 - i);
        sum += (id[9] - '0');                         // 最後一碼
        cout << (sum % 10 == 0 ? "real" : "fake") << "\n";
    }
    return 0;
}
