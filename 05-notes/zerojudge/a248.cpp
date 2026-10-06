// ZeroJudge a248 - 精準小數除法: a/b 到小數點後 N 位 (無條件捨去), 長除法
#include <iostream>
#include <string>
using namespace std;

int main() {
    long long a, b, N;
    while (cin >> a >> b >> N) {
        string out = to_string(a / b);
        long long r = a % b;
        out += '.';
        for (long long i = 0; i < N; i++) { r *= 10; out += char('0' + r / b); r %= b; }
        out += '\n';
        cout << out;
    }
    return 0;
}
