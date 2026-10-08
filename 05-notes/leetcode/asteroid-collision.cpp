// LeetCode #735 Asteroid Collision (Med)
// Stack: 遇到左移 (負) 跟 stack top (正) 比大小
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> asteroidCollision(vector<int>& ast) {
        vector<int> st;
        for (int a : ast) {
            bool alive = true;
            while (alive && a < 0 && !st.empty() && st.back() > 0) {
                if (st.back() < -a) st.pop_back();
                else if (st.back() == -a) { st.pop_back(); alive = false; }
                else alive = false;
            }
            if (alive) st.push_back(a);
        }
        return st;
    }
};
