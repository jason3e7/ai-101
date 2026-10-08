// LeetCode #127 Word Ladder (Hard)
// BFS 枚舉每位置改 'a'..'z', 命中字典就入 queue + 刪字典避免重複
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> dict(wordList.begin(), wordList.end());
        if (!dict.count(endWord)) return 0;
        queue<pair<string,int>> q;
        q.push({beginWord, 1});
        dict.erase(beginWord);
        while (!q.empty()) {
            auto [w, d] = q.front(); q.pop();
            if (w == endWord) return d;
            for (int i = 0; i < (int)w.size(); i++) {
                char orig = w[i];
                for (char c = 'a'; c <= 'z'; c++) {
                    if (c == orig) continue;
                    w[i] = c;
                    if (dict.count(w)) { dict.erase(w); q.push({w, d+1}); }
                }
                w[i] = orig;
            }
        }
        return 0;
    }
};
