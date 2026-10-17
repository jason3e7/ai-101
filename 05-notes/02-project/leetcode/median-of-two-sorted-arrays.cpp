// LeetCode #4 Median of Two Sorted Arrays (Hard)
// 二分較短陣列的切點, 用 half 推另一邊切點, O(log min(n,m))
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    double findMedianSortedArrays(vector<int>& A, vector<int>& B) {
        if (A.size() > B.size()) swap(A, B);
        int n = A.size(), m = B.size(), half = (n + m + 1) / 2;
        int lo = 0, hi = n;
        while (lo <= hi) {
            int i = (lo + hi) / 2;
            int j = half - i;
            int Al = i == 0 ? INT_MIN : A[i-1];
            int Ar = i == n ? INT_MAX : A[i];
            int Bl = j == 0 ? INT_MIN : B[j-1];
            int Br = j == m ? INT_MAX : B[j];
            if (Al <= Br && Bl <= Ar) {
                if ((n + m) % 2) return max(Al, Bl);
                return (max(Al, Bl) + min(Ar, Br)) / 2.0;
            } else if (Al > Br) hi = i - 1;
            else lo = i + 1;
        }
        return 0;
    }
};
