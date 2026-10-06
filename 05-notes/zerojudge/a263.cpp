// ZeroJudge a263 - 日期差幾天: 兩日期轉成天數相減 (Gregorian, days_from_civil)
#include <iostream>
using namespace std;

long long toDays(long long y, long long m, long long d) {
    if (m <= 2) { y--; m += 12; }
    long long era = (y >= 0 ? y : y - 399) / 400;
    long long yoe = y - era * 400;
    long long doy = (153 * (m - 3) + 2) / 5 + d - 1;
    long long doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + doe - 719468;
}
int main() {
    long long y1, m1, d1, y2, m2, d2;
    while (cin >> y1 >> m1 >> d1 >> y2 >> m2 >> d2) {
        long long diff = toDays(y1, m1, d1) - toDays(y2, m2, d2);
        if (diff < 0) diff = -diff;
        cout << diff << "\n";
    }
    return 0;
}
