// ZeroJudge d445 - 分堆大考驗: 把 {1..N} 分成兩個和相等的子集, 算方法數
// sum=N(N+1)/2; 奇數->0. 數和為 sum/2 的子集個數, 再除以 2 (每種分法被算兩次)
#include <bits/stdc++.h>
using namespace std;
int main(){
    int N;
    while (cin >> N) {
        long long sum = (long long)N * (N + 1) / 2;
        if (sum & 1) { cout << 0 << "\n"; continue; }
        long long target = sum / 2;
        vector<long long> dp(target + 1, 0);
        dp[0] = 1;
        for (int i = 1; i <= N; i++)
            for (long long s = target; s >= i; s--) dp[s] += dp[s - i];
        cout << dp[target] / 2 << "\n";
    }
    return 0;
}
