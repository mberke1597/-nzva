// ============================================================
//  10.7 — Bitmask DP: Asansör / Permütasyon→Subset
//  Kaynak: Competitive Programmer's Handbook, Bölüm 10
//
//  ✅ struct/class YOK → global diziler kullanılır
//  ✅ Permütasyon n! → Subset 2^n dönüşümü
//  ✅ best[] dizisi: {min_tur, son_tur_ağırlığı}
//
//  PROBLEM:
//  n kişi, asansör kapasitesi W. Minimum tur sayısı?
//  Kişiler en optimal sırayla asansöre biner.
//
//  NEDEN BITMASK DP?
//  Sıra değil, küme önemli: n! permütasyon → 2^n subset
//  n=20: 20! = 2.4×10¹⁸  vs  2^20 = 1M
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 20;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
// ─────────────────────────────────────────────
int weight[MAXN]; // Her kişinin ağırlığı
int N;            // Kişi sayısı
int W;            // Asansör kapasitesi

// best[mask] = {min_tur_sayısı, son_turdaki_toplam_ağırlık}
pair<int,int> best[1 << MAXN];

// ─────────────────────────────────────────────
//  ASANSÖR BITMASK DP
//
//  best[S] = S kümesindeki kişileri asansörle taşımak için
//            minimum tur ve son turun ağırlığı
//
//  Geçiş: S kümesindeki son kişi p için:
//    option = best[S ^ (1<<p)]
//    if option.weight + weight[p] ≤ W: p mevcut tura ekle
//    else: p için yeni tur başlat
//    best[S] = min(best[S], option)
//
//  Karmaşıklık: O(2^n × n)
// ─────────────────────────────────────────────
int minRides() {
    int states = 1 << N;

    // Başlangıç: boş küme = 1 tur, ağırlık 0
    best[0] = {1, 0};

    for (int s = 1; s < states; s++) {
        best[s] = {N + 1, 0}; // En kötü durum: N+1 tur

        for (int p = 0; p < N; p++) {
            if (!(s & (1 << p))) continue; // p, s içinde değil

            auto option = best[s ^ (1 << p)]; // p olmadan önceki durum

            if (option.second + weight[p] <= W) {
                // p'yi mevcut tura ekle (ağırlık sığıyor)
                option.second += weight[p];
            } else {
                // p için yeni tur gerekiyor
                option.first++;
                option.second = weight[p];
            }

            best[s] = min(best[s], option); // Daha iyi mi?
        }
    }

    return best[states - 1].first; // Tüm kişiler için min tur
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Kitap örneği ──
    // n=5, W=10, ağırlıklar={2,3,3,5,6}
    // Optimal: {0,2,3} (ağırlık=10) + {1,4} (ağırlık=9) = 2 tur
    cout << "=== Asansör Problemi ===" << endl;
    N = 5; W = 10;
    int w[] = {2, 3, 3, 5, 6};
    for (int i = 0; i < N; i++) weight[i] = w[i];

    cout << "  Kişi sayısı: " << N << ", Kapasite: " << W << endl;
    cout << "  Ağırlıklar: { ";
    for (int i = 0; i < N; i++) cout << weight[i] << " ";
    cout << "}" << endl;

    int result = minRides();
    cout << "  Minimum tur sayısı: " << result << "  (beklenen: 2)" << endl;

    // ── Test 2 ──
    cout << "\n=== Test 2: n=4, W=8 ===" << endl;
    N = 4; W = 8;
    int w2[] = {3, 4, 5, 6};
    for (int i = 0; i < N; i++) weight[i] = w2[i];
    cout << "  Ağırlıklar: {3,4,5,6}, Kapasite=8" << endl;
    cout << "  Minimum tur: " << minRides() << "  (beklenen: 3)" << endl;

    return 0;
}
