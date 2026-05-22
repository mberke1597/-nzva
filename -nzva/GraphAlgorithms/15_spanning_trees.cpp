// ============================================================
//  15 - Kapsayan Ağaçlar (Spanning Trees / MST)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 15
//
//  ✅ struct/class YOK → global edgeList, adjList, DSU dizileri
//  ✅ Kruskal → Edge list + DSU
//  ✅ Prim    → adjList + min-heap
//  ✅ DSU (Disjoint Set Union) — Path Compression + Union by Size
// ============================================================

#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <numeric>
using namespace std;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
// ─────────────────────────────────────────────
const int MAXN = 100005;
const long long INF = 1e18;

// Adjacency List — ağırlıklı (Prim için)
vector<pair<int,int>> adjList[MAXN]; // {v, w}

// Edge List — {w, u, v} (Kruskal: ağırlığa göre sıralayacağız)
vector<tuple<int,int,int>> edgeList; // {w, u, v}

int V, E;

// ─────────────────────────────────────────────
//  DSU — Disjoint Set Union (Union-Find)
//
//  NEDEN KULLANILIR?
//  Kruskal'da "bu iki düğüm aynı bileşende mi?" sorusunu
//  neredeyse O(1)'de cevaplamak için.
//  Genel olarak: Dinamik bileşen birleştirme ve kontrol.
//
//  İki optimizasyon:
//  1. Path Compression: find() çağrısında düğümü doğrudan köke bağla
//  2. Union by Size: Küçük ağacı büyüğün altına bağla
// ─────────────────────────────────────────────
int dsuParent[MAXN];
int dsuSize[MAXN];

void dsuInit(int n) {
    for (int i = 0; i <= n; i++) {
        dsuParent[i] = i; // Her düğüm kendi kümesinin kökü
        dsuSize[i]   = 1;
    }
}

// Kümesinin kökünü bul (Path Compression ile)
int dsuFind(int x) {
    if (dsuParent[x] != x)
        dsuParent[x] = dsuFind(dsuParent[x]); // Yol sıkıştırma
    return dsuParent[x];
}

// İki kümeyi birleştir (Union by Size)
// Aynı kümedeyse false döner (döngü oluşur)
bool dsuUnite(int a, int b) {
    a = dsuFind(a);
    b = dsuFind(b);

    if (a == b) return false; // Zaten aynı bileşen → döngü oluşur

    // Küçüğü büyüğün altına bağla
    if (dsuSize[a] < dsuSize[b]) swap(a, b);
    dsuParent[b]  = a;
    dsuSize[a]   += dsuSize[b];
    return true;
}

// ─────────────────────────────────────────────
//  KENAR EKLEME
// ─────────────────────────────────────────────
void addEdge(int u, int v, int w) {
    adjList[u].push_back({v, w});
    adjList[v].push_back({u, w});
    edgeList.push_back({w, u, v}); // Kruskal: ağırlık öne al
}

// ─────────────────────────────────────────────
//  1) KRUSKAL ile MST
//
//  NEDEN KRUSKAL?
//  Seyrek (sparse) graflarda hız avantajı: O(E log E)
//  Kenarları sıralayıp DSU ile döngü kontrolü — çok basit mantık.
//  Competitive programming'de en sık kullanılan MST algoritması.
//
//  Mantık:
//  1. Tüm kenarları ağırlığa göre küçükten büyüğe sırala
//  2. Her kenar için: DSU ile döngü oluşturur mu kontrol et
//  3. Oluşturmuyorsa MST'ye ekle
//
//  Karmaşıklık: O(E log E)
// ─────────────────────────────────────────────
long long kruskalMST(vector<pair<int,int>>& mstEdges) {
    sort(edgeList.begin(), edgeList.end()); // Ağırlığa göre sırala
    dsuInit(V);

    long long totalWeight = 0;
    mstEdges.clear();

    for (auto& [w, u, v] : edgeList) {
        if (dsuUnite(u, v)) {
            // Döngü oluşturmadı → MST'ye ekle
            totalWeight += w;
            mstEdges.push_back({u, v});

            // V-1 kenar eklenince MST tamamlandı
            if ((int)mstEdges.size() == V - 1) break;
        }
    }

    return totalWeight;
}

