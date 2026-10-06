// ZeroJudge a022 - 迴文 (palindrome), 多行到 EOF
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    while (cin >> s) {
        string r(s.rbegin(), s.rend());
        cout << (s == r ? "yes" : "no") << "\n";
    }
    return 0;
}
