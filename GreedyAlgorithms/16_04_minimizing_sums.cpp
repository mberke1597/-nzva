// ============================================================
//  16.4 — Toplamları Minimize Etme (Minimizing Sums)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 6
//
//  ✅ struct/class YOK → global diziler kullanılır
//  ✅ c=1: Medyan, c=2: Aritmetik Ortalama
//  ✅ Her iki durumun matematiksel kanıtı açıklanır
//
//  PROBLEM: Σ |aᵢ - x|^c toplamını minimize eden x'i bul.
//
//  KULLANIM ALANLARI:
//  - ML: K-means clustering merkezi hesaplama
//  - Lojistik: Teslim merkezi/depo konumu seçimi
//  - İstatistik: Merkezi eğilim ölçüleri
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
using namespace std;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
// ─────────────────────────────────────────────
const int MAXN = 100005;
int  arr[MAXN]; // Giriş dizisi
int  N;         // Eleman sayısı

// ─────────────────────────────────────────────
//  1) c = 1: MUTLAK FARK TOPLAMI → MEDYAN
//
//  NEDEN MEDYAN?
//  x medyandan sola kayarsa: sol yarı uzar, sağ yarı kısalır.
//  Sağdaki eleman sayısı > soldaki → toplam artar.
//  Tam medyanda denge en iyi.
//
//  Çift sayıda eleman varsa: ortadaki iki elemanın HERHANGİ BİRİ optimal.
//
//  Karmaşıklık: O(n log n) sıralama
// ─────────────────────────────────────────────
double getMedian() {
    vector<int> sorted(arr, arr + N);
    sort(sorted.begin(), sorted.end());

    if (N % 2 == 1) {
        return sorted[N / 2]; // Tek sayıda: tam ortadaki
    } else {
        // Çift sayıda: ortadaki iki eleman arasında HERHANGİ BİR x optimal
        // (dahil). Genellikle alt medyanı döneriz.
        return (sorted[N / 2 - 1] + sorted[N / 2]) / 2.0;
    }
}

long long sumAbsDiff(double x) {
    long long total = 0;
    for (int i = 0; i < N; i++) total += (long long)abs(arr[i] - x);
    return total;
}

// ─────────────────────────────────────────────
//  2) c = 2: KARE FARK TOPLAMI → ORTALAMA
//
//  NEDEN ORTALAMA?
//  f(x) = Σ (aᵢ - x)² = nx² - 2x·Σaᵢ + Σaᵢ²
//  Bu bir parabol (yukarı açık) → minimum türev=0 noktasında:
//  f'(x) = 2nx - 2Σaᵢ = 0 → x = Σaᵢ / n (aritmetik ortalama)
//
//  Karmaşıklık: O(n)
// ─────────────────────────────────────────────
double getMean() {
    long long sum = 0;
    for (int i = 0; i < N; i++) sum += arr[i];
    return (double)sum / N;
}

double sumSquareDiff(double x) {
    double total = 0;
    for (int i = 0; i < N; i++) {
        double diff = arr[i] - x;
        total += diff * diff;
    }
    return total;
}

// ─────────────────────────────────────────────
//  GENEL c İÇİN BRUTE FORCE DOĞRULAMA
//  (yalnızca küçük testler için)
// ─────────────────────────────────────────────
double bruteForceOptimalX(int c, double xMin, double xMax, double step = 0.1) {
    double bestX   = xMin;
    double bestVal = 1e18;

    for (double x = xMin; x <= xMax; x += step) {
        double val = 0;
        for (int i = 0; i < N; i++) {
            val += pow(abs(arr[i] - x), c);
        }
        if (val < bestVal) {
            bestVal = val;
            bestX   = x;
        }
    }
    return bestX;
}

