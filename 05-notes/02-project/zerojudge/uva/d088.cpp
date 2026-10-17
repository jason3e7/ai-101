// ZeroJudge d088 / UVa127 - "Accordian" Patience
// 52 張牌模擬: 頂牌與左1或左3頂牌同花色或同點數可疊; 優先左3, 取最左可動牌
#include <bits/stdc++.h>
using namespace std;
bool match(const string& a, const string& b){ return a[0]==b[0] || a[1]==b[1]; }
void solve(vector<string>& cards){
    vector<vector<string>> p(52);
    for (int i = 0; i < 52; i++) p[i].push_back(cards[i]);
    bool moved = true;
    while (moved) {
        moved = false;
        for (int i = 0; i < (int)p.size(); i++) {
            int dst = -1;
            if (i >= 3 && match(p[i].back(), p[i-3].back())) dst = i - 3;
            else if (i >= 1 && match(p[i].back(), p[i-1].back())) dst = i - 1;
            if (dst >= 0) {
                p[dst].push_back(p[i].back());
                p[i].pop_back();
                if (p[i].empty()) p.erase(p.begin() + i);
                moved = true;
                break; // 重新從左掃
            }
        }
    }
    int n = p.size();
    printf("%d pile%s remaining:", n, n == 1 ? "" : "s");
    for (auto& pile : p) printf(" %d", (int)pile.size());
    printf("\n");
}
int main(){
    string tok;
    vector<string> cards;
    while (cin >> tok) {
        if (tok == "#") break;
        cards.push_back(tok);
        if (cards.size() == 52) { solve(cards); cards.clear(); }
    }
    return 0;
}
