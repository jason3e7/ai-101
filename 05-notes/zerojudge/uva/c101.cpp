// ZeroJudge c101 / UVa122 - Trees on the level
// 由 (值,路徑) 建二元樹, 階層走訪; 重複或缺節點 -> not complete
#include <bits/stdc++.h>
using namespace std;
struct Node { int val = 0; bool has = false; Node* ch[2] = {nullptr, nullptr}; };
int main(){
    string tok;
    Node* root = new Node();
    bool bad = false, any = false;
    auto finish = [&](){
        bool ok = !bad;
        vector<int> order;
        if (ok) {
            queue<Node*> q; q.push(root);
            while (!q.empty()) {
                Node* u = q.front(); q.pop();
                if (!u->has) { ok = false; break; }
                order.push_back(u->val);
                if (u->ch[0]) q.push(u->ch[0]);
                if (u->ch[1]) q.push(u->ch[1]);
            }
        }
        if (ok) {
            for (size_t i = 0; i < order.size(); i++) printf("%d%c", order[i], i+1<order.size()?' ':'\n');
        } else printf("not complete\n");
        // reset
        root = new Node(); bad = false; any = false;
    };
    while (cin >> tok) {
        if (tok == "()") { finish(); continue; }
        any = true;
        // 解析 (val,path)
        string content = tok.substr(1, tok.size() - 2);
        size_t comma = content.find(',');
        int val = atoi(content.substr(0, comma).c_str());
        string path = content.substr(comma + 1);
        Node* cur = root;
        for (char c : path) {
            int d = (c == 'L') ? 0 : 1;
            if (!cur->ch[d]) cur->ch[d] = new Node();
            cur = cur->ch[d];
        }
        if (cur->has) bad = true;      // 同路徑重複
        else { cur->has = true; cur->val = val; }
    }
    return 0;
}
