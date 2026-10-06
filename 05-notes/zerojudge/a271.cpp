// ZeroJudge a271 - 彩色蘿蔔 (模擬)
// 早上先中毒 (扣 n * 已累積毒stack), 晚上吃: 1紅+x 2白+y 3黃-z 4黑-w且中毒stack++, 0不吃
// 任意時刻體重 <=0 -> bye~Rabbit, 否則輸出 "Wg". 食物行可能為空 (輸出初始體重).
#include <iostream>
#include <string>
#include <sstream>
#include <limits>
using namespace std;

int main() {
    int T;
    cin >> T;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (T--) {
        string line1, line2;
        getline(cin, line1);
        getline(cin, line2);
        long long x, y, z, w, n, m;
        stringstream ss1(line1); ss1 >> x >> y >> z >> w >> n >> m;
        long long weight = m, poison = 0;
        bool dead = false;
        int f;
        stringstream ss2(line2);
        while (ss2 >> f) {
            weight -= n * poison;                 // 早上中毒
            if (weight <= 0) { dead = true; break; }
            if (f == 1) weight += x;              // 晚上吃
            else if (f == 2) weight += y;
            else if (f == 3) weight -= z;
            else if (f == 4) { weight -= w; poison++; }
            if (weight <= 0) { dead = true; break; }
        }
        cout << (dead ? "bye~Rabbit" : to_string(weight) + "g") << "\n";
    }
    return 0;
}
