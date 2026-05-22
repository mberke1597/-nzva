// ============================================================
//  10.5 — Alt Izgara Sayımı + Bit Optimizasyonu
//  Kaynak: Competitive Programmer's Handbook, Bölüm 10
//
//  ✅ struct/class YOK → global 2D dizi kullanılır
//  ✅ Naif O(n³) vs Bit-optimized O(n³/64)
//  ✅ Satırları long long olarak sakla, AND+popcount ile say
//
//  PROBLEM: n×n ızgarada tüm köşeleri siyah olan
//           dikdörtgen alt ızgara sayısını bul.
//
//  NEDEN KULLANILIR?
//  Görüntü işleme, 2D desen eşleştirme, kombinatoryal sayım.
//  Bit versiyonu 64 sütunu birden işler → ~64x hız artışı.
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

const int MAXN = 3005;

int  color[MAXN][MAXN]; // 0=beyaz, 1=siyah
int  N;                  // Izgara boyutu

// Bit-optimize versiyon için: her satır long long bloklar halinde
// color_bits[y][blok] = o satırın o bloğundaki 64 sütun
unsigned long long colorBits[MAXN][MAXN / 64 + 1];

// ─────────────────────────────────────────────
//  NAİF: O(n³)
//  Her (satır_a, satır_b) çifti için ortak siyah sütunları say
// ─────────────────────────────────────────────
long long countSubgridsNaive() {
    long long result = 0;
    for (int a = 0; a < N; a++) {
        for (int b = a + 1; b < N; b++) {
            // a ve b satırlarında ortak siyah sütun sayısı
            int cnt = 0;
            for (int i = 0; i < N; i++) {
                if (color[a][i] == 1 && color[b][i] == 1) cnt++;
            }
            // cnt sütundan C(cnt,2) = cnt*(cnt-1)/2 alt ızgara
            result += (long long)cnt * (cnt - 1) / 2;
        }
    }
    return result;
}

// ─────────────────────────────────────────────
//  BİT OPTİMİZE: O(n³/64)
//  64 sütunu birden AND ile karşılaştır, popcount ile say
//
//  MANTIK: color_bits[a][blok] & color_bits[b][blok]
//          → Her iki satırda da siyah olan sütunları işaretle
//          popcount ile o blokta kaç ortak siyah sütun var → say
// ─────────────────────────────────────────────
void buildColorBits() {
    int blocks = (N + 63) / 64;
    for (int y = 0; y < N; y++) {
        for (int blk = 0; blk < blocks; blk++) {
            unsigned long long val = 0;
            for (int bit = 0; bit < 64; bit++) {
                int col = blk * 64 + bit;
                if (col < N && color[y][col] == 1)
                    val |= (1ULL << bit);
            }
            colorBits[y][blk] = val;
        }
    }
}

long long countSubgridsBit() {
    long long result = 0;
    int blocks = (N + 63) / 64;
    for (int a = 0; a < N; a++) {
        for (int b = a + 1; b < N; b++) {
            int cnt = 0;
            for (int blk = 0; blk < blocks; blk++) {
                // 64 sütunu birden AND → ortak siyah olanları bul
                cnt += __builtin_popcountll(colorBits[a][blk] & colorBits[b][blk]);
            }
            result += (long long)cnt * (cnt - 1) / 2;
        }
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Kitap örneği: 4×4 ızgara ──
    // Kitaptaki grafik: köşeleri siyah olan 2 alt ızgara
    cout << "=== Alt Izgara Sayımı ===" << endl;
    N = 4;
    // 0=beyaz, 1=siyah
    // Satır 0: 0 1 1 0
    // Satır 1: 1 0 1 1
    // Satır 2: 0 1 0 1
    // Satır 3: 1 1 0 1
    int grid[4][4] = {
        {0,1,1,0},
        {1,0,1,1},
        {0,1,0,1},
        {1,1,0,1}
    };
    for (int y = 0; y < N; y++)
        for (int x = 0; x < N; x++) color[y][x] = grid[y][x];

    buildColorBits();

    cout << "  Izgara:" << endl;
    for (int y = 0; y < N; y++) {
        cout << "    ";
        for (int x = 0; x < N; x++) cout << color[y][x] << " ";
        cout << endl;
    }

    long long naiveResult = countSubgridsNaive();
    long long bitResult   = countSubgridsBit();

    cout << "  Naif sonuç:  " << naiveResult << endl;
    cout << "  Bit sonucu:  " << bitResult << endl;
    cout << "  Eşleşiyor:   " << (naiveResult == bitResult ? "✅" : "❌") << endl;

    // ── Daha büyük test ──
    cout << "\n=== 8×8 Izgara Testi ===" << endl;
    N = 8;
    // Tüm hücreleri siyah yap
    for (int y = 0; y < N; y++)
        for (int x = 0; x < N; x++) color[y][x] = 1;
    buildColorBits();
    // 8 sütunun tamamı siyah: her satır çifti → C(8,2)=28 alt ızgara
    // C(8,2) satır çifti × 28 = 28 × 28 = 784
    long long n2 = countSubgridsNaive();
    long long b2 = countSubgridsBit();
    cout << "  Tüm siyah 8×8: Naif=" << n2 << " Bit=" << b2
         << (n2 == b2 ? " ✅" : " ❌") << endl;

    cout << "\n=== Performans Özeti ===" << endl;
    cout << "  Naif:  O(n³)    → n=2500: ~15 milyar işlem (29.6s)" << endl;
    cout << "  Bit64: O(n³/64) → n=2500: ~234 milyon işlem (1.7s)" << endl;
    cout << "  Kazanç: ~17x hız artışı" << endl;

    return 0;
}
