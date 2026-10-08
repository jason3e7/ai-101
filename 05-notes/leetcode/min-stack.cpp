// LeetCode #155 Min Stack (Med)
// 兩個 stack: 主 stack 存值, 輔 stack 僅在 val <= 當前 min 時 push
#include <bits/stdc++.h>
using namespace std;

class MinStack {
    stack<int> st, mn;
public:
    MinStack() {}
    void push(int val) {
        st.push(val);
        if (mn.empty() || val <= mn.top()) mn.push(val);
    }
    void pop() {
        if (st.top() == mn.top()) mn.pop();
        st.pop();
    }
    int top() { return st.top(); }
    int getMin() { return mn.top(); }
};
