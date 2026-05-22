// ============================================================
//  17.5 — Knapsack: Sırt Çantası Problemleri
//  Kaynak: Competitive Programmer's Handbook, Bölüm 7
//
//  ✅ struct/class YOK → global diziler kullanılır
//  ✅ Subset Sum: hangi toplamlar elde edilebilir?
//  ✅ 0/1 Knapsack: maksimum değer
//  ✅ Neden tersten güncelleme? Açıklama var.
//
//  NEDEN KULLANILIR?
//  Kaynak tahsisi, bütçe planlaması, yük optimizasyonu,
//  proje seçimi (hangi projeleri yap?).
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 1005; // Max eleman sayısı
const int MAXW = 100005; // Max kapasite

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
// ─────────────────────────────────────────────
int  weights[MAXN];   // Eşya ağırlıkları
int  values[MAXN];    // Eşya değerleri
int  N;               // Eşya sayısı
int  W;               // Çanta kapasitesi

bool possible[MAXW];  // Subset Sum: bu ağırlık elde edilebilir mi?
int  dpKnap[MAXW];    // 0/1 Knapsack: dp[x] = x kapasiteyle max değer

// ─────────────────────────────────────────────
//  1) SUBSET SUM
//
//  "Bu ağırlıklardan hangilerini toplayarak W elde edebiliriz?"
//
//  NEDEN TERSTEN GÜNCELLEME?
//  Soldan sağa gidersek aynı eleman birden fazla kez kullanılabilir.
//  Sağdan sola gidersek her eleman en fazla 1 kez kullanılır.
//
//  Karmaşıklık: O(n * W)
// ─────────────────────────────────────────────
void subsetSum() {
    fill(possible, possible + W + 1, false);
    possible[0] = true; // Boş seçim

    for (int i = 0; i < N; i++) {
        int w = weights[i];
        // TERSTEN güncelle: her eleman 1 kez (0/1)
        for (int x = W; x >= w; x--) {
            if (possible[x - w]) possible[x] = true;
        }
    }
}

// ─────────────────────────────────────────────
//  2) 0/1 KNAPSACK
//
//  "Kapasitesi W olan çantaya maksimum değeri dolduracak eşyaları seç."
//
//  dp[x] = x kapasiteyle elde edilebilecek maksimum değer
//  Geçiş: dp[x] = max(dp[x], dp[x - w] + v)
//
//  Karmaşıklık: O(n * W)
// ─────────────────────────────────────────────
int knapsack01() {
    fill(dpKnap, dpKnap + W + 1, 0);

    for (int i = 0; i < N; i++) {
        int w = weights[i];
        int v = values[i];
        // TERSTEN güncelle: her eleman 1 kez (0/1)
        for (int x = W; x >= w; x--) {
            dpKnap[x] = max(dpKnap[x], dpKnap[x - w] + v);
        }
    }

    return dpKnap[W];
}

// Knapsack'te hangi eşyalar seçildi (geri izleme)
void printSelectedItems() {
    // Geri izleme için 2D DP tablosu gerekir (basit versiyon)
    // Burada hangi kapasite değerlerinin güncellediğini takip ederiz
    vector<vector<int>> dp2d(N + 1, vector<int>(W + 1, 0));
    for (int i = 1; i <= N; i++) {
        int w = weights[i-1];
        int v = values[i-1];
        for (int x = 0; x <= W; x++) {
            dp2d[i][x] = dp2d[i-1][x]; // Kullanma
            if (x >= w) dp2d[i][x] = max(dp2d[i][x], dp2d[i-1][x-w] + v);
        }
    }

    cout << "  Seçilen eşyalar (geri izleme): ";
    int cap = W;
    for (int i = N; i >= 1; i--) {
        if (dp2d[i][cap] != dp2d[i-1][cap]) {
            cout << "Eşya" << i << "(w=" << weights[i-1] << ",v=" << values[i-1] << ") ";
            cap -= weights[i-1];
        }
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Test 1: Subset Sum ──
    cout << "=== Subset Sum ===" << endl;
    N = 5; W = 10;
    int sw[] = {2, 3, 5, 7, 1};
    copy(sw, sw+N, weights);
    subsetSum();
    cout << "  Ağırlıklar: { ";
    for (int i = 0; i < N; i++) cout << weights[i] << " ";
    cout << "}, Kapasite W=" << W << endl;
    cout << "  Elde edilebilen toplamlar: ";
    for (int x = 0; x <= W; x++) if (possible[x]) cout << x << " ";
    cout << endl;

    // ── Test 2: 0/1 Knapsack ──
    cout << "\n=== 0/1 Knapsack ===" << endl;
    N = 4; W = 7;
    int kw[] = {1, 3, 4, 5};
    int kv[] = {1, 4, 5, 7};
    copy(kw, kw+N, weights);
    copy(kv, kv+N, values);

    cout << "  Eşyalar (ağırlık, değer): ";
    for (int i = 0; i < N; i++) cout << "(" << weights[i] << "," << values[i] << ") ";
    cout << endl;
    cout << "  Kapasite W=" << W << endl;

    int maxVal = knapsack01();
    cout << "  Maksimum değer: " << maxVal << endl;
    printSelectedItems();

    // DP tablosu
    cout << "  dp tablosu: ";
    for (int x = 0; x <= W; x++) cout << dpKnap[x] << " ";
    cout << endl;

    // ── Test 3: Neden tersten? Demonstrasyon ──
    cout << "\n=== Neden Tersten Güncelleme? ===" << endl;
    cout << "  Soldan sağa (YANLIŞ - sınırsız kullanım):" << endl;
    N = 1; W = 6;
    weights[0] = 2; values[0] = 3;
    fill(dpKnap, dpKnap+W+1, 0);
    // Soldan sağa (yanlış - unbounded knapsack olur)
    for (int x = weights[0]; x <= W; x++) {
        dpKnap[x] = max(dpKnap[x], dpKnap[x - weights[0]] + values[0]);
    }
    cout << "    dp: "; for (int x=0;x<=W;x++) cout<<dpKnap[x]<<" "; cout<<endl;
    cout << "    (w=2, v=3, kapasite=6 → sınırsız kullanımda 3 eşya = 9)" << endl;

    cout << "  Sağdan sola (DOĞRU - 0/1 knapsack):" << endl;
    fill(dpKnap, dpKnap+W+1, 0);
    // Sağdan sola (doğru - her eleman 1 kez)
    for (int x = W; x >= weights[0]; x--) {
        dpKnap[x] = max(dpKnap[x], dpKnap[x - weights[0]] + values[0]);
    }
    cout << "    dp: "; for (int x=0;x<=W;x++) cout<<dpKnap[x]<<" "; cout<<endl;
    cout << "    (w=2, v=3, kapasite=6 → 0/1: 1 eşya max = 3)" << endl;

    return 0;
}
