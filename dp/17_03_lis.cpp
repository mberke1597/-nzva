// ============================================================
//  17.3 — LIS: En Uzun Artan Altdizi
//  Kaynak: Competitive Programmer's Handbook, Bölüm 7
//
//  ✅ struct/class YOK → global diziler kullanılır
//  ✅ Yaklaşım 1: O(n²) DP
//  ✅ Yaklaşım 2: O(n log n) Binary Search (tails dizisi)
//  ✅ Geri izleme: hangi elemanlar LIS'i oluşturuyor?
//
//  NEDEN KULLANILIR?
//  Stok fiyatlarında en uzun artış serisi, DNA dizilimi,
//  en uzun uyumlu aktivite sırası gibi problemlerde.
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 100005;

int  arr[MAXN];    // Giriş dizisi
int  lenDP[MAXN];  // lenDP[k] = k'da biten LIS uzunluğu (O(n²) için)
int  prevIdx[MAXN];// Geri izleme: önceki elemanın indeksi
int  N;

// ─── Yaklaşım 1: O(n²) DP ───────────────────
int lisN2() {
    fill(lenDP, lenDP + N, 1);
    fill(prevIdx, prevIdx + N, -1);
    int best = 1, bestEnd = 0;

    for (int k = 1; k < N; k++) {
        for (int i = 0; i < k; i++) {
            if (arr[i] < arr[k] && lenDP[i] + 1 > lenDP[k]) {
                lenDP[k]  = lenDP[i] + 1;
                prevIdx[k] = i;
            }
        }
        if (lenDP[k] > best) { best = lenDP[k]; bestEnd = k; }
    }

    // Geri izleme
    cout << "  LIS uzunluğu (O(n²)): " << best << endl;
    cout << "  LIS elemanları: ";
    vector<int> path;
    for (int idx = bestEnd; idx != -1; idx = prevIdx[idx])
        path.push_back(arr[idx]);
    reverse(path.begin(), path.end());
    for (int x : path) cout << x << " ";
    cout << endl;

    return best;
}

// ─── Yaklaşım 2: O(n log n) Binary Search ───
int lisNLogN() {
    vector<int> tails; // tails[i] = uzunluk i+1'li LIS'in en küçük son elemanı

    for (int i = 0; i < N; i++) {
        // arr[i] >= olan ilk elemanı bul
        auto it = lower_bound(tails.begin(), tails.end(), arr[i]);
        if (it == tails.end()) tails.push_back(arr[i]); // Yeni uzunluk
        else *it = arr[i]; // Mevcut pozisyonu güncelle (daha küçük son eleman)
    }

    cout << "  LIS uzunluğu (O(n log n)): " << tails.size() << endl;
    // NOT: tails dizisi LIS'in kendisi DEĞİL, sadece uzunluğu verir.
    // Gerçek elemanları bulmak için ayrı geri izleme gerekir.

    return tails.size();
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Kitap örneği: [6,2,5,1,7,4,8,3] → LIS = [2,5,7,8], uzunluk=4
    N = 8;
    int a[] = {6,2,5,1,7,4,8,3};
    copy(a, a+N, arr);

    cout << "=== LIS: [6,2,5,1,7,4,8,3] ===" << endl;
    lisN2();
    lisNLogN();

    // Test 2
    N = 6;
    int b[] = {3,1,4,1,5,9};
    copy(b, b+N, arr);
    cout << "\n=== LIS: [3,1,4,1,5,9] ===" << endl;
    lisN2();
    lisNLogN();

    // Test 3: Tüm azalan → LIS=1
    N = 5;
    int c[] = {5,4,3,2,1};
    copy(c, c+N, arr);
    cout << "\n=== LIS: [5,4,3,2,1] (azalan) ===" << endl;
    lisN2();

    return 0;
}

/*
 * KARMAŞIKLIK:
 *   O(n²) DP:            Zaman O(n²),  Uzay O(n)
 *   O(n log n) BinSearch: Zaman O(n log n), Uzay O(n)
 *
 * tails dizisi ne anlama gelir?
 *   tails[i] = uzunluğu i+1 olan tüm increasing subsequence'lerin
 *              bittiği en küçük değer.
 *   Her yeni eleman için lower_bound → O(log n) güncelleme.
 */
