// ============================================================
//  17.2 — Çözüm Sayısı Sayma (Counting Solutions)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 7
//
//  ✅ struct/class YOK → global diziler kullanılır
//  ✅ Sıra önemli (permütasyon): İç döngü = coins
//  ✅ Sıra önemsiz (kombinasyon): İç döngü = target, dış = coins
//
//  NEDEN KULLANILIR?
//  "Hedef n toplamını kaç FARKLI şekilde oluşturabiliriz?"
//  Optimizasyon değil SAYMA problemi.
//  count[x] = Σ count[x - c] for all c in coins
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

const int MAXN = 100005;
const int MOD  = 1e9 + 7;
const int MAXK = 25;

int  coins[MAXK];
int  K;
long long countArr[MAXN]; // countArr[x] = x toplamına ulaşmanın yol sayısı

// Sıra önemli (1+3 ve 3+1 farklı sayılır)
long long countWaysOrdered(int n) {
    fill(countArr, countArr + n + 1, 0);
    countArr[0] = 1; // Hiçbir para = 1 yol

    for (int x = 1; x <= n; x++) {
        for (int i = 0; i < K; i++) {
            int c = coins[i];
            if (c <= x) {
                countArr[x] = (countArr[x] + countArr[x - c]) % MOD;
            }
        }
    }
    return countArr[n];
}

// Sıra önemsiz (1+3 ve 3+1 aynı sayılır — kombinasyon)
long long countWaysUnordered(int n) {
    fill(countArr, countArr + n + 1, 0);
    countArr[0] = 1;

    for (int i = 0; i < K; i++) {  // Dış döngü: coins
        int c = coins[i];
        for (int x = c; x <= n; x++) { // İç döngü: target
            countArr[x] = (countArr[x] + countArr[x - c]) % MOD;
        }
    }
    return countArr[n];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // coins={1,3,4}, n=5
    // Sıra önemli: 1+1+3, 1+3+1, 3+1+1, 1+4, 4+1, 1+1+1+1+1 = 6 yol
    K = 3;
    coins[0]=1; coins[1]=3; coins[2]=4;
    int n = 5;

    cout << "=== Çözüm Sayısı Sayma ===" << endl;
    cout << "  coins={1,3,4}, n=" << n << endl;
    cout << "  Sıra önemli  (permütasyon): " << countWaysOrdered(n) << " yol" << endl;
    cout << "  Sıra önemsiz (kombinasyon): " << countWaysUnordered(n) << " yol" << endl;

    // DP tablosu
    cout << "\n  DP tablosu (sıra önemli):" << endl;
    countWaysOrdered(n);
    cout << "  x   : ";
    for (int x = 0; x <= n; x++) cout << x << " ";
    cout << endl;
    cout << "  c[x]: ";
    for (int x = 0; x <= n; x++) cout << countArr[x] << " ";
    cout << endl;

    // Mod örneği
    K = 2;
    coins[0]=1; coins[1]=2;
    cout << "\n  coins={1,2}, n=10, sıra önemli: "
         << countWaysOrdered(10) << " yol" << endl;

    return 0;
}
