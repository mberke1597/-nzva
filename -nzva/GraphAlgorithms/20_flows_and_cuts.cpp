// ============================================================
//  20 - Akışlar ve Kesiler (Flows and Cuts)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 20
//
//  ✅ struct/class YOK → global capacity matrisi, adjList
//  ✅ Edmonds-Karp (BFS tabanlı Ford-Fulkerson) → O(V·E²)
//  ✅ Max-Flow Min-Cut Teoremi
//  ✅ Bipartite Maximum Matching
// ============================================================

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
// ─────────────────────────────────────────────
const int MAXN = 505; // Akış grafları genellikle küçük olur

// Kapasite matrisi: capacity[u][v] = u→v kenarının kalan kapasitesi
// (Residual graph: ters kenarlar da burada tutulur)
int capacity[MAXN][MAXN];

// Adjacency list — kenar listesi (ters kenar dahil)
vector<int> adjList[MAXN];

int V; // Düğüm sayısı

// ─────────────────────────────────────────────
//  YARDIMCI: Kenar Ekle
//
//  Her u→v kenarı eklenirken ters v→u kenarı da eklenir.
//  Ters kenar başlangıçta kapasitesi 0'dır ama akış iadesi için gereklidir.
//  "Gönderilen akışı geri alabilmek" → algoritmanın doğruluğunu sağlar.
// ─────────────────────────────────────────────
void addEdge(int u, int v, int cap) {
    adjList[u].push_back(v);
    adjList[v].push_back(u);    // Ters kenar (residual)
    capacity[u][v] += cap;
    // capacity[v][u] başlangıçta 0 (ters kenar kapasitesi)
}

// ─────────────────────────────────────────────
//  EDMONDS-KARP ALGORİTMASI (Max-Flow)
//
//  NEDEN KULLANILIR?
//  "Kaynaktan (source) hedefe (sink) ne kadar maksimum akış gönderilebilir?"
//
//  Gerçek dünya kullanımları:
//  - Boru hattı / su şebekesi → maksimum akış kapasitesi
//  - İnternet ağı → maksimum bant genişliği
//  - Bipartite eşleştirme → max matching = max flow
//  - Ayrık yol sayısı → kapasite=1 yapıp max flow bul
//
//  MAX-FLOW MIN-CUT TEOREMİ:
//  Maksimum akış = Minimum kenar kesimi (min-cut) kapasitesine eşittir.
//  "Ağı ikiye bölen minimum maliyet = Maksimum taşınabilecek akış"
//
//  ALGORİTMA:
//  1. BFS ile kaynaktan hedefe "artırılabilir yol" bul (residual capacity > 0)
//  2. Bu yoldaki en dar kenar kapasitesi = o tur gönderilebilecek akış
//  3. Kapasiteleri güncelle: ileri kenarlarda azalt, ters kenarlarda artır
//  4. Artırılabilir yol kalmadığında bitti
//
//  Ford-Fulkerson'dan farkı: BFS kullanması → en kısa artırma yolunu seçer
//  → O(V·E²) garantili (DFS kullanınca garantisiz olabilir)
//
//  Karmaşıklık: O(V · E²)
// ─────────────────────────────────────────────
int bfsAugment(int s, int t, vector<int>& parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2; // Kaynak düğümü işaretle

    // {düğüm, o ana kadar gönderilebilecek maksimum akış}
    queue<pair<int,int>> q;
    q.push({s, INT_MAX});

    while (!q.empty()) {
        auto [u, flow] = q.front(); q.pop();

        for (int v : adjList[u]) {
            // Ziyaret edilmemiş ve kapasitesi olan kenar
            if (parent[v] == -1 && capacity[u][v] > 0) {
                parent[v]    = u;
                int newFlow  = min(flow, capacity[u][v]);
                if (v == t) return newFlow; // Hedefe ulaştık!
                q.push({v, newFlow});
            }
        }
    }
    return 0; // Artırılabilir yol yok
}

int edmondsKarp(int s, int t) {
    int totalFlow = 0;
    vector<int> parent(V + 1);
    int addedFlow;

    // Artırılabilir yol kalmadığı sürece devam et
    while ((addedFlow = bfsAugment(s, t, parent)) > 0) {
        totalFlow += addedFlow;

        // Yolu geri iz sürerek kapasiteleri güncelle
        int curr = t;
        while (curr != s) {
            int prev           = parent[curr];
            capacity[prev][curr] -= addedFlow; // İleri kenar kapasitesini azalt
            capacity[curr][prev] += addedFlow; // Ters kenar kapasitesini artır
            curr               = prev;
        }
    }

    return totalFlow;
}

