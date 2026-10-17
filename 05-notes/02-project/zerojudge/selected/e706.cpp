// ZeroJudge e706 - Cool word: >=2 相異字母 且 各字母出現次數皆相異
#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    int tc=0;
    while(scanf("%d",&n)==1){
        long long cool=0;
        for(int i=0;i<n;i++){
            char buf[64]; scanf("%s",buf);
            int cnt[26]={0};
            for(char*p=buf;*p;p++) cnt[*p-'a']++;
            vector<int> f;
            for(int c=0;c<26;c++) if(cnt[c]) f.push_back(cnt[c]);
            bool ok = f.size()>=2;
            if(ok){ set<int> s(f.begin(),f.end()); if(s.size()!=f.size()) ok=false; }
            if(ok) cool++;
        }
        printf("Case %d: %lld\n", ++tc, cool);
    }
    return 0;
}
