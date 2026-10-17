// LeetCode #130 Surrounded Regions (Med)
// 從邊界的 'O' DFS 標 '#' 當保留, 剩下 'O' 全翻 'X', '#' 再翻回 'O'
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    void solve(vector<vector<char>>& b) {
        int n = b.size(), m = b[0].size();
        function<void(int,int)> dfs = [&](int r, int c) {
            if (r < 0 || c < 0 || r >= n || c >= m || b[r][c] != 'O') return;
            b[r][c] = '#';
            dfs(r+1,c); dfs(r-1,c); dfs(r,c+1); dfs(r,c-1);
        };
        for (int i = 0; i < n; i++) { dfs(i, 0); dfs(i, m-1); }
        for (int j = 0; j < m; j++) { dfs(0, j); dfs(n-1, j); }
        for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) {
            if (b[i][j] == 'O') b[i][j] = 'X';
            else if (b[i][j] == '#') b[i][j] = 'O';
        }
    }
};
