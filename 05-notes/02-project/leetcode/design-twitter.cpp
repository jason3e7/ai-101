// LeetCode #355 Design Twitter (Med)
// tweets[u] 時間序排; getNewsFeed max-heap 自己 + 關注者各丟 10 條合併取 top-10
#include <bits/stdc++.h>
using namespace std;

class Twitter {
    int ts = 0;
    unordered_map<int, vector<pair<int,int>>> tweets;
    unordered_map<int, unordered_set<int>> follows;
public:
    Twitter() {}
    void postTweet(int userId, int tweetId) { tweets[userId].push_back({ts++, tweetId}); }
    vector<int> getNewsFeed(int userId) {
        priority_queue<pair<int,int>> pq;
        auto add = [&](int u) {
            auto& v = tweets[u];
            for (int i = (int)v.size() - 1, cnt = 0; i >= 0 && cnt < 10; i--, cnt++)
                pq.push(v[i]);
        };
        add(userId);
        for (int f : follows[userId]) if (f != userId) add(f);
        vector<int> res;
        while (!pq.empty() && (int)res.size() < 10) { res.push_back(pq.top().second); pq.pop(); }
        return res;
    }
    void follow(int a, int b) { follows[a].insert(b); }
    void unfollow(int a, int b) { follows[a].erase(b); }
};
