// LeetCode #36 Valid Sudoku (Med)
// 三個 9x9 bitmask (row/col/box), 任一重複即 false
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        int row[9][9] = {0}, col[9][9] = {0}, box[9][9] = {0};
        for (int i = 0; i < 9; i++) for (int j = 0; j < 9; j++) {
            if (board[i][j] == '.') continue;
            int d = board[i][j] - '1';
            int b = (i/3)*3 + j/3;
            if (row[i][d] || col[j][d] || box[b][d]) return false;
            row[i][d] = col[j][d] = box[b][d] = 1;
        }
        return true;
    }
};
