// ZeroJudge d129 / UVa136 - Ugly Numbers
// 質因數只含 2,3,5 的數; 求第 1500 個. 三指標合併.
#include <cstdio>
#include <algorithm>
using namespace std;
int main(){
    long long u[1500];
    u[0] = 1;
    int i2 = 0, i3 = 0, i5 = 0;
    for (int n = 1; n < 1500; n++) {
        long long c = min({u[i2]*2, u[i3]*3, u[i5]*5});
        u[n] = c;
        if (c == u[i2]*2) i2++;
        if (c == u[i3]*3) i3++;
        if (c == u[i5]*5) i5++;
    }
    printf("The 1500'th ugly number is %lld.\n", u[1499]);
    return 0;
}
