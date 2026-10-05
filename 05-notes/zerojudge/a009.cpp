// ZeroJudge a009 - 解碼器 (凱薩密碼, K=7, 解密 = 每個字元 - 7)
#include <iostream>
using namespace std;

int main() {
    char ch;
    while (cin.get(ch)) {
        if (ch == '\n') cout << '\n';
        else if (ch == '\r') ;          // 跳過 Windows 行尾
        else cout << (char)(ch - 7);
    }
    return 0;
}
