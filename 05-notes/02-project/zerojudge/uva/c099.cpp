// ZeroJudge c099 / UVa115 - Climbing Trees
// 家族樹(每人至多一個家長)查關係: 直系 child/parent (great..), 旁系 cousin, sibling
#include <bits/stdc++.h>
using namespace std;
map<string,int> id_;
vector<int> par;
int getId(const string& s){
    auto it = id_.find(s);
    if (it != id_.end()) return it->second;
    int k = par.size(); id_[s] = k; par.push_back(-1); return k;
}
// 回傳 p 到根的祖先鏈 (含 p 自己, index = 邊數距離)
vector<int> chain(int x){ vector<int> c; while(x!=-1){ c.push_back(x); x=par[x]; } return c; }

string lineageWord(int d, const string& base){ // d>=1; base = "child" or "parent"
    if (d == 1) return base;
    string r;
    for (int i = 0; i < d - 2; i++) r += "great ";
    r += "grand " + base;
    return r;
}
int main(){
    string a, b;
    // 讀 child-parent 對, 直到 no.child
    while (cin >> a >> b) {
        if (a == "no.child") break;
        int c = getId(a), p = getId(b);
        par[c] = p;
    }
    string sp, sq;
    while (cin >> sp >> sq) {
        // 名字不存在 => 無關係
        if (!id_.count(sp) || !id_.count(sq)) { printf("no relation\n"); continue; }
        int p = id_[sp], q = id_[sq];
        vector<int> cp = chain(p), cq = chain(q);
        // 找 LCA: cp 由近到遠, 第一個也在 cq 的
        int lca = -1, ep = -1, eq = -1;
        for (int i = 0; i < (int)cp.size() && lca < 0; i++)
            for (int j = 0; j < (int)cq.size(); j++)
                if (cp[i] == cq[j]) { lca = cp[i]; ep = i; eq = j; break; }
        if (lca < 0) { printf("no relation\n"); continue; }
        if (ep == 0 && eq == 0) { printf("no relation\n"); continue; } // 同一人
        if (ep == 0) {            // p 是 q 的祖先 -> parent 類, 距離 eq
            printf("%s\n", lineageWord(eq, "parent").c_str());
        } else if (eq == 0) {     // q 是 p 的祖先 -> p 是 child 類, 距離 ep
            printf("%s\n", lineageWord(ep, "child").c_str());
        } else {                   // 旁系
            int m = ep - 1, n = eq - 1;
            int k = min(m, n), j = abs(m - n);
            if (k == 0 && j == 0) printf("sibling\n");
            else if (j == 0) printf("%d cousin\n", k);
            else printf("%d cousin removed %d\n", k, j);
        }
    }
    return 0;
}
