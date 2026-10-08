// LeetCode #981 Time Based Key-Value Store (Med)
// 每個 key 存 (ts, val) 已排序, get 用 upper_bound 配 \x7f 字串找最大 ts <= 查詢
#include <bits/stdc++.h>
using namespace std;

class TimeMap {
    unordered_map<string, vector<pair<int,string>>> mp;
public:
    TimeMap() {}
    void set(string key, string value, int timestamp) { mp[key].push_back({timestamp, value}); }
    string get(string key, int timestamp) {
        auto& v = mp[key];
        auto it = upper_bound(v.begin(), v.end(), make_pair(timestamp, string("\x7f")));
        if (it == v.begin()) return "";
        return prev(it)->second;
    }
};
