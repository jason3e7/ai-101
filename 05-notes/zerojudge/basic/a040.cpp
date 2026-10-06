// ZeroJudge a040 - 阿姆斯壯數 (Armstrong numbers) 在範圍 [lo,hi] 內
#include <iostream>
#include <vector>
#include <string>
using namespace std;

long long ipow(int b, int e) { long long r = 1; while (e--) r *= b; return r; }

int main() {
    vector<long long> arm;
    for (long long i = 1; i <= 1000000; i++) {
        long long t = i; int d = 0; while (t) { d++; t /= 10; }
        long long sum = 0; t = i; while (t) { sum += ipow(t % 10, d); t /= 10; }
        if (sum == i) arm.push_back(i);
    }
    long long lo, hi;
    while (cin >> lo >> hi) {
        string out; bool f = false;
        for (long long a : arm) if (a >= lo && a <= hi) { if (f) out += " "; out += to_string(a); f = true; }
        cout << (f ? out : "none") << "\n";
    }
    return 0;
}
