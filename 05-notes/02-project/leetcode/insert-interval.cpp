// LeetCode #57 Insert Interval (Med)
// 三段式: 不重疊的左邊照抄 → 重疊的合併 → 不重疊的右邊照抄
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<int>> insert(vector<vector<int>>& iv, vector<int>& ni) {
        vector<vector<int>> res;
        int i = 0, n = iv.size();
        while (i < n && iv[i][1] < ni[0]) res.push_back(iv[i++]);
        while (i < n && iv[i][0] <= ni[1]) {
            ni[0] = min(ni[0], iv[i][0]);
            ni[1] = max(ni[1], iv[i][1]);
            i++;
        }
        res.push_back(ni);
        while (i < n) res.push_back(iv[i++]);
        return res;
    }
};
