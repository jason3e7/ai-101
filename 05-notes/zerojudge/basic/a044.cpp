// ZeroJudge a044 - 空間切割: n 個平面最多把空間切成 (n^3 + 5n + 6)/6 塊
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

void print128(__int128 x) {
    if (x == 0) { cout << '0'; return; }
    string s;
    while (x) { s += char('0' + (int)(x % 10)); x /= 10; }
    reverse(s.begin(), s.end());
    cout << s;
}

int main() {
    long long n;
    while (cin >> n) {
        __int128 N = n;
        __int128 r = (N * N * N + 5 * N + 6) / 6;
        print128(r);
        cout << "\n";
    }
    return 0;
}
