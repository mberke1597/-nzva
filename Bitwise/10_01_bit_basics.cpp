// ============================================================
//  10.1 — Temel Bit Operasyonları (Bit Basics)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 10
//
//  ✅ struct/class YOK → global diziler kullanılır
//  ✅ AND, OR, XOR, NOT, Shift
//  ✅ __builtin_popcount, clz, ctz, parity
//  ✅ Bit gösterimi yazdırma
//
//  NEDEN BIT OPERASYONLARI?
//  Donanım seviyesinde tek CPU komutu = en hızlı işlem.
//  Küme işlemleri, hız optimizasyonu, bitmask DP'nin temeli.
// ============================================================

#include <iostream>
#include <string>
using namespace std;

// ─────────────────────────────────────────────
//  Sayının bit gösterimini yazdır (32 bit)
// ─────────────────────────────────────────────
void printBits(int x, const string& label = "") {
    if (!label.empty()) cout << "  " << label << ": ";
    for (int i = 31; i >= 0; i--) {
        cout << ((x >> i) & 1);
        if (i % 4 == 0 && i > 0) cout << " "; // Okunabilirlik için grupla
    }
    cout << "  (" << x << ")" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── AND ──────────────────────────────────
    cout << "=== AND (&) ===" << endl;
    cout << "  Kural: Her ikisi de 1 ise 1" << endl;
    int a = 22, b = 26;
    printBits(a, "22");
    printBits(b, "26");
    printBits(a & b, "22&26");
    cout << "  22 & 26 = " << (a & b) << "  (beklenen: 18)" << endl;

    // ── OR ───────────────────────────────────
    cout << "\n=== OR (|) ===" << endl;
    cout << "  Kural: En az biri 1 ise 1" << endl;
    printBits(a | b, "22|26");
    cout << "  22 | 26 = " << (a | b) << "  (beklenen: 30)" << endl;

    // ── XOR ──────────────────────────────────
    cout << "\n=== XOR (^) ===" << endl;
    cout << "  Kural: Tam olarak biri 1 ise 1" << endl;
    printBits(a ^ b, "22^26");
    cout << "  22 ^ 26 = " << (a ^ b) << "  (beklenen: 12)" << endl;
    cout << "  XOR özelliği: a^b^b = a → " << (a ^ b ^ b) << "  (=22)" << endl;

    // ── NOT ──────────────────────────────────
    cout << "\n=== NOT (~) ===" << endl;
    cout << "  Kural: ~x = -x - 1" << endl;
    cout << "  ~29 = " << ~29 << "  (beklenen: -30)" << endl;

    // ── BIT SHIFT ────────────────────────────
    cout << "\n=== Bit Shift (<< ve >>) ===" << endl;
    cout << "  14 << 2 = " << (14 << 2) << "  (14 × 4 = 56)" << endl;
    cout << "  49 >> 3 = " << (49 >> 3) << "  (49 / 8 = 6)" << endl;
    cout << "  1 << k  = 2^k: ";
    for (int k = 0; k <= 10; k++) cout << (1 << k) << " ";
    cout << endl;

    // ── GCC BUILTIN FONKSIYONLAR ──────────────
    cout << "\n=== GCC __builtin Fonksiyonlar ===" << endl;
    int x = 5328; // 00000000000000000001010011010000
    printBits(x, "5328");
    cout << "  __builtin_clz(5328)      = " << __builtin_clz(x)
         << "  (baştaki sıfır sayısı)" << endl;
    cout << "  __builtin_ctz(5328)      = " << __builtin_ctz(x)
         << "  (sondaki sıfır sayısı)" << endl;
    cout << "  __builtin_popcount(5328) = " << __builtin_popcount(x)
         << "  (1 bit sayısı)" << endl;
    cout << "  __builtin_parity(5328)   = " << __builtin_parity(x)
         << "  (1 sayısı tek mi? 1=tek)" << endl;

    // ── TEK/ÇİFT KONTROLÜ ────────────────────
    cout << "\n=== Pratik Uygulamalar ===" << endl;
    for (int n : {7, 8, 15, 16}) {
        cout << "  " << n << " " << (n & 1 ? "tek" : "çift")
             << " | 2'nin kuvveti mi? " << (n > 0 && (n & (n-1)) == 0 ? "EVET" : "HAYIR")
             << endl;
    }

    // ── SIGNED vs UNSIGNED ────────────────────
    cout << "\n=== Signed vs Unsigned ===" << endl;
    int sx = -43;
    unsigned int ux = sx;
    cout << "  int x = -43    → " << sx << endl;
    cout << "  unsigned int x = " << ux << "  (= 2^32 - 43)" << endl;

    return 0;
}

/*
 * TEMEL FORMÜLLER:
 *   x & 1    → tek mi?
 *   x >> k   → x / 2^k
 *   x << k   → x * 2^k
 *   ~x       → -x - 1
 *   x ^ x    → 0 (kendisiyle XOR = sıfır)
 *   x ^ 0    → x (sıfırla XOR = kendisi)
 */
