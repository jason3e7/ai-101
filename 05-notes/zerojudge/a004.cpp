// ZeroJudge a004 - 文文的求婚 (閏年判斷)
// 讀年份到 EOF; 被4整除且不被100整除, 或被400整除 → 閏年, 否則平年
#include <iostream>
using namespace std;

int main() {
    long long y;
    while (cin >> y) {
        bool leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
        cout << (leap ? "閏年" : "平年") << "\n";
    }
    return 0;
}
