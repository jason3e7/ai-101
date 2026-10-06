// ZeroJudge a244 - 新手訓練 ~ for + if: a=1 b+c, 2 b-c, 3 b*c, 4 b/c
#include <iostream>
using namespace std;

int main() {
    int N;
    if (!(cin >> N)) return 0;
    while (N--) {
        long long a, b, c;
        cin >> a >> b >> c;
        long long r = (a == 1) ? b + c : (a == 2) ? b - c : (a == 3) ? b * c : b / c;
        cout << r << "\n";
    }
    return 0;
}