// ─────────────────────────────────────────────
//  2) PRIM ile MST
//
//  NEDEN PRIM?
//  Yoğun (dense) graflarda Kruskal'dan daha verimli olabilir.
//  Adjacency List + min-heap kombinasyonu ile çalışır.
//  "Başlangıç düğümünden büyüyen ağaç" mantığı sezgisel.
//
//  Mantık:
//  1. src düğümünü MST'ye ekle
//  2. MST'deki düğümlerden dışarı çıkan en ucuz kenarı seç
//  3. O kenarla yeni düğümü MST'ye ekle
//  4. V-1 kenar eklenene kadar devam et
//
//  Karmaşıklık: O((V + E) log V)
// ─────────────────────────────────────────────
long long primMST(int src = 1) {
    vector<bool> inMST(V + 1, false);
    long long totalWeight = 0;

    // Min-heap: {ağırlık, düğüm}
    priority_queue<
        pair<int,int>,
        vector<pair<int,int>>,
        greater<pair<int,int>>
    > pq;

    pq.push({0, src});

    while (!pq.empty()) {
        auto [w, u] = pq.top(); pq.pop();

        if (inMST[u]) continue; // Zaten MST'de

        inMST[u]     = true;
        totalWeight += w;

        for (auto& [v, weight] : adjList[u]) {
            if (!inMST[v]) {
                pq.push({weight, v});
            }
        }
    }

    return totalWeight;
}

// ─────────────────────────────────────────────
//  MAIN — Test
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Graf:
    //  1 --(4)-- 2 --(2)-- 4
    //  |         |         |
    // (1)       (3)       (5)
    //  |         |         |
    //  3 -------(-6)-------+
    //
    //  MST: 1-3(1), 1-2(4), 2-4(2) → toplam = 7
    V = 4; E = 5;
    addEdge(1, 2, 4);
    addEdge(1, 3, 1);
    addEdge(2, 3, 3);
    addEdge(2, 4, 2);
    addEdge(3, 4, 5);

    // ── Kruskal ──
    cout << "=== Kruskal MST ===" << endl;
    vector<pair<int,int>> mstEdges;
    long long kWeight = kruskalMST(mstEdges);
    cout << "  MST Ağırlığı: " << kWeight << endl;
    cout << "  MST Kenarları: ";
    for (auto [u, v] : mstEdges)
        cout << u << "-" << v << "  ";
    cout << endl;

    // ── Prim ──
    cout << "\n=== Prim MST ===" << endl;
    long long pWeight = primMST(1);
    cout << "  MST Ağırlığı: " << pWeight << endl;

    // ── DSU Bağlantı Kontrolü ──
    cout << "\n=== DSU Bağlantı Sorguları ===" << endl;
    dsuInit(V);
    dsuUnite(1, 2);
    dsuUnite(3, 4);
    cout << "  1-2 bağlı mı? " << (dsuFind(1) == dsuFind(2) ? "EVET" : "HAYIR") << endl;
    cout << "  1-3 bağlı mı? " << (dsuFind(1) == dsuFind(3) ? "EVET" : "HAYIR") << endl;
    dsuUnite(1, 3);
    cout << "  1-3 birleşti. Artık 1-4 bağlı mı? "
         << (dsuFind(1) == dsuFind(4) ? "EVET" : "HAYIR") << endl;

    return 0;
}

/*
 * ÖZET:
 *
 * MST = Tüm düğümleri kapsayan, toplam ağırlığı minimum olan ağaç
 *
 * Kruskal: Kenarları sırala + DSU ile döngü kontrolü  → Sparse graflarda hızlı
 * Prim:    Düğüm bazlı büyüyen ağaç + min-heap        → Dense graflarda hızlı
 *
 * DSU formülleri:
 *   find(x): x'in kümesinin kökünü bul (path compression ile)
 *   unite(a,b): a ve b kümelerini birleştir (union by size ile)
 *   Zaman: neredeyse O(1) amortize (α(n) inverse Ackermann)
 */
