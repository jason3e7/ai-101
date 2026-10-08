// LeetCode #345 Reverse Vowels of a String (Easy)
// 兩指針跳到母音, 交換
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string reverseVowels(string s) {
        auto isV = [](char c) { c = tolower(c); return c=='a'||c=='e'||c=='i'||c=='o'||c=='u'; };
        int l = 0, r = s.size() - 1;
        while (l < r) {
            while (l < r && !isV(s[l])) l++;
            while (l < r && !isV(s[r])) r--;
            swap(s[l++], s[r--]);
        }
        return s;
    }
};
