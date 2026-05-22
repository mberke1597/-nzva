// ============================================================
//  10.6 — Bitmask DP: Optimal Seçim (Ürün/Gün)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 10
//
//  ✅ struct/class YOK → global 2D diziler kullanılır
//  ✅ Bitmask ile ürün kümesi temsili
//  ✅ Geri izleme: hangi günde hangi ürün alındı?
//
//  PROBLEM:
//  k ürün var, n gün var. price[x][d] = x ürününün d. gündeki fiyatı.
//  Her ürün tam olarak 1 kez alınacak, günde en fazla 1 ürün alınabilir.
//  Minimum toplam fiyatı bul.
//
//  NEDEN BITMASK DP?
//  "Hangi ürünler alındı?" durumu önemli → 2^k subset ile tut.
//  Karmaşıklık: O(n × 2^k × k)  —  n! yerine!
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXK = 20;   // Max ürün sayısı (2^20 = 1M)
const int MAXN = 105;  // Max gün sayısı
const int INF  = 1e9;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
// ─────────────────────────────────────────────
int price[MAXK][MAXN];  // price[ürün][gün]
int dp[1 << MAXK][MAXN];// dp[mask][gün] = min toplam fiyat
int choice[1 << MAXK][MAXN]; // Geri izleme: o gün hangi ürün alındı (-1=hiç)

int K, N; // Ürün ve gün sayısı

// ─────────────────────────────────────────────
//  BITMASK DP: OPTİMAL SEÇIM
//
//  dp[mask][d] = mask kümesindeki ürünler alınmış, d. güne kadar min maliyet
//
//  Geçiş:
//  dp[mask][d] = min(
//    dp[mask][d-1],                          // Bugün hiçbir şey alma
//    dp[mask ^ (1<<x)][d-1] + price[x][d]   // Bugün x'i al (x ∈ mask)
//  )
//
//  Karmaşıklık: O(n × 2^k × k)
// ─────────────────────────────────────────────
int optimalSelection() {
    int states = 1 << K;

    // Başlangıç: her şey INF
    for (int s = 0; s < states; s++)
        for (int d = 0; d < N; d++) {
            dp[s][d] = INF;
            choice[s][d] = -1;
        }

    // Taban: boş küme, her gün için maliyet = 0
    for (int d = 0; d < N; d++) dp[0][d] = 0;

    // 0. gün: tek ürün seçilebilir
    for (int x = 0; x < K; x++) {
        dp[1 << x][0] = price[x][0];
        choice[1 << x][0] = x;
    }

    // d. günde mask kümesini tamamlamanın min maliyeti
    for (int d = 1; d < N; d++) {
        for (int s = 0; s < states; s++) {
            // Seçenek 1: Bugün hiçbir şey alma
            if (dp[s][d-1] < dp[s][d]) {
                dp[s][d]      = dp[s][d-1];
                choice[s][d]  = -1; // Önceki günden devral
            }
            // Seçenek 2: Bugün s kümesindeki bir ürünü al
            for (int x = 0; x < K; x++) {
                if (!(s & (1 << x))) continue; // x, s içinde değil
                int prevMask = s ^ (1 << x);   // x olmadan önceki küme
                if (dp[prevMask][d-1] == INF) continue;
                int newCost = dp[prevMask][d-1] + price[x][d];
                if (newCost < dp[s][d]) {
                    dp[s][d]     = newCost;
                    choice[s][d] = x;
                }
            }
        }
    }

    int fullMask = states - 1;
    // En iyi gün: tüm ürünlerin alındığı son gün
    int bestDay = 0;
    for (int d = 1; d < N; d++)
        if (dp[fullMask][d] < dp[fullMask][bestDay])
            bestDay = d;

    return dp[fullMask][bestDay];
}

// Geri izleme: hangi günde hangi ürün alındı
void traceBack() {
    int fullMask = (1 << K) - 1;
    int bestDay = 0;
    for (int d = 1; d < N; d++)
        if (dp[fullMask][d] < dp[fullMask][bestDay]) bestDay = d;

    cout << "  Satın alma planı:" << endl;
    int mask = fullMask;
    int d    = bestDay;
    vector<pair<int,int>> plan; // {gün, ürün}

    while (mask > 0 && d >= 0) {
        int x = choice[mask][d];
        if (x == -1) {
            d--; // Bu gün alım yok, bir önceki güne git
        } else {
            plan.push_back({d, x});
            mask ^= (1 << x);
            d--;
        }
    }

    for (auto [day, prod] : plan)
        cout << "    Gün " << day << ": Ürün " << prod
             << " → " << price[prod][day] << " TL" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Kitap örneği ──
    // k=3 ürün, n=8 gün
    // Minimum toplam = 5 (ürün0:gün6=1, ürün1:gün1=2, ürün2:gün4=3 → 1+2+3=6? Hayır)
    // Kitaptaki cevap: 5 = ürün2(gün1=3) + ürün1(gün3=2) + ürün0(gün6=1) → 6? 
    // Kitap tablosu: min=5 ama farklı kombinasyon var
    cout << "=== Kitap Örneği: k=3 ürün, n=8 gün ===" << endl;
    K = 3; N = 8;

    // price[ürün][gün]
    int p[3][8] = {
        {6, 9, 5, 2, 8, 9, 1, 6},  // Ürün 0
        {8, 2, 6, 2, 7, 5, 7, 2},  // Ürün 1
        {5, 3, 9, 7, 3, 5, 1, 4}   // Ürün 2
    };
    for (int x = 0; x < K; x++)
        for (int d = 0; d < N; d++) price[x][d] = p[x][d];

    cout << "  Fiyat tablosu:" << endl;
    cout << "       Gün: ";
    for (int d = 0; d < N; d++) cout << d << "  ";
    cout << endl;
    for (int x = 0; x < K; x++) {
        cout << "  Ürün " << x << ":    ";
        for (int d = 0; d < N; d++) cout << price[x][d] << "  ";
        cout << endl;
    }

    int result = optimalSelection();
    cout << "\n  Minimum toplam fiyat: " << result << endl;
    traceBack();

    // ── Küçük test ──
    cout << "\n=== Küçük Test: k=2 ürün, n=3 gün ===" << endl;
    K = 2; N = 3;
    int p2[2][3] = {{5,3,8},{2,7,1}};
    for (int x = 0; x < K; x++)
        for (int d = 0; d < N; d++) price[x][d] = p2[x][d];
    // Optimum: Ürün0=gün1(3), Ürün1=gün0(2) → 5
    // veya: Ürün0=gün1(3), Ürün1=gün2(1) → 4 ✓
    cout << "  Minimum toplam: " << optimalSelection() << "  (beklenen: 4)" << endl;
    traceBack();

    return 0;
}

/*
 * KARMAŞIKLIK:
 *   Zaman: O(n × 2^k × k)
 *   Uzay:  O(n × 2^k)
 *
 *   k=20, n=100 → 100 × 1M × 20 = 2 milyar → yavaş
 *   k=15, n=100 → 100 × 32K × 15 = 48M → hızlı
 *   Pratikte k ≤ 20 için uygulanabilir
 */
