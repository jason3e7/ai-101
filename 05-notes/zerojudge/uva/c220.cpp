// ZeroJudge c220 / UVa129 - Krypton Factor
// 找第 n 個「hard」序列(無相鄰重複子序列), 前 L 個字母, 字典序
#include <bits/stdc++.h>
using namespace std;
int n, L;
bool found;
string s, result;
long long cnt;
// 檢查 s 結尾是否出現相鄰重複 (新加字元在結尾)
bool ok(){
    int len = s.size();
    for (int l = 1; l * 2 <= len; l++) {
        bool same = true;
        for (int k = 0; k < l; k++)
            if (s[len-1-k] != s[len-1-l-k]) { same = false; break; }
        if (same) return false;
    }
    return true;
}
void dfs(){
    for (int c = 0; c < L && !found; c++) {
        s.push_back('A' + c);
        if (ok()) {
            cnt++;
            if (cnt == n) { result = s; found = true; }
            else dfs();
        }
        s.pop_back();
    }
}
int main(){
    while (scanf("%d %d", &n, &L) == 2 && (n || L)) {
        found = false; cnt = 0; s.clear();
        dfs();
        // 每 4 字一組, 空白分隔; 超過 16 組 (每 64 字) 換行
        int len = result.size();
        string line;
        for (int i = 0; i < len; i++) {
            if (i > 0 && i % 4 == 0) { if (i % 64 == 0) line.push_back('\n'); else line.push_back(' '); }
            line.push_back(result[i]);
        }
        printf("%s\n%d\n", line.c_str(), len);
    }
    return 0;
}
