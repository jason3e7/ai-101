// ZeroJudge b586 - 文章壓縮 (Move-to-Front): 字母組成 word, 其餘為分隔符原樣輸出
// 新 word 原樣輸出並放到最前; 已出現的 word 輸出其位置(從1), 並移到最前. 讀到 0 結束.
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

int main() {
    vector<string> lst;
    string word;
    char c;
    auto flush = [&]() {
        if (word.empty()) return;
        int pos = -1;
        for (int i = 0; i < (int)lst.size(); i++) if (lst[i] == word) { pos = i; break; }
        if (pos >= 0) {
            cout << (pos + 1);
            lst.erase(lst.begin() + pos);
            lst.insert(lst.begin(), word);
        } else {
            cout << word;
            lst.insert(lst.begin(), word);
        }
        word.clear();
    };
    while (cin.get(c)) {
        if (isalpha((unsigned char)c)) word += c;
        else {
            flush();
            if (c == '0') break;
            cout << c;
        }
    }
    return 0;
}
