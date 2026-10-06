// ZeroJudge a149 - 乘乘樂: 把每個位數相乘
#include <iostream>
#include <string>
using namespace std;

int main() {
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        string s; cin >> s;
        long long p = 1;
        for (char c : s) p *= (c - '0');
        cout << p << "\n";
    }
    return 0;
}
