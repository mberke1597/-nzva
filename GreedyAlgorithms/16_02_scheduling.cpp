// ============================================================
//  16.2 — Etkinlik Zamanlama (Activity Selection / Scheduling)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 6
//
//  ✅ struct/class YOK → global paralel diziler kullanılır
//  ✅ Etkinlikleri bitiş zamanına göre sıralamak = optimal strateji
//  ✅ Neden diğer stratejiler yanlış — karşıt örneklerle gösterilir
//
//  NEDEN BU ALGORİTMA?
//  "Birbiriyle çakışmayan maksimum etkinlik sayısını bul."
//  Toplantı odası planlaması, CPU görev atama, spor programları.
//
//  GREEDY STRATEJİSİ: En erken biten etkinliği seç.
//  Kanıt: En erken biten seçmek, sonraki etkinlikler için
//         mümkün olan en geniş zaman aralığını bırakır.
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

int  startTime[MAXN];    // Her etkinliğin başlangıç zamanı
int  endTime[MAXN];      // Her etkinliğin bitiş zamanı
int  idx[MAXN];          // Sıralama için indeks dizisi (sıralama sırasını tutmak için)
bool selected[MAXN];     // Bu etkinlik seçildi mi?

int N; // Toplam etkinlik sayısı

// ─────────────────────────────────────────────
//  ETKINLIK ZAMANLAMA — GREEDY
//
//  Adımlar:
//  1. Etkinlikleri bitiş zamanına göre artan sıraya diz
//  2. İlk etkinliği daima seç
//  3. Sonraki her etkinlik için: başlangıç zamanı ≥ son seçilen bitiş zamanı mı?
//     Evet → seç, Hayır → atla
//
//  Karmaşıklık: O(n log n)
// ─────────────────────────────────────────────
int greedySchedule() {
    // idx[] dizisini 0'dan N-1'e başlat
    iota(idx, idx + N, 0);

    // Bitiş zamanına göre sırala (bitiş eşitse başlangıca bak)
    sort(idx, idx + N, [](int a, int b) {
        if (endTime[a] != endTime[b]) return endTime[a] < endTime[b];
        return startTime[a] < startTime[b];
    });

    fill(selected, selected + N, false);

    int count       = 0;
    int lastEndTime = -1; // Başlangıçta seçili etkinlik yok

    for (int i = 0; i < N; i++) {
        int e = idx[i]; // Sıralanmış i. etkinlik

        // Bu etkinlik son seçilenle çakışıyor mu?
        if (startTime[e] >= lastEndTime) {
            selected[e] = true;
            lastEndTime = endTime[e];
            count++;
        }
    }

    return count; // Seçilen etkinlik sayısı
}

// ─────────────────────────────────────────────
//  SONUCU YAZDIR
// ─────────────────────────────────────────────
void printResult() {
    cout << "  Etkinlikler: " << endl;
    for (int i = 0; i < N; i++) {
        cout << "    [" << startTime[i] << ", " << endTime[i] << "]";
        if (selected[i]) cout << " ← SEÇİLDİ";
        cout << endl;
    }
}

// ─────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Test 1: Kitap örneği ──
    // Etkinlikler: A[1,3], B[2,5], C[3,9], D[6,8]
    // Beklenen: A ve D seçilir (veya A ve B değil, A ve D = 2)
    // Bitiş sırasına göre: A(3), B(5), D(8), C(9)
    // A seç (lastEnd=3), B başlangıcı=2 < 3 → atla, D başlangıcı=6 ≥ 3 → seç, C başlangıcı=3 < 8 → atla
    cout << "=== Test 1: Kitap Örneği ===" << endl;
    N = 4;
    int s1[] = {1, 2, 3, 6};
    int e1[] = {3, 5, 9, 8};
    copy(s1, s1 + N, startTime);
    copy(e1, e1 + N, endTime);

    int count1 = greedySchedule();
    cout << "  Maksimum etkinlik sayısı: " << count1 << endl;
    printResult();

    // ── Test 2: Neden "en kısa etkinlik önce" YANLIŞ? ──
    // [1,5], [4,7], [6,10]
    // Kısa olan [4,7] seçilirse sadece 1 etkinlik olabilir
    // Greedy (bitiş sırası): [1,5] → [6,10] = 2 etkinlik (DOĞRU)
    cout << "\n=== Test 2: Kısa Etkinlik Önce YANLIŞ ===\n"
         << "  [4,7] kısa ama [1,5] ve [6,10]'u bloklar!" << endl;
    N = 3;
    int s2[] = {1, 4, 6};
    int e2[] = {5, 7, 10};
    copy(s2, s2 + N, startTime);
    copy(e2, e2 + N, endTime);

    int count2 = greedySchedule();
    cout << "  Greedy (bitiş sırası) bulduğu: " << count2 << " etkinlik" << endl;
    printResult();

    // ── Test 3: Neden "en erken başlayan önce" YANLIŞ? ──
    // [1,100], [2,3], [4,5]
    // En erken başlayan [1,100]'ü seçersek sadece 1 etkinlik
    // Greedy (bitiş sırası): [2,3] ve [4,5] = 2 etkinlik (DOĞRU)
    cout << "\n=== Test 3: Erken Başlayan Önce YANLIŞ ===\n"
         << "  [1,100] erken başlar ama tüm günü kapar!" << endl;
    N = 3;
    int s3[] = {1, 2, 4};
    int e3[] = {100, 3, 5};
    copy(s3, s3 + N, startTime);
    copy(e3, e3 + N, endTime);

    int count3 = greedySchedule();
    cout << "  Greedy (bitiş sırası) bulduğu: " << count3 << " etkinlik" << endl;
    printResult();

    // ── Test 4: Büyük test ──
    cout << "\n=== Test 4: 6 Etkinlik ===" << endl;
    N = 6;
    int s4[] = {1, 3, 0, 5, 8, 5};
    int e4[] = {2, 4, 6, 7, 9, 9};
    copy(s4, s4 + N, startTime);
    copy(e4, e4 + N, endTime);

    int count4 = greedySchedule();
    cout << "  Maksimum etkinlik sayısı: " << count4 << endl;
    printResult();

    return 0;
}

/*
 * KARMAŞIKLIK:
 *   Zaman: O(n log n) sıralama + O(n) tarama
 *   Uzay:  O(n) giriş dizileri
 *
 * DOĞRULUK KANITI (Exchange Argument):
 *   Diyelim optimal çözüm bitiş zamanı e_opt olan bir etkinlik içeriyor,
 *   ama greedy bitiş zamanı e_g ≤ e_opt olan farklı bir etkinlik seçti.
 *   Greedy'nin seçtiğini optimal'inkiyle değiştirsek:
 *   - e_g ≤ e_opt olduğundan sonraki etkinlikler için daha fazla alan açılır
 *   - Yani optimal çözüm "kötüleşmez" → Greedy en az o kadar iyidir.
 *
 * KULLANIM ALANLARI:
 *   Toplantı odası rezervasyon sistemi
 *   İşlemci görev çizelgeleme (CPU scheduling)
 *   TV yayın programı planlaması
 */
