// LeetCode #841 Keys and Rooms (Med)
// 從 room 0 開始 DFS, 看能不能走遍所有 room
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool canVisitAllRooms(vector<vector<int>>& rooms) {
        int n = rooms.size();
        vector<char> vis(n, 0);
        stack<int> st; st.push(0); vis[0] = 1;
        int cnt = 1;
        while (!st.empty()) {
            int u = st.top(); st.pop();
            for (int v : rooms[u]) if (!vis[v]) { vis[v] = 1; cnt++; st.push(v); }
        }
        return cnt == n;
    }
};
