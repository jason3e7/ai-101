// ZeroJudge c103 / UVa131 - The Psychic Poker Player
// 保留手牌任意子集(其餘用桌牌前 j 張替換), 枚舉 32 種組合取最佳牌型
#include <bits/stdc++.h>
using namespace std;
int rankVal(char c){
    switch(c){ case 'T':return 10; case 'J':return 11; case 'Q':return 12;
               case 'K':return 13; case 'A':return 14; default: return c - '0'; }
}
// 回傳牌型分數 (大=好)
int eval5(vector<string>& h){
    int rv[5]; char suit[5];
    for (int i = 0; i < 5; i++) { rv[i] = rankVal(h[i][0]); suit[i] = h[i][1]; }
    bool flush = true;
    for (int i = 1; i < 5; i++) if (suit[i] != suit[0]) flush = false;
    // 計 rank 次數
    map<int,int> cnt;
    for (int i = 0; i < 5; i++) cnt[rv[i]]++;
    vector<int> uniq;
    for (auto& p : cnt) uniq.push_back(p.first);
    bool straight = false;
    if ((int)uniq.size() == 5) {
        if (uniq.back() - uniq.front() == 4) straight = true;
        // A2345 : {2,3,4,5,14}
        if (uniq == vector<int>{2,3,4,5,14}) straight = true;
    }
    vector<int> counts;
    for (auto& p : cnt) counts.push_back(p.second);
    sort(counts.rbegin(), counts.rend());
    bool four = counts[0] == 4;
    bool three = counts[0] == 3;
    bool full = (counts[0] == 3 && counts.size() > 1 && counts[1] == 2);
    int pairs = 0; for (int c : counts) if (c == 2) pairs++;
    if (straight && flush) return 8;
    if (four) return 7;
    if (full) return 6;
    if (flush) return 5;
    if (straight) return 4;
    if (three) return 3;
    if (pairs == 2) return 2;
    if (pairs == 1) return 1;
    return 0;
}
int main(){
    const char* name[9] = {"highest-card","one-pair","two-pairs","three-of-a-kind",
        "straight","flush","full-house","four-of-a-kind","straight-flush"};
    vector<string> card(10);
    while (cin >> card[0]) {
        for (int i = 1; i < 10; i++) cin >> card[i];
        int best = -1;
        for (int mask = 0; mask < 32; mask++) {
            vector<string> cur;
            for (int i = 0; i < 5; i++) if (mask & (1<<i)) cur.push_back(card[i]); // 保留的手牌
            int need = 5 - cur.size();
            for (int j = 0; j < need; j++) cur.push_back(card[5 + j]); // 補桌牌前 need 張
            best = max(best, eval5(cur));
        }
        printf("Hand:");
        for (int i = 0; i < 5; i++) printf(" %s", card[i].c_str());
        printf(" Deck:");
        for (int i = 5; i < 10; i++) printf(" %s", card[i].c_str());
        printf(" Best hand: %s\n", name[best]);
    }
    return 0;
}
