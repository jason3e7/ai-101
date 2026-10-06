// ZeroJudge a224 - 明明愛明明: 字母能否重排成迴文 (忽略大小寫與非字母; 奇數次字母 <=1)
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

int main() {
    string line;
    while (getline(cin, line)) {
        int cnt[26] = {0};
        for (unsigned char c : line)
            if (isalpha(c)) cnt[tolower(c) - 'a']++;
        int odd = 0;
        for (int i = 0; i < 26; i++) if (cnt[i] & 1) odd++;
        cout << (odd <= 1 ? "yes !" : "no...") << "\n";
    }
    return 0;
}
