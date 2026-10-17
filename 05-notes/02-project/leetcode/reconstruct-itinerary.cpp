// LeetCode #332 Reconstruct Itinerary (Hard)
// Hierholzer 歐拉路徑: multiset 按字典序取邊, 後序 push 再反轉
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<string> findItinerary(vector<vector<string>>& tickets) {
        map<string, multiset<string>> g;
        for (auto& t : tickets) g[t[0]].insert(t[1]);
        vector<string> res;
        function<void(string)> dfs = [&](string u) {
            while (g[u].size()) {
                string v = *g[u].begin();
                g[u].erase(g[u].begin());
                dfs(v);
            }
            res.push_back(u);
        };
        dfs("JFK");
        reverse(res.begin(), res.end());
        return res;
    }
};
