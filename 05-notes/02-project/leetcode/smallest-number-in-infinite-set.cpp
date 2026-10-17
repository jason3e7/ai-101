// LeetCode #2336 Smallest Number in Infinite Set (Med)
// 用 next 追蹤下一個未發出的, set 存 addBack 後小於 next 的
#include <bits/stdc++.h>
using namespace std;

class SmallestInfiniteSet {
    set<int> added;
    int next = 1;
public:
    SmallestInfiniteSet() {}
    int popSmallest() {
        if (!added.empty()) { int x = *added.begin(); added.erase(added.begin()); return x; }
        return next++;
    }
    void addBack(int num) {
        if (num < next) added.insert(num);
    }
};
