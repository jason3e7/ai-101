// LeetCode #1268 Search Suggestions System (Med)
// 排序後對每個 prefix 做 lower_bound, 連續取前 3 個仍帶 prefix 的
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<string>> suggestedProducts(vector<string>& products, string searchWord) {
        sort(products.begin(), products.end());
        vector<vector<string>> res;
        string prefix;
        for (char c : searchWord) {
            prefix.push_back(c);
            auto it = lower_bound(products.begin(), products.end(), prefix);
            vector<string> cur;
            for (int i = 0; i < 3 && it + i != products.end(); i++) {
                const string& s = *(it + i);
                if (s.compare(0, prefix.size(), prefix) != 0) break;
                cur.push_back(s);
            }
            res.push_back(cur);
        }
        return res;
    }
};
