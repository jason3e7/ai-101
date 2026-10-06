// ZeroJudge a121 - 質數又來囉: 計算 [a,b] 間質數個數 (b-a<=1000, b<=1e8)
#include <iostream>
using namespace std;

bool isPrime(long long n) {
    if (n < 2) return false;
    for (long long i = 2; i * i <= n; i++) if (n % i == 0) return false;
    return true;
}
int main() {
    long long a, b;
    while (cin >> a >> b) {
        int c = 0;
        for (long long x = a; x <= b; x++) if (isPrime(x)) c++;
        cout << c << "\n";
    }
    return 0;
}
