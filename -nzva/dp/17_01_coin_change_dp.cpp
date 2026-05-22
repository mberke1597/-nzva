// ============================================================
//  17.1 — Coin Change: Dynamic Programming (Genel Durum)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 7
//
//  ✅ struct/class YOK → global diziler kullanılır
//  ✅ Bottom-up tabulation (tercih edilen yöntem)
//  ✅ Yol geri izleme: hangi paralar seçildi?
//  ✅ Greedy'nin başaramadığı durumda DP nasıl çalışır?
//
//  NEDEN DP?
//  Greedy genel para setleri için yanlış sonuç verebilir.
//  DP, overlapping subproblems'ı memoize ederek her durumu garanti çözer.
//  dp[x] = x toplamını minimum kaç parayla yapabiliriz?
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
// ─────────────────────────────────────────────
const int MAXN  = 1000005;
const int INF   = 1e9;
const int MAXK  = 25;

int  coins[MAXK];  // Para değerleri
int  K;            // Para çeşit sayısı

int  dp[MAXN];     // dp[x] = x toplamı için minimum para sayısı
int  first[MAXN];  // first[x] = x toplamındaki optimal son para (geri izleme için)

// ─────────────────────────────────────────────
//  COIN CHANGE DP — MİNİMUM PARA SAYISI
//
//  Formül: dp[x] = min(dp[x - c] + 1)  for c in coins, c ≤ x
//  Taban:  dp[0] = 0
//
//  Karmaşıklık: O(n * k) zaman, O(n) uzay
// ─────────────────────────────────────────────
int coinChangeDP(int n) {
    fill(dp + 0, dp + n + 1, INF);
    fill(first + 0, first + n + 1, 0);
    dp[0] = 0;

    for (int x = 1; x <= n; x++) {
        for (int i = 0; i < K; i++) {
            int c = coins[i];
            if (c <= x && dp[x - c] != INF && dp[x - c] + 1 < dp[x]) {
                dp[x]    = dp[x - c] + 1;
                first[x] = c; // Bu geçişte kullanılan para
            }
        }
    }

    return dp[n]; // INF ise imkansız
}

// ─────────────────────────────────────────────
//  SONUCU YAZDIR + GERİ İZLEME
// ─────────────────────────────────────────────
void printResult(int n, const string& label) {
    cout << "\n=== " << label << " (hedef=" << n << ") ===" << endl;
    cout << "  Para seti: { ";
    for (int i = 0; i < K; i++) cout << coins[i] << " ";
    cout << "}" << endl;

    int result = coinChangeDP(n);

    if (result == INF) {
        cout << "  SONUÇ: Oluşturulamaz!" << endl;
        return;
    }

    cout << "  Minimum para sayısı: " << result << endl;
    cout << "  Seçilen paralar: ";
    int temp = n;
    while (temp > 0) {
        cout << first[temp] << " ";
        temp -= first[temp];
    }
    cout << endl;

    // Tüm dp değerlerini göster (küçük n için)
    if (n <= 20) {
        cout << "  DP tablosu: ";
        for (int x = 0; x <= n; x++) {
            if (dp[x] == INF) cout << "∞ ";
            else cout << dp[x] << " ";
        }
        cout << endl;
    }
}

// ─────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Test 1: Greedy'nin başarısız olduğu durum ──
    // coins={1,3,4}, n=6
    // Greedy: 4+1+1 = 3 para ❌
    // DP:     3+3   = 2 para ✅
    K = 3;
    coins[0]=1; coins[1]=3; coins[2]=4;
    printResult(6, "DP vs Greedy — {1,3,4}, n=6 (Greedy yanlis=3, DP dogru=2)");

    // ── Test 2: Euro seti (Greedy ile aynı sonuç) ──
    K = 8;
    int euroArr[] = {1,2,5,10,20,50,100,200};
    copy(euroArr, euroArr+8, coins);
    printResult(520, "Euro Seti — n=520 (Greedy ile ayni: 4 para)");

    // ── Test 3: Oluşturulamayan durum ──
    K = 2;
    coins[0]=3; coins[1]=5;
    printResult(7, "coins={3,5}, n=7 — OLUSTURULAMAZ");

    // ── Test 4: Büyük n ──
    K = 3;
    coins[0]=1; coins[1]=5; coins[2]=10;
    printResult(27, "coins={1,5,10}, n=27");

    // ── Test 5: DP tablosunu göster ──
    cout << "\n=== DP Tablosu Görselleştirme (coins={1,3,4}, n=10) ===" << endl;
    K = 3;
    coins[0]=1; coins[1]=3; coins[2]=4;
    coinChangeDP(10);
    cout << "  x   : ";
    for (int x = 0; x <= 10; x++) cout << x << "  ";
    cout << endl;
    cout << "  dp[x]: ";
    for (int x = 0; x <= 10; x++) {
        if (dp[x] == INF) cout << "∞  ";
        else cout << dp[x] << "  ";
    }
    cout << endl;

    return 0;
}

/*
 * KARMAŞIKLIK:
 *   Zaman: O(n * k)  — n hedef, k para çeşidi
 *   Uzay:  O(n)      — dp ve first dizileri
 *
 * OVERLAPPING SUBPROBLEMS:
 *   dp[6] hesaplarken dp[5], dp[3], dp[2] lazım.
 *   dp[5] hesaplarken dp[4], dp[2], dp[1] lazım.
 *   dp[3] hesaplarken dp[2] lazım.
 *   → dp[2] birden fazla kez lazım = overlapping!
 *   DP bunu tek kez hesaplayıp kaydeder.
 *
 * GERİ İZLEME (first[] dizisi):
 *   first[x] = x toplamını veren optimal son para.
 *   x → x - first[x] → ... → 0 şeklinde geriye git.
 */