// ─────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Test 1: Medyan (c=1) ──
    // a = {1, 3, 5, 7, 9} → Medyan = 5
    cout << "=== c=1: Mutlak Fark Toplamı (Medyan) ===" << endl;
    N = 5;
    int a1[] = {1, 3, 5, 7, 9};
    copy(a1, a1 + N, arr);

    double median = getMedian();
    cout << "  Dizi: { ";
    for (int i = 0; i < N; i++) cout << arr[i] << " ";
    cout << "}" << endl;
    cout << "  Optimal x (Medyan) = " << median << endl;
    cout << "  Σ|aᵢ - x| = " << sumAbsDiff(median) << endl;

    // Medyandan sapma kontrolü
    cout << "  Kontrol: x=" << median - 1 << " → Σ=" << sumAbsDiff(median - 1)
         << "  (daha büyük olmalı)" << endl;
    cout << "  Kontrol: x=" << median + 1 << " → Σ=" << sumAbsDiff(median + 1)
         << "  (daha büyük olmalı)" << endl;

    // ── Test 2: Çift sayıda eleman (c=1) ──
    cout << "\n=== c=1: Çift Sayıda Eleman ===" << endl;
    N = 4;
    int a2[] = {2, 4, 6, 8};
    copy(a2, a2 + N, arr);

    double median2 = getMedian();
    cout << "  Dizi: { ";
    for (int i = 0; i < N; i++) cout << arr[i] << " ";
    cout << "}" << endl;
    cout << "  Ortadaki iki eleman: 4 ve 6 → HERHANGİ BİR x ∈ [4,6] optimal" << endl;
    cout << "  Σ|aᵢ - 4| = " << sumAbsDiff(4) << endl;
    cout << "  Σ|aᵢ - 5| = " << sumAbsDiff(5) << endl;
    cout << "  Σ|aᵢ - 6| = " << sumAbsDiff(6) << " (hepsi eşit!)" << endl;

    // ── Test 3: Ortalama (c=2) ──
    cout << "\n=== c=2: Kare Fark Toplamı (Ortalama) ===" << endl;
    N = 5;
    int a3[] = {1, 3, 5, 7, 9};
    copy(a3, a3 + N, arr);

    double mean = getMean();
    cout << "  Dizi: { ";
    for (int i = 0; i < N; i++) cout << arr[i] << " ";
    cout << "}" << endl;
    cout << "  Optimal x (Ortalama) = " << mean << endl;
    cout << "  Σ(aᵢ - x)² = " << sumSquareDiff(mean) << endl;
    cout << "  Kontrol: x=" << mean - 1 << " → Σ=" << sumSquareDiff(mean - 1)
         << "  (daha büyük)" << endl;
    cout << "  Kontrol: x=" << mean + 1 << " → Σ=" << sumSquareDiff(mean + 1)
         << "  (daha büyük)" << endl;

    // ── Test 4: Depo konumu problemi ──
    cout << "\n=== Depo Konumu Problemi ===" << endl;
    cout << "  Müşteri konumları (1D): { 1, 4, 6, 10, 12 }" << endl;
    N = 5;
    int positions[] = {1, 4, 6, 10, 12};
    copy(positions, positions + N, arr);

    double optC1 = getMedian();
    double optC2 = getMean();

    cout << "  c=1 (toplam mesafe minimize): x = " << optC1
         << ", toplam = " << sumAbsDiff(optC1) << endl;
    cout << "  c=2 (kare mesafe minimize):   x = " << optC2
         << ", toplam = " << sumSquareDiff(optC2) << endl;

    return 0;
}

/*
 * KARMAŞIKLIK:
 *   c=1 (Medyan): O(n log n) sıralama
 *   c=2 (Ortalama): O(n) toplam hesaplama
 *
 * FORMÜLLER:
 *   c=1: x* = medyan(a₁, ..., aₙ)
 *   c=2: x* = (1/n) · Σaᵢ
 *
 * KULLANIM ALANLARI:
 *   c=1: Lojistik merkezleri, L1-norm minimizasyon
 *   c=2: Regresyon, MSE minimizasyon, ML merkez hesaplama
 */
