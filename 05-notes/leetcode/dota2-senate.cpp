// LeetCode #649 Dota2 Senate (Med)
// 兩個 queue 存 R/D 的 index, 誰 index 小誰先投票 ban 對方
// 被 ban 的從 queue 消失, 自己 push 回 index + n (下輪)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string predictPartyVictory(string s) {
        queue<int> r, d;
        int n = s.size();
        for (int i = 0; i < n; i++) (s[i] == 'R' ? r : d).push(i);
        while (!r.empty() && !d.empty()) {
            int ri = r.front(), di = d.front();
            r.pop(); d.pop();
            if (ri < di) r.push(ri + n);
            else d.push(di + n);
        }
        return r.empty() ? "Dire" : "Radiant";
    }
};
