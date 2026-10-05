// ZeroJudge a003 - 兩光法師占卜術
// 輸入月 M 日 D, S=(M*2+D)%3; S=0 普通, 1 吉, 2 大吉. 多組讀到 EOF.
#include <iostream>
using namespace std;

int main() {
    long long M, D;
    while (cin >> M >> D) {
        int S = (int)(((M * 2 + D) % 3 + 3) % 3);
        if (S == 0) cout << "普通\n";
        else if (S == 1) cout << "吉\n";
        else cout << "大吉\n";
    }
    return 0;
}
