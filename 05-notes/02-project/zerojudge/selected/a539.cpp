// ZeroJudge a539 (UVa 10327) - 相鄰交換排序最少次數 = 逆序數對. N<=1000 用 O(N^2).
#include <cstdio>
#include <vector>
using namespace std;
int main(){
    int n;
    while(scanf("%d",&n)==1){
        vector<int> a(n);
        for(int i=0;i<n;i++) scanf("%d",&a[i]);
        long long inv=0;
        for(int i=0;i<n;i++) for(int j=i+1;j<n;j++) if(a[i]>a[j]) inv++;
        printf("Minimum exchange operations : %lld\n", inv);
    }
    return 0;
}
