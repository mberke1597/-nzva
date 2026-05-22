// ============================================================
//  17.7 — Izgara Kaplama (Counting Tilings — Bitmask DP)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 7
//
//  ✅ struct/class YOK → global diziler kullanılır
//  ✅ n×m ızgarayı 1×2 dominolarla kapla
//  ✅ Bitmask: her satırın durumunu m bitlik sayı ile temsil et
//  ✅ Rekürsif maske geçişleri → global dizi üzerinde
//
//  NEDEN KULLANILIR?
//  Mozaik döşeme sayısı, kimya (benzen molekülü sayımı),
//  kombinatoryal sayım problemleri.
//
//  BITMASK ANLAMI:
//  dp[satır][mask]: Bu satırın profili 'mask' iken kaç yol var?
//  mask'in i. biti:
//    1 → Üst satırdan dikey domino bu hücreye uzanıyor (dolu)
//    0 → Bu hücre boş (yatay domino veya bu satırda dikey başlayacak)
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const long long MOD = 1e9 + 7;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
// ─────────────────────────────────────────────
int  gridN, gridM;    // Izgara boyutları
// dp[r][mask]: r. satırın profilinin 'mask' olduğu kaplama sayısı
// gridM ≤ 20 olmalı (2^20 = 1M): kısa kenarı gridM yap
vector<vector<long long>> dpTile;

// Geçiş maskelerini saklamak için
// transitions[mask] = bu satır 'mask' profilindeyken
//                     alt satırda oluşacak olası 'next_mask' listesi
vector<vector<int>> transitions;

// ─────────────────────────────────────────────
//  GEÇİŞ MASKELERİNİ ÜRET (Rekürsif)
//
//  col: şu an işlenen sütun
//  curMask: üst satırdan gelen taşma profili
//  nextMask: bir alt satıra geçen taşma profili (oluşturuluyor)
// ─────────────────────────────────────────────
void generateTransitions(int col, int curMask, int nextMask) {
    if (col == gridM) {
        // Bir satır tamamlandı → geçişi kaydet
        transitions[curMask].push_back(nextMask);
        return;
    }

    if (curMask & (1 << col)) {
        // Bu hücre üst satırdan dikey domino ile dolu → atla
        generateTransitions(col + 1, curMask, nextMask);
    } else {
        // Seçenek 1: Dikey domino yerleştir (alt satıra taşar)
        generateTransitions(col + 1, curMask, nextMask | (1 << col));

        // Seçenek 2: Yatay domino yerleştir (sol-sağ)
        // Sağ komşu da boş olmalı VE sınır içinde olmalı
        if (col + 1 < gridM && !(curMask & (1 << (col + 1)))) {
            // 2 sütunu birden kapla → col+2'den devam
            generateTransitions(col + 2, curMask, nextMask);
        }
    }
}

// ─────────────────────────────────────────────
//  IZGARA KAPLAMA SAYISI
//
//  Karmaşıklık: O(n × 2^m × m)
//  m ≤ 10 için çok hızlı, m ≤ 20 için uygulanabilir
// ─────────────────────────────────────────────
long long countTilings(int n, int m) {
    // Kısa kenar m olsun (2^m küçük tutmak için)
    if (n < m) swap(n, m);
    gridN = n;
    gridM = m;

    int states = (1 << m); // Toplam maske sayısı

    // Geçişleri hazırla
    transitions.assign(states, {});
    for (int mask = 0; mask < states; mask++) {
        generateTransitions(0, mask, 0);
    }

    // DP tablosu: dp[satır][mask]
    dpTile.assign(n + 1, vector<long long>(states, 0));
    dpTile[0][0] = 1; // Başlangıç: 0. satır, profil=0 (hiç taşma yok)

    for (int r = 0; r < n; r++) {
        for (int mask = 0; mask < states; mask++) {
            if (dpTile[r][mask] == 0) continue;

            // Bu satırın profili 'mask' iken, bir sonraki satırın
            // profili 'nextMask' olabilir
            for (int nextMask : transitions[mask]) {
                dpTile[r + 1][nextMask] =
                    (dpTile[r + 1][nextMask] + dpTile[r][mask]) % MOD;
            }
        }
    }

    // Son satırdan sonra alt satıra dikey taşma olamaz → profil=0
    return dpTile[n][0];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Test 1: 2×2 ızgara ──
    // Yalnızca 2 yatay domino YAN YANA veya 2 dikey domino = 2 yol
    cout << "=== 2×2 Izgara ===" << endl;
    cout << "  Kaplama yolları: " << countTilings(2, 2) << "  (beklenen: 2)" << endl;

    // ── Test 2: 2×4 ızgara ──
    cout << "=== 2×4 Izgara ===" << endl;
    cout << "  Kaplama yolları: " << countTilings(2, 4) << "  (beklenen: 5)" << endl;

    // ── Test 3: 4×4 ızgara ──
    cout << "=== 4×4 Izgara ===" << endl;
    cout << "  Kaplama yolları: " << countTilings(4, 4) << "  (beklenen: 36)" << endl;

    // ── Test 4: Tek sütun ──
    cout << "=== 4×1 Izgara ===" << endl;
    cout << "  Kaplama yolları: " << countTilings(4, 1) << "  (beklenen: 1)" << endl;

    // ── Test 5: 2×8 ──
    cout << "=== 2×8 Izgara ===" << endl;
    cout << "  Kaplama yolları: " << countTilings(2, 8) << "  (beklenen: 34)" << endl;

    // Fibonacci bağlantısı
    cout << "\n=== 2×n için Fibonacci Bağlantısı ===" << endl;
    cout << "  (2×n ızgara yolları = Fibonacci sayısı F(n+1))" << endl;
    for (int n = 1; n <= 8; n++) {
        cout << "  2×" << n << " → " << countTilings(2, n) << endl;
    }

    return 0;
}

/*
 * KARMAŞIKLIK:
 *   Zaman: O(n × 2^m × m)
 *   Uzay:  O(n × 2^m)
 *
 * BITMASK DP'NİN MANTIGI:
 *   - Her satırın durumu m bitlik sayıyla temsil edilir
 *   - Bit=1: üst satırdan taşan dikey domino var
 *   - Bit=0: bu hücre boş (yeni domino başlayacak)
 *   - Geçişler: boş hücreleri dikey veya yatay domino ile doldur
 *
 * 2×n Izgara — Fibonacci Bağlantısı:
 *   f(1)=1, f(2)=2, f(3)=3, f(4)=5, f(5)=8, ...
 *   f(n) = f(n-1) + f(n-2)
 *   Çünkü: son sütun ya dikey domino (f(n-1)) ya yatay çift (f(n-2))
 */
