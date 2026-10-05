// ZeroJudge a010 - 因數分解 (質因數分解, 格式 2^2 * 5)
#include <iostream>
#include <string>
using namespace std;

int main() {
    long long n;
    while (cin >> n) {
        string out;
        bool first = true;
        for (long long p = 2; p * p <= n; p++) {
            if (n % p == 0) {
                int e = 0;
                while (n % p == 0) { n /= p; e++; }
                if (!first) out += " * ";
                first = false;
                out += to_string(p);
                if (e > 1) out += "^" + to_string(e);
            }
        }
        if (n > 1) {
            if (!first) out += " * ";
            out += to_string(n);
        }
        cout << out << "\n";
    }
    return 0;
}
