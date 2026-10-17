// LeetCode #853 Car Fleet (Med)
// 依 position 降序排, 計算每車到終點時間; 後車時間 ≤ 前車就併隊, 否則獨立
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int carFleet(int target, vector<int>& position, vector<int>& speed) {
        int n = position.size();
        vector<pair<int,double>> cars(n);
        for (int i = 0; i < n; i++) cars[i] = {position[i], (double)(target - position[i]) / speed[i]};
        sort(cars.begin(), cars.end(), greater<>());
        int fleets = 0;
        double cur = 0;
        for (auto& c : cars) if (c.second > cur) { fleets++; cur = c.second; }
        return fleets;
    }
};
