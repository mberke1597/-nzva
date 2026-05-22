// ============================================================
//  10.2 — Bit Hileleri (Bit Tricks)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 10
//
//  ✅ struct/class YOK → global diziler kullanılır
//  ✅ k. bit okuma, yazma, silme, toggle
//  ✅ LSB izolasyonu, 2^k testi, son 1 biti sıfırlama
//
//  NEDEN KULLANILIR?
//  Koşullu dallanma yerine tek bir bit komutu → hız.
//  Bitmask DP, set işlemleri, alt küme iterasyonunun temeli.
// ============================================================

#include <iostream>
using namespace std;

// ─────────────────────────────────────────────
//  TEK BİT İŞLEMLERİ (Global fonksiyonlar)
// ─────────────────────────────────────────────

// k. biti oku (0 veya 1 döner)
int getBit(int x, int k)    { return (x >> k) & 1; }

// k. biti 1 yap
int setBit(int x, int k)    { return x | (1 << k); }

// k. biti 0 yap
int clearBit(int x, int k)  { return x & ~(1 << k); }

// k. biti tersine çevir (0→1, 1→0)
int toggleBit(int x, int k) { return x ^ (1 << k); }

// ─────────────────────────────────────────────
//  ÖNEMLİ BİT FORMÜLLERI
// ─────────────────────────────────────────────

// En sağdaki 1 biti sıfırla: x & (x-1)
// Kullanım: 2'nin kuvveti testi, popcount döngüsü
int clearLSB(int x)  { return x & (x - 1); }

// Sadece en sağdaki 1 biti tut (LSB isolate): x & (-x)
// Kullanım: Fenwick Tree'de p(k) hesabı
int isolateLSB(int x) { return x & (-x); }

// 2'nin kuvveti mi? (x > 0 şartı da gerekli)
bool isPowerOf2(int x) { return x > 0 && (x & (x - 1)) == 0; }

// En sağdaki 1 bitten sonrasını hepsini 1 yap: x | (x-1)
int fillAfterLSB(int x) { return x | (x - 1); }

// ─────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Tek Bit İşlemleri ──
    cout << "=== Tek Bit İşlemleri (x=0b1010 = 10) ===" << endl;
    int x = 0b1010; // 10
    cout << "  x         = " << x << " (1010)" << endl;
    cout << "  getBit(1) = " << getBit(x, 1) << "  (1. bit = 1)" << endl;
    cout << "  getBit(0) = " << getBit(x, 0) << "  (0. bit = 0)" << endl;
    cout << "  setBit(0) = " << setBit(x, 0) << "  (1011 = 11)" << endl;
    cout << "  clearBit(1)= " << clearBit(x, 1) << " (1000 = 8)" << endl;
    cout << "  toggleBit(3)=" << toggleBit(x, 3) << " (0010 = 2)" << endl;

    // ── LSB ve 2^k Testi ──
    cout << "\n=== LSB ve 2'nin Kuvveti ===" << endl;
    for (int n : {1, 2, 4, 6, 8, 12, 16, 32}) {
        cout << "  n=" << n
             << "\t isolateLSB=" << isolateLSB(n)
             << "\t clearLSB=" << clearLSB(n)
             << "\t 2^k?: " << (isPowerOf2(n) ? "EVET" : "HAYIR")
             << endl;
    }

    // ── clearLSB ile popcount döngüsü ──
    cout << "\n=== popcount — clearLSB döngüsü ===" << endl;
    int val = 0b10110101; // 5 tane 1 biti var
    int count = 0;
    int tmp = val;
    while (tmp) {
        count++;
        tmp = clearLSB(tmp); // Her adımda en sağdaki 1 biti sil
    }
    cout << "  0b10110101 = " << val << ", 1 bit sayısı = " << count
         << "  (builtin=" << __builtin_popcount(val) << ")" << endl;

    // ── Bit hileleri tablosu ──
    cout << "\n=== Bit Hileleri Özet Tablosu ===" << endl;
    cout << "  İşlem                       | Formül          | Örnek" << endl;
    cout << "  ----------------------------|-----------------|-------" << endl;
    int demo = 0b11011010; // 218
    cout << "  k. biti oku                 | (x>>k)&1        | bit3=" << getBit(demo,3) << endl;
    cout << "  k. biti 1 yap               | x|(1<<k)        | " << setBit(demo,2) << endl;
    cout << "  k. biti 0 yap               | x&~(1<<k)       | " << clearBit(demo,3) << endl;
    cout << "  k. biti toggle              | x^(1<<k)        | " << toggleBit(demo,1) << endl;
    cout << "  Son 1 biti sıfırla          | x&(x-1)         | " << clearLSB(demo) << endl;
    cout << "  Sadece son 1'i tut          | x&(-x)          | " << isolateLSB(demo) << endl;
    cout << "  2'nin kuvveti mi?           | x>0&&(x&x-1)==0 | " << isPowerOf2(demo) << endl;
    cout << "  Son 1 bitten sonrasını doldur | x|(x-1)       | " << fillAfterLSB(demo) << endl;

    // ── x & ~(x-1) = isolateLSB kanıtı ──
    cout << "\n=== Neden x & (-x) En Sağdaki 1'i İzole Eder? ===" << endl;
    int y = 0b1011000; // 88
    cout << "  x         = " << y << " (1011000)" << endl;
    cout << "  -x (two's complement) = " << -y << endl;
    cout << "  x & (-x)  = " << (y & (-y)) << "  (en sağdaki 1 = " << __builtin_ctz(y) << ". bit)" << endl;

    return 0;
}
