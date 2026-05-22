// ============================================================
//  16.3 — Görevler ve Deadline (Tasks and Deadlines)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 6
//
//  ✅ struct/class YOK → global paralel diziler kullanılır
//  ✅ Görevleri süreye göre küçükten büyüğe sırala = optimal
//  ✅ Exchange Argument kanıtı açıklanır
//
//  PROBLEM:
//  n görev var. Her görev i'nin:
//    - duration[i]: tamamlanma süresi
//    - deadline[i]: son teslim tarihi
//  Görev bitiş anı x olursa kazanılan puan = deadline[i] - x
//  Tüm görevleri sıraya koyup maksimum toplam puanı bul.
//
//  GREEDY SONUCU: Deadline'a değil, SÜREYE göre küçükten büyüğe sırala!
//
//  NEDEN DEADLINE'A GÖRE SIRALAMAK YANLIŞ?
//  Deadline sadece toplam skoru etkiler, sıralama kararını değil.
//  Kısa görevleri öne koymak sonraki görevlerin daha erken bitmesini sağlar.
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR (struct yok!)
// ─────────────────────────────────────────────
const int MAXN = 100005;

int  duration[MAXN];   // Her görevin süresi
int  deadline[MAXN];   // Her görevin son teslim tarihi
int  sortIdx[MAXN];    // Sıralama için indeks

int N; // Görev sayısı

// ─────────────────────────────────────────────
//  GREEDY: SÜREYE GÖRE SIRALA
//
//  Adımlar:
//  1. Görevleri süreye göre küçükten büyüğe sırala
//  2. Sırayla yap: her görev bitince bitiş zamanını kaydet
//  3. Toplam puan = Σ (deadline[i] - bitiş_zamanı[i])
//
//  Karmaşıklık: O(n log n)
// ─────────────────────────────────────────────
long long greedyTasks() {
    iota(sortIdx, sortIdx + N, 0);

    // Süreye göre küçükten büyüğe sırala
    sort(sortIdx, sortIdx + N, [](int a, int b) {
        return duration[a] < duration[b];
    });

    long long currentTime = 0;
    long long totalScore  = 0;

    cout << "  Görev sırası (süreye göre): " << endl;
    for (int i = 0; i < N; i++) {
        int t = sortIdx[i];
        currentTime += duration[t];
        long long score = (long long)deadline[t] - currentTime;
        totalScore += score;

        cout << "    Görev " << t + 1
             << " [süre=" << duration[t]
             << ", deadline=" << deadline[t]
             << ", bitiş=" << currentTime
             << ", puan=" << score << "]" << endl;
    }

    return totalScore;
}

// ─────────────────────────────────────────────
//  KARŞILAŞTIRMA: Yanlış sıralama vs doğru sıralama
// ─────────────────────────────────────────────
long long calculateScore(int permIdx[], int n) {
    long long currentTime = 0;
    long long totalScore  = 0;
    for (int i = 0; i < n; i++) {
        int t = permIdx[i];
        currentTime += duration[t];
        totalScore  += (long long)deadline[t] - currentTime;
    }
    return totalScore;
}

// ─────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Test 1: Kitap örneği ──
    // Görevler (süre, deadline): (1,3), (2,1), (3,2)
    // Optimal sıra (süreye göre): Görev1(1) → Görev2(2) → Görev3(3)
    // Bitiş zamanları: 1, 3, 6
    // Puanlar: 3-1=2, 1-3=-2, 2-6=-4 → Toplam: -4
    // Başka sıra: 2,1,3 → bitiş=2,3,6 → puan=-1+0-4=-5 (daha kötü)
    cout << "=== Test 1: 3 Görev ===" << endl;
    N = 3;
    int d1[] = {1, 2, 3};
    int dl1[] = {3, 1, 2};
    copy(d1, d1 + N, duration);
    copy(dl1, dl1 + N, deadline);

    long long score1 = greedyTasks();
    cout << "  Toplam Puan (süreye göre sıralı): " << score1 << endl;

    // Yanlış sıra: deadline'a göre
    iota(sortIdx, sortIdx + N, 0);
    sort(sortIdx, sortIdx + N, [](int a, int b) {
        return deadline[a] < deadline[b];
    });
    long long wrongScore1 = calculateScore(sortIdx, N);
    cout << "  Toplam Puan (deadline'a göre sıralı - YANLIŞ): " << wrongScore1 << endl;

    // ── Test 2: Exchange Argument demonstrasyonu ──
    // 2 görev: X(süre=5, dl=10), Y(süre=2, dl=7)
    // Sıra X→Y: X biter t=5, Y biter t=7 → 10-5 + 7-7 = 5+0 = 5
    // Sıra Y→X: Y biter t=2, X biter t=7 → 7-2 + 10-7 = 5+3 = 8  ← DAHA İYİ
    cout << "\n=== Test 2: Exchange Argument Demonstrasyonu ===" << endl;
    cout << "  X(süre=5, dl=10), Y(süre=2, dl=7)" << endl;

    N = 2;
    duration[0] = 5; deadline[0] = 10; // X
    duration[1] = 2; deadline[1] = 7;  // Y

    // X→Y sırası
    int xyOrder[] = {0, 1};
    long long scoreXY = calculateScore(xyOrder, 2);
    cout << "  X→Y sırası: puan = " << scoreXY << endl;

    // Y→X sırası
    int yxOrder[] = {1, 0};
    long long scoreYX = calculateScore(yxOrder, 2);
    cout << "  Y→X sırası: puan = " << scoreYX << " ← DAHA İYİ (kısa önce)" << endl;

    // Greedy sonucu
    long long greedyResult = greedyTasks();
    cout << "  Greedy bulduğu: " << greedyResult << endl;

    // ── Test 3: Büyük test ──
    cout << "\n=== Test 3: 5 Görev ===" << endl;
    N = 5;
    int d3[] = {3, 1, 2, 4, 5};
    int dl3[] = {9, 3, 5, 10, 12};
    copy(d3, d3 + N, duration);
    copy(dl3, dl3 + N, deadline);

    long long score3 = greedyTasks();
    cout << "  Toplam Puan (greedy): " << score3 << endl;

    return 0;
}

/*
 * KARMAŞIKLIK:
 *   Zaman: O(n log n) sıralama + O(n) hesaplama
 *   Uzay:  O(n)
 *
 * EXCHANGE ARGUMENT KANITININ ÖZETİ:
 *   Ardışık iki görev X(süre=a) ve Y(süre=b) için a > b:
 *   X→Y sırası: toplam puan = ... + (dX - t) + (dY - t - a - b)
 *   Y→X sırası: toplam puan = ... + (dY - t) + (dX - t - a - b)
 *   Fark: Y→X puanı - X→Y puanı = a - b > 0 (çünkü a > b)
 *   → Kısa olanı her zaman öne almak puanı artırır (veya eşit bırakır).
 *
 * ÖNEMLİ NOT:
 *   Deadline değerleri sadece toplam skoru etkiler.
 *   Sıralama kararında deadline kullanmak YANLIŞTIR.
 */