// ─────────────────────────────────────────────
//  BİPARTİTE MAKSİMUM EŞLEŞTİRME
//
//  NEDEN KULLANILIR?
//  Sol küme (işçiler) ile sağ küme (görevler) arasında
//  maksimum sayıda eşleştirme bul. Her işçi en fazla bir göreve atanabilir.
//
//  DÖNÜŞÜM → Max-Flow:
//  1. Sahte kaynak S → tüm sol düğümlere kapasite-1 kenar
//  2. Tüm sağ düğümler → sahte hedef T'ye kapasite-1 kenar
//  3. Sol-sağ kenarları kopyala (kapasite 1)
//  4. Max-Flow(S, T) = Maksimum Eşleştirme
// ─────────────────────────────────────────────
int bipartiteMatching(int leftN, int rightN,
                      const vector<pair<int,int>>& matchEdges) {
    // Düğüm numaralandırması:
    // Sol: 1..leftN
    // Sağ: leftN+1..leftN+rightN
    // S = 0, T = leftN+rightN+1
    int S = 0;
    int T = leftN + rightN + 1;
    V     = T + 1; // Toplam düğüm sayısı

    // Kapasiteleri sıfırla
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            capacity[i][j] = 0;
    for (int i = 0; i < V; i++) adjList[i].clear();

    // S → sol düğümler (kapasite 1)
    for (int i = 1; i <= leftN; i++)
        addEdge(S, i, 1);

    // Sağ düğümler → T (kapasite 1)
    for (int j = 1; j <= rightN; j++)
        addEdge(leftN + j, T, 1);

    // Sol-sağ eşleştirme kenarları (kapasite 1)
    for (auto [l, r] : matchEdges)
        addEdge(l, leftN + r, 1);

    return edmondsKarp(S, T);
}

// ─────────────────────────────────────────────
//  MAIN — Test
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Max-Flow Testi ──
    // Graf:
    //  S=1 --(10)--> 2 --(10)--> T=4
    //  S=1 --(10)--> 3 --(10)--> T=4
    //           2 --(1)--> 3
    //
    //  Max-Flow = 20 (iki yol: 1→2→4 ve 1→3→4)
    V = 4;
    addEdge(1, 2, 10);
    addEdge(1, 3, 10);
    addEdge(2, 4, 10);
    addEdge(3, 4, 10);
    addEdge(2, 3, 1);

    cout << "=== Edmonds-Karp Max-Flow ===" << endl;
    cout << "  Max-Flow (1 → 4) = " << edmondsKarp(1, 4)
         << "  (beklenen: 20)" << endl;

    // ── Bipartite Matching Testi ──
    // İşçiler: {A=1, B=2, C=3}
    // Görevler: {X=1, Y=2, Z=3}
    // Kenarlar: A-X, A-Y, B-Y, B-Z, C-Z
    // Maksimum eşleştirme: A-X, B-Y, C-Z = 3
    cout << "\n=== Bipartite Maximum Matching ===" << endl;
    vector<pair<int,int>> matchEdges = {
        {1,1}, {1,2}, // A-X, A-Y
        {2,2}, {2,3}, // B-Y, B-Z
        {3,3}         // C-Z
    };
    int maxMatch = bipartiteMatching(3, 3, matchEdges);
    cout << "  Maksimum Eşleştirme = " << maxMatch
         << "  (beklenen: 3)" << endl;

    return 0;
}

/*
 * ÖZET:
 *
 * Max-Flow → Kaynaktan hedefe maksimum akış
 *   Edmonds-Karp (BFS augmenting path): O(V·E²)
 *   Residual graph: ters kenarlar akış iadesini sağlar
 *
 * Max-Flow = Min-Cut (Menger Teoremi)
 *   Ağı ikiye bölen minimum kapasiteli kenar kümesi = Max-Flow değeri
 *
 * Bipartite Matching → Max-Flow'a dönüştür (kapasite=1)
 *   Maksimum Eşleştirme = Max-Flow(S, T)
 *
 * Disjoint Paths:
 *   Kenar ayrık yollar: kapasite=1, max-flow bul
 *   Düğüm ayrık yollar: u → u_in --(1)--> u_out, sonra max-flow
 */
