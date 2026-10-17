// LeetCode #43 Multiply Strings (Med)
// 直式乘法, p[i+j+1] 存低位, p[i+j] 存進位
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string multiply(string a, string b) {
        if (a == "0" || b == "0") return "0";
        int n = a.size(), m = b.size();
        vector<int> p(n+m, 0);
        for (int i = n-1; i >= 0; i--) for (int j = m-1; j >= 0; j--) {
            int mul = (a[i]-'0') * (b[j]-'0') + p[i+j+1];
            p[i+j+1] = mul % 10;
            p[i+j] += mul / 10;
        }
        string s;
        for (int d : p) if (!(s.empty() && d == 0)) s.push_back('0' + d);
        return s;
    }
};
