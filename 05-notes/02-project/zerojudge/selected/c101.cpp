// ZeroJudge c101 (UVa 122) - Trees on the level
// 讀 (val,path) token 直到 "()" 結束一組. 依 path(L/R) 建二元樹, 設值.
// not complete 條件: 同一路徑給了 >1 次(重複), 或 BFS 時走到有建立卻沒設值的節點.
#include <bits/stdc++.h>
using namespace std;
struct Node{ long long val=0; bool has=false; int l=-1,r=-1; };
int main(){
    vector<Node> t;
    string tok;
    auto reset=[&](){ t.clear(); t.push_back(Node()); }; // index 0 = root
    reset();
    bool err=false;
    vector<string> outAll;
    auto finish=[&](){
        bool complete=!err;
        vector<long long> vals;
        if(complete){
            // BFS from root (0)
            queue<int> q; q.push(0);
            while(!q.empty()){
                int u=q.front(); q.pop();
                if(!t[u].has){ complete=false; break; }
                vals.push_back(t[u].val);
                if(t[u].l!=-1) q.push(t[u].l);
                if(t[u].r!=-1) q.push(t[u].r);
            }
        }
        if(complete){
            string s;
            for(size_t i=0;i<vals.size();i++){ if(i)s+=' '; s+=to_string(vals[i]); }
            outAll.push_back(s);
        } else outAll.push_back("not complete");
        reset(); err=false;
    };
    while(cin>>tok){
        if(tok=="()"){ finish(); continue; }
        // parse (val,path)
        // strip parens
        string body=tok.substr(1, tok.size()-2);
        size_t comma=body.find(',');
        long long val=stoll(body.substr(0,comma));
        string path=body.substr(comma+1);
        int cur=0;
        for(char ch: path){
            if(ch=='L'){ if(t[cur].l==-1){ t[cur].l=t.size(); t.push_back(Node()); } cur=t[cur].l; }
            else if(ch=='R'){ if(t[cur].r==-1){ t[cur].r=t.size(); t.push_back(Node()); } cur=t[cur].r; }
        }
        if(t[cur].has) err=true;      // 重複給同一節點
        t[cur].val=val; t[cur].has=true;
    }
    // 若最後一組沒有 () 結尾也輸出(保險); 題目保證有 () 則此組為空不觸發
    for(auto&s:outAll) cout<<s<<"\n";
    return 0;
}
