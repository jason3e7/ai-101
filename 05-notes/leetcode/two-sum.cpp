// LeetCode #1 Two Sum (Easy)
// hash map 一遍過: 掃到 nums[i] 時, 看 target - nums[i] 之前是否出現過
// O(n) 時間, O(n) 空間
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> seen;  // value -> index
        for (int i = 0; i < (int)nums.size(); i++) {
            int need = target - nums[i];
            auto it = seen.find(need);
            if (it != seen.end()) return {it->second, i};
            seen[nums[i]] = i;
        }
        return {};  // 題目保證有解, 不會到這
    }
};

// 本機測試 harness
#ifdef LOCAL
int main() {
    Solution s;
    auto run = [&](vector<int> nums, int target, vector<int> expected) {
        auto got = s.twoSum(nums, target);
        sort(got.begin(), got.end());
        sort(expected.begin(), expected.end());
        bool ok = got == expected;
        cout << (ok ? "PASS" : "FAIL") << " target=" << target << " got=[";
        for (size_t i = 0; i < got.size(); i++) cout << (i ? "," : "") << got[i];
        cout << "] expected=[";
        for (size_t i = 0; i < expected.size(); i++) cout << (i ? "," : "") << expected[i];
        cout << "]\n";
    };
    run({2, 7, 11, 15}, 9, {0, 1});
    run({3, 2, 4}, 6, {1, 2});
    run({3, 3}, 6, {0, 1});
    return 0;
}
#endif
