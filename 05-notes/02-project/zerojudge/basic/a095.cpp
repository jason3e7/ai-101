// ZeroJudge a095 - 麥哲倫的陰謀 (猜帽子歸納): M 頂紅帽
// 紅帽者第 M 天確定離開; 若還有白帽(M<N), 白帽者第 M+1 天才確定 -> 答案 M+1
// 若全是紅帽(M==N), 無白帽須等, 答案為 M
#include <iostream>
using namespace std;

int main() {
    long long n, m;
    while (cin >> n >> m) cout << (m == n ? m : m + 1) << "\n";
    return 0;
}
