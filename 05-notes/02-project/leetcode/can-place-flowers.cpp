// LeetCode #605 Can Place Flowers (Easy)
// 貪心: 掃到空位且左右都空 -> 種下 (改 1)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool canPlaceFlowers(vector<int>& f, int n) {
        int sz = f.size();
        for (int i = 0; i < sz && n > 0; i++) {
            if (f[i] == 0 && (i == 0 || f[i-1] == 0) && (i == sz-1 || f[i+1] == 0)) {
                f[i] = 1; n--;
            }
        }
        return n <= 0;
    }
};
