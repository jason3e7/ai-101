// ZeroJudge a671 / UVa113 - Power of Cryptography
// 求 k = p^(1/n), 保證 k 為整數且 k<=1e9. double pow 精度足夠.
#include <cstdio>
#include <cmath>
int main(){
    double n, p;
    while (scanf("%lf %lf", &n, &p) == 2)
        printf("%.0lf\n", pow(p, 1.0 / n));
    return 0;
}
