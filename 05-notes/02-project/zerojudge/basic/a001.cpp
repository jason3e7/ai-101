// ZeroJudge a001 - 哈囉
// 讀入一行字串 s, 輸出 "hello, " + s
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    while (getline(cin, s)) {
        // 去掉 Windows 行尾的 \r, 避免輸出多一個不可見字元
        while (!s.empty() && (s.back() == '\r' || s.back() == '\n')) s.pop_back();
        cout << "hello, " << s << "\n";
    }
    return 0;
}
