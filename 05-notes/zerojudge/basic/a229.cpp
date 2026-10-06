// ZeroJudge a229 - 括號匹配問題: 列出 N 組括號所有合法匹配 (字典序, '(' 優先), 每組測資後空一行
#include <iostream>
#include <string>
using namespace std;

void gen(int n, int open, int close, string& cur, string& out) {
    if ((int)cur.size() == 2 * n) { out += cur; out += '\n'; return; }
    if (open < n)    { cur.push_back('('); gen(n, open + 1, close, cur, out); cur.pop_back(); }
    if (close < open){ cur.push_back(')'); gen(n, open, close + 1, cur, out); cur.pop_back(); }
}
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int N;
    while (cin >> N) {
        string out, cur;
        gen(N, 0, 0, cur, out);
        out += '\n';           // 每組後空一行
        cout << out;
    }
    return 0;
}
