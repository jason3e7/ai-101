// LeetCode #202 Happy Number (Easy)
// Floyd 龜兔跑, 迴圈入口不是 1 就 false
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool isHappy(int n) {
        auto next = [](int x) {
            int s = 0;
            while (x) { int d = x % 10; s += d * d; x /= 10; }
            return s;
        };
        int slow = n, fast = next(n);
        while (fast != 1 && slow != fast) {
            slow = next(slow);
            fast = next(next(fast));
        }
        return fast == 1;
    }
};
