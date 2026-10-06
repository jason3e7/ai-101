// ZeroJudge a021 - 大數運算 (+ - * /, 正整數, 結果 <= 500 位)
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

string strip(string s) { int i = 0; while (i + 1 < (int)s.size() && s[i] == '0') i++; return s.substr(i); }
int cmp(const string& a, const string& b) {
    string x = strip(a), y = strip(b);
    if (x.size() != y.size()) return x.size() < y.size() ? -1 : 1;
    if (x < y) return -1; if (x > y) return 1; return 0;
}
string add(const string& a, const string& b) {
    string r; int i = a.size() - 1, j = b.size() - 1, c = 0;
    while (i >= 0 || j >= 0 || c) {
        int s = c; if (i >= 0) s += a[i--] - '0'; if (j >= 0) s += b[j--] - '0';
        r += char('0' + s % 10); c = s / 10;
    }
    reverse(r.begin(), r.end()); return strip(r);
}
string sub(const string& a, const string& b) {   // a >= b >= 0
    string r; int i = a.size() - 1, j = b.size() - 1, bor = 0;
    while (i >= 0) {
        int s = (a[i] - '0') - bor - (j >= 0 ? (b[j] - '0') : 0);
        if (s < 0) { s += 10; bor = 1; } else bor = 0;
        r += char('0' + s); i--; j--;
    }
    reverse(r.begin(), r.end()); return strip(r);
}
string mul(const string& a, const string& b) {
    if (strip(a) == "0" || strip(b) == "0") return "0";
    int n = a.size(), m = b.size();
    vector<int> res(n + m, 0);
    for (int i = n - 1; i >= 0; i--)
        for (int j = m - 1; j >= 0; j--) {
            int sum = (a[i] - '0') * (b[j] - '0') + res[i + j + 1];
            res[i + j + 1] = sum % 10; res[i + j] += sum / 10;
        }
    string r; for (int x : res) r += char('0' + x); return strip(r);
}
string divide(const string& a, const string& b) {
    if (cmp(a, b) < 0) return "0";
    string result, cur = "";
    for (char ch : a) {
        cur = strip(cur + ch);
        int q = 0;
        for (int d = 9; d >= 1; d--) if (cmp(mul(b, to_string(d)), cur) <= 0) { q = d; break; }
        result += char('0' + q);
        if (q) cur = sub(cur, mul(b, to_string(q)));
    }
    return strip(result);
}
int main() {
    string a, op, b;
    while (cin >> a >> op >> b) {
        string res;
        if (op == "+") res = add(a, b);
        else if (op == "*") res = mul(a, b);
        else if (op == "/") res = divide(a, b);
        else res = (cmp(a, b) >= 0) ? sub(a, b) : "-" + sub(b, a);
        cout << res << "\n";
    }
    return 0;
}
