// ============================================================
//  10.3 — Bitmask ile Küme Temsili (Set Representation)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 10
//
//  ✅ struct/class YOK → global diziler kullanılır
//  ✅ Küme oluşturma, eleman ekleme/silme, sorgulama
//  ✅ Kesişim, Birleşim, Fark, Tümleyen
//  ✅ Alt küme iterasyonu
//
//  NEDEN KULLANILIR?
//  n ≤ 30 küme elemanı → tek int ile temsil et.
//  Küme işlemleri O(1), normal set/unordered_set'ten çok hızlı.
//  Bitmask DP'nin zorunlu temel bilgisi.
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

// ─────────────────────────────────────────────
//  KÜME İŞLEM FONKSİYONLARI (Global, inline)
// ─────────────────────────────────────────────

int  setAdd(int s, int k)    { return s | (1 << k); }      // k'yı ekle
int  setRemove(int s, int k) { return s & ~(1 << k); }     // k'yı sil
bool setHas(int s, int k)    { return (s >> k) & 1; }      // k ∈ s?
int  setSize(int s)          { return __builtin_popcount(s);}
int  setUnion(int a, int b)  { return a | b; }             // a ∪ b
int  setIntersect(int a, int b) { return a & b; }          // a ∩ b
int  setDiff(int a, int b)   { return a & (~b); }          // a \ b

// Kümedeki tüm elemanları yazdır
void printSet(int s, const string& name = "S") {
    cout << "  " << name << " = { ";
    for (int i = 0; i < 32; i++)
        if (setHas(s, i)) cout << i << " ";
    cout << "}  (mask=" << s << ", |" << name << "|=" << setSize(s) << ")" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Kitap örneği: {1,3,4,8} ──
    cout << "=== Kitap Örneği: {1,3,4,8} ===" << endl;
    int x = 0;
    x = setAdd(x, 1);
    x = setAdd(x, 3);
    x = setAdd(x, 4);
    x = setAdd(x, 8);
    printSet(x, "x");
    cout << "  Tek satırda: (1<<1)|(1<<3)|(1<<4)|(1<<8) = "
         << ((1<<1)|(1<<3)|(1<<4)|(1<<8)) << endl;

    // ── Küme İşlemleri ──
    cout << "\n=== Küme İşlemleri ===" << endl;
    int setA = (1<<1)|(1<<3)|(1<<4)|(1<<8); // {1,3,4,8}
    int setB = (1<<3)|(1<<6)|(1<<8)|(1<<9); // {3,6,8,9}
    printSet(setA, "A");
    printSet(setB, "B");
    printSet(setUnion(setA, setB),     "A∪B");
    printSet(setIntersect(setA, setB), "A∩B");
    printSet(setDiff(setA, setB),      "A\\B");

    // ── Eleman sorgulama ──
    cout << "\n=== Eleman Sorgulama ===" << endl;
    for (int k : {1, 3, 5, 8}) {
        cout << "  " << k << " ∈ A? " << (setHas(setA, k) ? "EVET" : "HAYIR") << endl;
    }

    // ── Tüm alt kümeleri gez ──
    cout << "\n=== {0,1,2}'nin Tüm Alt Kümeleri ===" << endl;
    int n = 3;
    for (int b = 0; b < (1 << n); b++) {
        cout << "  mask=" << b << ": { ";
        for (int i = 0; i < n; i++)
            if (b & (1 << i)) cout << i << " ";
        cout << "}" << endl;
    }

    // ── Tam olarak k elemanlı alt kümeler ──
    cout << "\n=== {0,1,2,3}'ün Tam 2 Elemanlı Alt Kümeleri ===" << endl;
    n = 4; int k = 2;
    for (int b = 0; b < (1 << n); b++) {
        if (__builtin_popcount(b) == k) {
            cout << "  { ";
            for (int i = 0; i < n; i++)
                if (b & (1 << i)) cout << i << " ";
            cout << "}" << endl;
        }
    }

    // ── x'in alt kümelerini gez ──
    cout << "\n=== {1,3,8}'in Alt Kümeleri ===" << endl;
    int bigSet = (1<<1)|(1<<3)|(1<<8);
    int b = 0;
    do {
        cout << "  { ";
        for (int i = 0; i < 10; i++)
            if (b & (1 << i)) cout << i << " ";
        cout << "}" << endl;
    } while (b = (b - bigSet) & bigSet);

    return 0;
}

/*
 * ÖZET:
 *   Küme işlemleri O(1) — tek CPU komutu
 *   n ≤ 30: int kullan
 *   n ≤ 60: long long kullan
 *   n > 60: bitset<N> kullan
 *
 *   Alt küme iterasyonu: O(2^n)
 *   x'in alt kümelerini gez: O(2^|x|)
 */
