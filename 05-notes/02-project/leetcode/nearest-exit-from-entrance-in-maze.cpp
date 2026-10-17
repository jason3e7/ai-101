// LeetCode #1926 Nearest Exit from Entrance in Maze (Med)
// BFS 從入口找最近邊界 (非入口本身)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int nearestExit(vector<vector<char>>& maze, vector<int>& entrance) {
        int m = maze.size(), n = maze[0].size();
        queue<tuple<int,int,int>> q;
        q.push({entrance[0], entrance[1], 0});
        maze[entrance[0]][entrance[1]] = '+';
        int dr[4] = {-1,1,0,0}, dc[4] = {0,0,-1,1};
        while (!q.empty()) {
            auto [r, c, d] = q.front(); q.pop();
            for (int k = 0; k < 4; k++) {
                int nr = r + dr[k], nc = c + dc[k];
                if (nr<0||nr>=m||nc<0||nc>=n||maze[nr][nc]=='+') continue;
                if (nr==0||nr==m-1||nc==0||nc==n-1) return d + 1;
                maze[nr][nc] = '+';
                q.push({nr, nc, d + 1});
            }
        }
        return -1;
    }
};
