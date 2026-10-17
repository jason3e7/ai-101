// LeetCode #901 Online Stock Span (Med)
// 單調遞減 stack 儲存 (價格, 累計 span); 吃掉 <= price 的區段
#include <bits/stdc++.h>
using namespace std;

class StockSpanner {
    stack<pair<int,int>> st;
public:
    StockSpanner() {}
    int next(int price) {
        int span = 1;
        while (!st.empty() && st.top().first <= price) { span += st.top().second; st.pop(); }
        st.push({price, span});
        return span;
    }
};
