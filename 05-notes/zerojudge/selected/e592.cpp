// ZeroJudge e592 (UVa 10142) - Australian Voting (即時第二輪 IRV 模擬)
// 每輪數各票的第一順位(跳過已淘汰者); 得票 >50% 當選; 否則淘汰同票數最少者;
// 若存活者全部同票數則一起當選. 候選人名可含空格. 多組測資間空行分隔.
#include <bits/stdc++.h>
using namespace std;
static string strip(string s){ while(!s.empty()&&(s.back()=='\r'||s.back()=='\n')) s.pop_back(); return s; }
static bool blank(const string&s){ for(char c:s) if(!isspace((unsigned char)c)) return false; return true; }
int main(){
    vector<string> lines; string ln;
    while(getline(cin,ln)) lines.push_back(strip(ln));
    size_t p=0; auto nextNonBlank=[&](){ while(p<lines.size()&&blank(lines[p])) p++; };
    nextNonBlank();
    if(p>=lines.size()) return 0;
    int T=stoi(lines[p++]);
    string out;
    for(int tc=0;tc<T;tc++){
        nextNonBlank();
        int n=stoi(lines[p++]);
        vector<string> name(n+1);
        for(int i=1;i<=n;i++) name[i]=lines[p++];
        vector<vector<int>> ballots;
        while(p<lines.size()&&!blank(lines[p])){
            istringstream is(lines[p++]); vector<int> b; int x;
            while(is>>x) b.push_back(x);
            if(!b.empty()) ballots.push_back(b);
        }
        vector<char> elim(n+1,0);
        vector<int> winners;
        while(true){
            vector<long long> cnt(n+1,0);
            for(auto&b:ballots){ for(int c:b){ if(!elim[c]){ cnt[c]++; break; } } }
            long long B=ballots.size();
            int win=-1;
            for(int i=1;i<=n;i++) if(!elim[i] && 2*cnt[i]>B){ win=i; break; }
            if(win!=-1){ winners.push_back(win); break; }
            long long mn=LLONG_MAX,mx=LLONG_MIN;
            for(int i=1;i<=n;i++) if(!elim[i]){ mn=min(mn,cnt[i]); mx=max(mx,cnt[i]); }
            if(mn==mx){ for(int i=1;i<=n;i++) if(!elim[i]) winners.push_back(i); break; }
            for(int i=1;i<=n;i++) if(!elim[i]&&cnt[i]==mn) elim[i]=1;
        }
        if(tc) out+="\n";              // 測資間空行
        for(int w:winners){ out+=name[w]; out+="\n"; }
    }
    cout<<out;
    return 0;
}
