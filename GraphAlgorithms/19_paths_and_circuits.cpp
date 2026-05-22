// ============================================================
//  19 - Yollar ve Çevrimler (Paths and Circuits)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 19
//
//  ✅ struct/class YOK → global adjList, edgeUsed dizileri
//  ✅ Hierholzer: Euler Circuit / Path (O(V+E))
//  ✅ Hamilton Yolu Sayısı (Bitmask DP, O(N² × 2^N))
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
// ─────────────────────────────────────────────
const int MAXN = 100005;
const int MOD  = 1e9 + 7;

// Euler için: adjList[u] = { {v, edge_id} }
vector<pair<int,int>> adjList[MAXN]; // {komşu, kenar_id}
bool edgeUsed[MAXN * 2];            // Kenar kullanıldı mı?
vector<int> eulerPath;              // Sonuç yolu

int V, totalEdges;

// ─────────────────────────────────────────────
//  YARDIMCI
// ─────────────────────────────────────────────
void addEdgeEuler(int u, int v, int edgeId) {
    adjList[u].push_back({v, edgeId});
    adjList[v].push_back({u, edgeId}); // Yönsüz
}

// ─────────────────────────────────────────────
//  1) HİERHOLZER ALGORİTMASI — Euler Circuit / Path
//
//  NEDEN KULLANILIR?
//  "Bir grafikte her KENAR'ı tam olarak bir kez geçen yol var mı?"
//
//  Gerçek dünya: Posta dağıtım güzergahı (her sokak bir kez),
//  DNA dizi birleştirme (De Bruijn), bulmaca çözme.
//
//  KOŞULLAR (Yönsüz Graf):
//  → Euler Çevrimi: Tüm düğümler ÇIFT dereceliyse
//  → Euler Yolu:    Tam olarak 2 düğüm TEK dereceliyse (başlangıç ve bitiş)
//  → Yukarıdakiler yoksa: Euler yolu/çevrimi yok
//
//  ALGORİTMA (Hierholzer):
//  1. Gidilecek kenar kalmadığında düğümü stack'e at (post-order)
//  2. Stack'i ters çevir → Euler yolu/çevrimi
//
//  Neden basit DFS değil?
//  Basit DFS çıkmaz sokaklara girebilir — Hierholzer geri izleme yapar.
//
//  Karmaşıklık: O(V + E)
// ─────────────────────────────────────────────
void dfsEuler(int u) {
    // Bu düğümden gidilmemiş kenar varsa git
    while (!adjList[u].empty()) {
        auto [v, idx] = adjList[u].back();
        adjList[u].pop_back();

        if (edgeUsed[idx]) continue; // Zaten kullanıldı, atla
        edgeUsed[idx] = true;

        dfsEuler(v);
    }
    eulerPath.push_back(u); // Post-order: geri dönerken ekle
}

// Euler Circuit bul ve yazdır
// Eğer mümkün değilse boş döner
vector<int> findEulerCircuit(int startNode) {
    fill(edgeUsed, edgeUsed + totalEdges, false);
    eulerPath.clear();

    dfsEuler(startNode);
    reverse(eulerPath.begin(), eulerPath.end());

    // Doğrulama: tüm kenarlar kullanıldı mı?
    if ((int)eulerPath.size() != totalEdges + 1) return {};
    return eulerPath;
}

// Koşul kontrolü: Euler circuit veya path mümkün mü?
// degree[u] = u'nun derecesi
int degree[MAXN];

int checkEulerCondition() {
    int oddCount = 0;
    int oddNode  = -1;
    for (int i = 1; i <= V; i++) {
        if (degree[i] % 2 != 0) {
            oddCount++;
            oddNode = i;
        }
    }
    if (oddCount == 0) return 1;  // Euler Circuit mümkün
    if (oddCount == 2) return 2;  // Euler Path mümkün (tek dereceli düğümden başla)
    return 0;                     // Mümkün değil
}

