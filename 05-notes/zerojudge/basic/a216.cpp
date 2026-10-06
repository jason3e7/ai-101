// ZeroJudge a216 - 數數愛明明: f(n)=n(n+1)/2, g(n)=n(n+1)(n+2)/6
#include <iostream>
using namespace std;

int main() {
    long long n;
    while (cin >> n) {
        long long f = n * (n + 1) / 2;
        long long g = n * (n + 1) * (n + 2) / 6;
        cout << f << " " << g << "\n";
    }
    return 0;
}
