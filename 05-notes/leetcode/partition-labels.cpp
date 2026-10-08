// LeetCode #763 Partition Labels (Med)
// 預先算每字母最後出現位置, 走過時延伸 end, 到 end 切一刀
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> partitionLabels(string s) {
        int last[26] = {0};
        for (int i = 0; i < (int)s.size(); i++) last[s[i]-'a'] = i;
        vector<int> res;
        int start = 0, end = 0;
        for (int i = 0; i < (int)s.size(); i++) {
            end = max(end, last[s[i]-'a']);
            if (i == end) { res.push_back(end - start + 1); start = i + 1; }
        }
        return res;
    }
};
