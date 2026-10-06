// ZeroJudge a554 - NCPC SHA-4 反推: 給 digest 求原始 5 字元訊息
// 前向: a_{i+1}=4a+ (b+c)+e+K+word; b<-a; c<-8b; d<-c; e<-d. 初值 h0..h4 固定.
// 反推: 由最終 e5=8*a1, d5=8*a2, c5=8*a3, b5=a4, a5 解出各 word (word 很小, 0..94).
#include <cstdio>
#include <cstdint>
using namespace std;

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    const uint32_t K = 0x5a82, h0 = 0xdead, h1 = 0xcafe, h2 = 0xbeef, h3 = 0x3399, h4 = 0x7788;
    const uint32_t M29 = (1u << 29) - 1;
    while (T--) {
        uint32_t D0, D1, D2, D3, D4;
        scanf("%x %x %x %x %x", &D0, &D1, &D2, &D3, &D4);
        uint32_t a5 = D0 - h0, b5 = D1 - h1, c5 = D2 - h2, d5 = D3 - h3, e5 = D4 - h4;
        uint32_t base0 = 4*h0 + h1 + h2 + h4 + K;
        uint32_t w0 = ((e5 >> 3) - base0) & M29;
        uint32_t a1 = base0 + w0;
        uint32_t base1 = 4*a1 + h0 + 8*h1 + h3 + K;
        uint32_t w1 = ((d5 >> 3) - base1) & M29;
        uint32_t a2 = base1 + w1;
        uint32_t base2 = 4*a2 + a1 + 8*h0 + h2 + K;
        uint32_t w2 = ((c5 >> 3) - base2) & M29;
        uint32_t a3 = base2 + w2;
        uint32_t base3 = 4*a3 + a2 + 8*a1 + 8*h1 + K;
        uint32_t w3 = b5 - base3;                 // a4 = b5
        uint32_t base4 = 4*b5 + a3 + 8*a2 + 8*h0 + K;
        uint32_t w4 = a5 - base4;
        char M[6] = { (char)(w0+32), (char)(w1+32), (char)(w2+32), (char)(w3+32), (char)(w4+32), 0 };
        printf("%s\n", M);
    }
    return 0;
}
