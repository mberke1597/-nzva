// ============================================================
//  16.1 — Coin Change: Greedy Yaklaşım
//  Kaynak: Competitive Programmer's Handbook, Bölüm 6
//
//  ✅ struct/class YOK → global diziler kullanılır
//  ✅ Ne zaman çalışır, ne zaman çalışmaz — her ikisi de gösterilir
//  ✅ Euro seti (doğru) vs genel set (yanlış) karşılaştırması
//
//  NEDEN GREEDY?
//  Her adımda kalan toplamı aşmayan en büyük parayı seç.
//  Çok hızlı: O(k log k) sıralama + O(n/cmax) seçim.
//
//  UYARI: Yalnızca "canonical" para setlerinde (Euro gibi) optimal.
//  Genel durumda → 17_01_coin_change_dp.cpp kullan!
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
// ─────────────────────────────────────────────
// Para değerleri ve sayım — global diziler
int  coinValues[20];    // Kullanılabilecek para değerleri
int  coinCount;         // Para çeşit sayısı
int  selectedCoins[10000]; // Seçilen paraların listesi
int  selectedCount;     // Kaç para seçildi

// ─────────────────────────────────────────────
//  GREEDY COIN CHANGE
//
//  Algoritma: Paraları büyükten küçüğe sırala.
//  Her adımda: kalan_miktar / para_degeri kadar o paradan al.
//
//  Geri döndürülen değer: kullanılan para sayısı (-1 = imkansız)
// ─────────────────────────────────────────────
int greedyCoinChange(int n, int coins[], int k) {
    // Büyükten küçüğe sırala
    // (coins dizisi zaten sorted varsayılabilir ama sıralayalım)
    vector<int> sortedCoins(coins, coins + k);
    sort(sortedCoins.rbegin(), sortedCoins.rend());

    selectedCount = 0;
    int remaining = n;

    for (int coin : sortedCoins) {
        // Bu paradan kaç tane alabiliriz?
        while (remaining >= coin) {
            selectedCoins[selectedCount++] = coin;
            remaining -= coin;
        }
    }

    if (remaining > 0) return -1; // Oluşturulamadı
    return selectedCount;
}

// ─────────────────────────────────────────────
//  SONUCU YAZDIR
// ─────────────────────────────────────────────
void printResult(int n, int coins[], int k, const string& label) {
    cout << "\n=== " << label << " (hedef=" << n << ") ===" << endl;
    cout << "  Para seti: { ";
    for (int i = 0; i < k; i++) cout << coins[i] << " ";
    cout << "}" << endl;

    int count = greedyCoinChange(n, coins, k);

    if (count == -1) {
        cout << "  SONUÇ: Oluşturulamaz!" << endl;
    } else {
        cout << "  Seçilen paralar: ";
        for (int i = 0; i < count; i++) cout << selectedCoins[i] << " ";
        cout << endl;
        cout << "  Toplam para sayısı: " << count << endl;
    }
}

// ─────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Test 1: Euro Seti — Greedy DOĞRU ──
    // Kitaptan: n=520, beklenen çözüm: 200+200+100+20 = 4 para
    {
        int euroCoins[] = {1, 2, 5, 10, 20, 50, 100, 200};
        int k = 8;
        printResult(520, euroCoins, k, "Euro Seti — Greedy DOGRU");
    }

    // ── Test 2: Genel Set — Greedy YANLIŞ ──
    // Kitaptan: coins={1,3,4}, n=6
    // Greedy: 4+1+1=3 para  ← YANLIŞ
    // Optimal: 3+3=2 para   ← DOGRU (DP ile bulunur)
    {
        int generalCoins[] = {1, 3, 4};
        int k = 3;
        printResult(6, generalCoins, k, "Genel Set {1,3,4} — Greedy YANLIS (optimal=2)");
    }

    // ── Test 3: Başka genel set karşıt örneği ──
    // coins={1,6,9}, n=11
    // Greedy: 9+1+1=3 para  ← YANLIŞ
    // Optimal: 6+5... hayır 5 yok. 6+1+1+1+1+1=6? 
    // Aslında 9+1+1=3 ile karşılaştır: 6+6=-1? 6+5 yok. Doğru: 9+1+1=3 hm
    // Daha net: coins={1,5,6,9}, n=11
    // Greedy: 9+1+1=3  Optimal: 6+5=2
    {
        int trapCoins[] = {1, 5, 6, 9};
        int k = 4;
        printResult(11, trapCoins, k, "Tuzak Set {1,5,6,9} — Greedy 9+1+1=3, Optimal=6+5=2");
    }

    // ── Test 4: 1 kuruş yoksa ──
    {
        int noOneCoins[] = {3, 5, 7};
        int k = 3;
        printResult(11, noOneCoins, k, "Para seti {3,5,7} n=11");
        printResult(4, noOneCoins, k, "Para seti {3,5,7} n=4 — Olusturulamaz!");
    }

    cout << "\n=== Özet ===" << endl;
    cout << "  ✅ Greedy: Sadece canonical setlerde (Euro, cent gibi) kullan" << endl;
    cout << "  ❌ Greedy: Genel set → yanlış sonuç verebilir" << endl;
    cout << "  → Genel durum için: 17_01_coin_change_dp.cpp kullan" << endl;

    return 0;
}

/*
 * KARMAŞIKLIK:
 *   Zaman: O(k log k) sıralama + O(n / c_max) seçim
 *   Uzay:  O(k) coins + O(n) selected (en kötü)
 *
 * NEDEN EURO SETİ ÇALIŞIR?
 *   Her para x için: x'ten küçük paralarla oluşturulabilecek
 *   maksimum optimal toplam x-1'dir.
 *   (ör: 100 cent'ten küçük paralarla max toplam = 50+20+20+5+2+2 = 99)
 *   Dolayısıyla her zaman en büyük parayı seçmek garanti optimal.
 */
