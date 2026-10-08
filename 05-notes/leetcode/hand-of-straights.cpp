// LeetCode #846 Hand of Straights (Med)
// 貪心, map (已排序) 取最小當起點, 連續 k 張各扣 1
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool isNStraightHand(vector<int>& hand, int groupSize) {
        if (hand.size() % groupSize) return false;
        map<int,int> cnt;
        for (int x : hand) cnt[x]++;
        while (!cnt.empty()) {
            int start = cnt.begin()->first;
            for (int i = 0; i < groupSize; i++) {
                auto it = cnt.find(start + i);
                if (it == cnt.end()) return false;
                if (--it->second == 0) cnt.erase(it);
            }
        }
        return true;
    }
};
