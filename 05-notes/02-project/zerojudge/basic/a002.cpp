// ZeroJudge a002 - 簡易加法
// 每行兩個整數 a b (以空白隔開), 輸出其和; 多組輸入讀到 EOF 為止
#include <iostream>
using namespace std;

int main() {
    long long a, b;
    while (cin >> a >> b) {
        cout << a + b << "\n";
    }
    return 0;
}
