// ZeroJudge a034 - 二進位制轉換 (10進位 -> 2進位), 多行到 EOF
#include <iostream>
#include <string>
using namespace std;

int main() {
    long long n;
    while (cin >> n) {
        if (n == 0) { cout << "0\n"; continue; }
        long long x = n < 0 ? -n : n;
        string s;
        while (x) { s = char('0' + x % 2) + s; x /= 2; }
        if (n < 0) s = "-" + s;
        cout << s << "\n";
    }
    return 0;
}
