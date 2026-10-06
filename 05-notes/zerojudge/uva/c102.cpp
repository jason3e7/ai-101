// ZeroJudge c102 / UVa128 - Software CRC
// g=34943. crc 使 m*65536+crc 能被 g 整除 => crc = (g - m*65536 mod g) mod g
#include <bits/stdc++.h>
using namespace std;
int main(){
    const int g = 34943;
    string line;
    while (getline(cin, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back(); // 去掉 Windows 行尾
        if (line == "#") break;
        long long r = 0;
        for (unsigned char c : line) r = (r * 256 + c) % g;
        r = (r * 65536) % g;           // 補 2 個 0 位元組
        int crc = (int)((g - r) % g);
        printf("%02X %02X\n", (crc >> 8) & 0xff, crc & 0xff);
    }
    return 0;
}
