// LeetCode #739 Daily Temperatures (Med)
// 單調遞減 stack, 遇更高溫就 pop 並記錄距離
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& t) {
        int n = t.size();
        vector<int> res(n, 0);
        stack<int> st;
        for (int i = 0; i < n; i++) {
            while (!st.empty() && t[i] > t[st.top()]) {
                int j = st.top(); st.pop();
                res[j] = i - j;
            }
            st.push(i);
        }
        return res;
    }
};
