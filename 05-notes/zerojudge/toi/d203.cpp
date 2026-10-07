// ZeroJudge d203 - 加法減法的奧妙: 在 1..n 間插入 ' '(接合)/'+'/'-' 使和為 0, 列出所有解(ASCII 序)
// DFS 以 ' '<'+'<'- 的順序嘗試 => 輸出自然按 ASCII 排序
#include <bits/stdc++.h>
using namespace std;
int n;
string buf;
void dfs(int k, long long total, long long term, int sign){
    if (k > n) { if (total + sign*term == 0) printf("%s\n", buf.c_str()); return; }
    string ks = to_string(k); int d = ks.size();
    long long pw = 1; for (int i = 0; i < d; i++) pw *= 10;
    long long kv = k;
    // ' ' 接合: 延長目前的項
    buf += ' '; buf += ks; dfs(k+1, total, term*pw + kv, sign); buf.resize(buf.size()-1-d);
    // '+'
    buf += '+'; buf += ks; dfs(k+1, total + sign*term, kv, +1); buf.resize(buf.size()-1-d);
    // '-'
    buf += '-'; buf += ks; dfs(k+1, total + sign*term, kv, -1); buf.resize(buf.size()-1-d);
}
int main(){
    while (scanf("%d", &n) == 1) {
        buf = "1";
        dfs(2, 0, 1, 1);
    }
    return 0;
}
