// ============================================================
//  10.4 — Hamming Mesafesi + Bit Optimizasyonu
//  Kaynak: Competitive Programmer's Handbook, Bölüm 10
//
//  ✅ struct/class YOK → global diziler kullanılır
//  ✅ Naif O(n²k) vs Bit-optimized O(n²k/32)
//  ✅ popcount(a ^ b) ile O(1) mesafe hesabı
//
//  NEDEN KULLANILIR?
//  DNA dizi karşılaştırma, hata düzeltme kodları, ML özellik mesafesi.
//  Bit versiyonu naif versiyondan ~30x hızlı (kitap ölçümü).
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <climits>
using namespace std;

const int MAXN = 10005;

// ─────────────────────────────────────────────
//  NAİF HAMMİNG MESAFESİ O(k) — karakter karşılaştırma
// ─────────────────────────────────────────────
int hammingNaive(const string& a, const string& b) {
    int d = 0;
    for (int i = 0; i < (int)a.size(); i++)
        if (a[i] != b[i]) d++;
    return d;
}

// ─────────────────────────────────────────────
//  BİT OPTİMİZE HAMMİNG MESAFESİ O(1) — XOR + popcount
//
//  MANTIK: a XOR b → sadece farklı pozisyonlar 1 olur
//          popcount(a ^ b) = farklı bit sayısı = Hamming mesafesi
//
//  k ≤ 32: int kullan, k ≤ 64: long long kullan
// ─────────────────────────────────────────────
int hammingBit(int a, int b) {
    return __builtin_popcount(a ^ b);
}

// Bit dizisi olarak saklanmış stringler
int bitStrings[MAXN]; // Her string bir int'e sıkıştırılmış
int N, K;             // N: string sayısı, K: string uzunluğu

// Bit stringi oluştur ("010110" → int)
int toBitInt(const string& s) {
    int result = 0;
    for (int i = 0; i < (int)s.size(); i++)
        if (s[i] == '1') result |= (1 << i);
    return result;
}

// ─────────────────────────────────────────────
//  MİNİMUM HAMMİNG MESAFESİ — tüm çiftler
// ─────────────────────────────────────────────
int minHammingNaive(const vector<string>& strs) {
    int minDist = INT_MAX;
    for (int i = 0; i < (int)strs.size(); i++)
        for (int j = i + 1; j < (int)strs.size(); j++)
            minDist = min(minDist, hammingNaive(strs[i], strs[j]));
    return minDist;
}

int minHammingBit() {
    int minDist = INT_MAX;
    for (int i = 0; i < N; i++)
        for (int j = i + 1; j < N; j++)
            minDist = min(minDist, hammingBit(bitStrings[i], bitStrings[j]));
    return minDist;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Temel test ──
    cout << "=== Hamming Mesafesi ===" << endl;
    string s1 = "01101", s2 = "11001";
    cout << "  \"" << s1 << "\" ile \"" << s2 << "\"" << endl;
    cout << "  Naif: " << hammingNaive(s1, s2) << "  (beklenen: 2)" << endl;
    cout << "  Bit:  " << hammingBit(toBitInt(s1), toBitInt(s2)) << endl;

    // ── Kitap örneği ──
    cout << "\n=== Kitap Örneği: min Hamming [00111, 01101, 11110] ===" << endl;
    vector<string> strs = {"00111", "01101", "11110"};
    N = strs.size(); K = strs[0].size();
    for (int i = 0; i < N; i++) bitStrings[i] = toBitInt(strs[i]);

    cout << "  Naif: " << minHammingNaive(strs) << "  (beklenen: 2)" << endl;
    cout << "  Bit:  " << minHammingBit() << endl;

    // ── Tüm çiftlerin mesafeleri ──
    cout << "\n  Tüm çift mesafeleri:" << endl;
    for (int i = 0; i < N; i++)
        for (int j = i+1; j < N; j++)
            cout << "    hamming(\"" << strs[i] << "\",\"" << strs[j] << "\") = "
                 << hammingBit(bitStrings[i], bitStrings[j]) << endl;

    cout << "\n=== Bit Optimizasyonu Özeti ===" << endl;
    cout << "  Naif:  O(n² × k) — karakter karakter karşılaştır" << endl;
    cout << "  Bit:   O(n² × k/32) — 32 bit birden XOR+popcount" << endl;
    cout << "  Kazanç: ~30x hız artışı (kitap: 13.5s → 0.5s)" << endl;

    return 0;
}
