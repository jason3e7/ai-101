// ZeroJudge a024 - 最大公因數 (GCD), 多組到 EOF
#include <iostream>
using namespace std;

int main() {
    long long a, b;
    while (cin >> a >> b) {
        while (b) { long long t = a % b; a = b; b = t; }
        cout << a << "\n";
    }
    return 0;
}
