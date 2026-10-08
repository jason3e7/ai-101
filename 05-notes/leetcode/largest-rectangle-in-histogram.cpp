// LeetCode #84 Largest Rectangle in Histogram (Hard)
// 單調遞增 stack, 遇到較低柱就 pop + 算面積, 末尾加 0 收尾
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int largestRectangleArea(vector<int>& h) {
        h.push_back(0);
        stack<int> st;
        int best = 0, n = h.size();
        for (int i = 0; i < n; i++) {
            while (!st.empty() && h[st.top()] > h[i]) {
                int top = st.top(); st.pop();
                int w = st.empty() ? i : i - st.top() - 1;
                best = max(best, h[top] * w);
            }
            st.push(i);
        }
        return best;
    }
};
