// ZeroJudge e208 - Run-length Decoding: 字母後接出現次數(可多位), 解碼還原
#include <bits/stdc++.h>
using namespace std;
int main(){
    int T; if(!(cin>>T)) return 0;
    for(int t=1;t<=T;t++){
        string s; cin>>s;
        string out;
        for(size_t i=0;i<s.size();){
            char ch=s[i++];
            long long cnt=0;
            while(i<s.size() && isdigit((unsigned char)s[i])) cnt=cnt*10+(s[i++]-'0');
            out.append((size_t)cnt, ch);
        }
        cout<<"Case "<<t<<": "<<out<<"\n";
    }
    return 0;
}
