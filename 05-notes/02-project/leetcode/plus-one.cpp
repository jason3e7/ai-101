// LeetCode #66 Plus One (Easy)
// 從後往前, 遇 9 設 0 再進位, 全進位則 prepend 1
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> plusOne(vector<int>& d) {
        for (int i = d.size() - 1; i >= 0; i--) {
            if (d[i] < 9) { d[i]++; return d; }
            d[i] = 0;
        }
        d.insert(d.begin(), 1);
        return d;
    }
};
