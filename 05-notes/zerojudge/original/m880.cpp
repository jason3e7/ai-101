// ZeroJudge m880 - 抽鬼牌: 依序交換後輸出牌組, 並找出拿到 Joker 的座號
#include <bits/stdc++.h>
using namespace std;
int main(){
    vector<string> deck(14);
    for (auto& c : deck) cin >> c;
    int N; cin >> N;
    while (N--) { int a,b; cin >> a >> b; swap(deck[a], deck[b]); }
    vector<int> take(14);
    for (auto& t : take) cin >> t;
    for (int i=0;i<14;i++) cout << deck[i] << (i<13?' ':'\n');
    for (int i=0;i<14;i++) if (deck[take[i]]=="Joker") { cout << (i+1) << "\n"; break; }
    return 0;
}