// ─────────────────────────────────────────────
//  2) HAMİLTON YOLU SAYISI — Bitmask DP
//
//  NEDEN KULLANILIR?
//  "Her DÜĞÜM'ü tam olarak bir kez ziyaret eden yol var mı / kaç tane?"
//  TSP (Gezgin Satıcı Problemi) bu temel üzerine kurulur.
//
//  NOT: Hamilton yolu NP-Hard. Küçük N için (N ≤ 20) bitmask DP kullan.
//
//  DP TANIMI:
//  dp[mask][u] = 'mask' kümesindeki düğümler gezilmiş ve son durak u iken
//                geçerli yol sayısı
//
//  GEÇIŞ:
//  dp[mask | (1<<v)][v] += dp[mask][u]    eğer u-v kenarı varsa
//
//  BAŞLANGIÇ:
//  dp[1<<0][0] = 1   (0. düğümden başlıyoruz)
//
//  SONUÇ:
//  dp[(1<<n)-1][n-1] = n-1. düğümde biten tüm Hamilton yolu sayısı
//
//  Karmaşıklık: O(N² × 2^N) → N ≤ 20 için uygulanabilir
// ─────────────────────────────────────────────
int countHamiltonianPaths(int n, const vector<vector<int>>& mat) {
    // dp[mask][u]: son durak u, gezilen kümesi mask
    vector<vector<int>> dp(1 << n, vector<int>(n, 0));
    dp[1][0] = 1; // 0. düğümden başla

    for (int mask = 1; mask < (1 << n); mask++) {
        for (int u = 0; u < n; u++) {
            if (!(mask & (1 << u))) continue; // u, mask içinde değil
            if (dp[mask][u] == 0) continue;   // Bu duruma hiç gelinmedi

            for (int v = 0; v < n; v++) {
                // v henüz gezilmemiş ve u-v arası kenar var
                if (mat[u][v] && !(mask & (1 << v))) {
                    int newMask = mask | (1 << v);
                    dp[newMask][v] = (dp[newMask][v] + dp[mask][u]) % MOD;
                }
            }
        }
    }

    // Tüm düğümler gezilmiş, son durak n-1
    return dp[(1 << n) - 1][n - 1];
}

// ─────────────────────────────────────────────
//  MAIN — Test
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Euler Circuit Testi ──
    //  Graf: 1-2-3-4-1 dörtgen + 2-4 diyagonal
    //  Tüm düğümler çift dereceli → Euler Circuit var
    V = 4;
    totalEdges = 5;

    // degree takibi
    auto addE = [&](int u, int v, int id) {
        addEdgeEuler(u, v, id);
        degree[u]++;
        degree[v]++;
    };
    addE(1, 2, 0); addE(2, 3, 1); addE(3, 4, 2);
    addE(4, 1, 3); addE(2, 4, 4);

    cout << "=== Euler Koşul Kontrolü ===" << endl;
    int cond = checkEulerCondition();
    if (cond == 1) cout << "  Euler Circuit mümkün!" << endl;
    else if (cond == 2) cout << "  Euler Path mümkün (tek dereceli düğümden başla)" << endl;
    else cout << "  Euler yolu/çevrimi MÜMKÜN DEĞİL" << endl;

    cout << "\n=== Euler Circuit ===" << endl;
    vector<int> circuit = findEulerCircuit(1);
    if (circuit.empty()) {
        cout << "  Euler circuit bulunamadı!" << endl;
    } else {
        cout << "  Euler Circuit: ";
        for (int i = 0; i < (int)circuit.size(); i++) {
            cout << circuit[i];
            if (i + 1 < (int)circuit.size()) cout << " → ";
        }
        cout << endl;
    }

    // ── Hamilton Yolu Sayısı ──
    cout << "\n=== Hamilton Yolu Sayısı (N=4) ===" << endl;
    // 4 düğümlü tam graf (her iki düğüm arası kenar var)
    int n = 4;
    vector<vector<int>> mat(n, vector<int>(n, 1));
    for (int i = 0; i < n; i++) mat[i][i] = 0; // Kendine kenar yok

    cout << "  Tam grafta (0'dan başlayıp 3'te biten) "
         << "Hamilton yolu sayısı: " << countHamiltonianPaths(n, mat) << endl;

    return 0;
}

/*
 * ÖZET:
 *
 * Euler Path/Circuit → Her KENAR tam 1 kez geçilir
 *   Koşul (yönsüz): 0 veya 2 tek dereceli düğüm
 *   Algoritma: Hierholzer → O(V+E)
 *
 * Hamilton Path → Her DÜĞÜM tam 1 kez geçilir
 *   NP-Hard → N ≤ 20 için Bitmask DP: O(N² × 2^N)
 *   dp[mask][u] = mask kümesini gezmiş, u'da son durak
 *
 * Fark:
 *   Euler → Kenar sayısı, polinom zaman
 *   Hamilton → Düğüm sayısı, NP-Hard
 */
