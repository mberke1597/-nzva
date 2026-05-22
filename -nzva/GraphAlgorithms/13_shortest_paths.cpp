// ============================================================
//  13 - En Kısa Yollar (Shortest Paths)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 13
//
//  ✅ struct/class YOK → global adjList / edgeList kullanılır
//  ✅ Bellman-Ford  → negatif kenar, negatif döngü tespiti
//  ✅ Dijkstra      → min-heap ile O((V+E)logV)
//  ✅ Floyd-Warshall → tüm çiftler, O(V³), adjMatrix üzerinde
// ============================================================

#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <climits>
using namespace std;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
// ─────────────────────────────────────────────
const int    MAXN  = 100005;
const long long INF = 1e18;

// Adjacency List — ağırlıklı: adjList[u] = {v, w}
// Dijkstra ve genel amaçlı kullanım için
vector<pair<int,int>> adjList[MAXN];

// Edge List — {u, v, w}: Bellman-Ford tüm kenarları gezar
vector<tuple<int,int,int>> edges;

// Floyd-Warshall için 2D mesafe matrisi
// dist_fw[i][j] = i'den j'ye en kısa yol
// Küçük graflar için: MAXV küçük tutulmalı (V ≤ 500)
const int MAXV = 505;
long long dist_fw[MAXV][MAXV];

int V, E; // Düğüm ve kenar sayısı

// ─────────────────────────────────────────────
//  YARDIMCI: Kenar Ekle
// ─────────────────────────────────────────────
// Yönlü ağırlıklı kenar (Dijkstra + Bellman-Ford için)
void addEdgeDirected(int u, int v, int w) {
    adjList[u].push_back({v, w});
    edges.push_back({u, v, w});
}

// Yönsüz ağırlıklı kenar
void addEdgeUndirected(int u, int v, int w) {
    adjList[u].push_back({v, w});
    adjList[v].push_back({u, w});
    edges.push_back({u, v, w});
    edges.push_back({v, u, w});
}

// ─────────────────────────────────────────────
//  1) BELLMAN-FORD
//
//  NEDEN BELLMAN-FORD?
//  Negatif ağırlıklı kenarlar içeren graflarda Dijkstra çalışmaz.
//  Bellman-Ford V-1 kez tüm kenarları gevşeterek doğru sonucu verir.
//  Eğer V. iterasyonda hala güncelleme oluyorsa → negatif döngü!
//
//  Örnek kullanım: Para birimi arbitraj tespiti, negatif maliyetli yollar
//
//  dist[v] = src'den v'ye en kısa mesafe
//  Fonksiyon true dönerse → negatif döngü var
//
//  Karmaşıklık: O(V * E)
// ─────────────────────────────────────────────
bool bellmanFord(int src, vector<long long>& dist) {
    dist.assign(V + 1, INF);
    dist[src] = 0;

    // V-1 kez tüm kenarları gevşet
    for (int iter = 1; iter <= V - 1; iter++) {
        bool updated = false; // Erken çıkış optimizasyonu

        for (auto& [u, v, w] : edges) {
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v]  = dist[u] + w;
                updated  = true;
            }
        }

        if (!updated) break; // Hiçbir güncelleme yoksa bitti
    }

    // V. iterasyon: hala güncelleme var mı? → negatif döngü!
    for (auto& [u, v, w] : edges) {
        if (dist[u] != INF && dist[u] + w < dist[v]) {
            return true; // Negatif döngü tespit edildi
        }
    }
    return false;
}

// ─────────────────────────────────────────────
//  2) DIJKSTRA (Min-Heap ile)
//
//  NEDEN DIJKSTRA?
//  Negatif kenar yoksa Dijkstra çok daha hızlıdır: O((V+E)logV)
//  GPS navigasyon, oyun AI, ağ routing'in standart algoritması.
//
//  Mantık:
//  - Priority queue (min-heap) → en yakın düğümü seç
//  - Seçilen düğümün komşularını güncelle (relax)
//  - Heap'ten çıkan düğümün mesafesi artık kesin (greedy)
//
//  dist[v] = src'den v'ye en kısa mesafe
//  dist[v] == INF → v erişilemez
//
//  Karmaşıklık: O((V + E) log V)
// ─────────────────────────────────────────────
void dijkstra(int src, vector<long long>& dist) {
    dist.assign(V + 1, INF);
    dist[src] = 0;

    // Min-heap: {mesafe, düğüm}
    // greater<> ile küçük mesafeli düğüm önce çıkar
    priority_queue<
        pair<long long, int>,
        vector<pair<long long, int>>,
        greater<pair<long long, int>>
    > pq;

    pq.push({0LL, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // Stale entry: heap'te daha eski, daha büyük bir mesafe var
        if (d > dist[u]) continue;

        for (auto& [v, w] : adjList[u]) {
            // Gevşetme (relax): u üzerinden v'ye daha kısa yol var mı?
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
}

// Dijkstra + Yol Geri İzleme (path reconstruction)
void dijkstraWithPath(int src, int target,
                      vector<long long>& dist,
                      vector<int>& parent) {
    dist.assign(V + 1, INF);
    parent.assign(V + 1, -1);
    dist[src] = 0;

    priority_queue<
        pair<long long, int>,
        vector<pair<long long, int>>,
        greater<pair<long long, int>>
    > pq;
    pq.push({0LL, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;

        for (auto& [v, w] : adjList[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v]   = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // Yolu yazdır
    if (dist[target] == INF) {
        cout << "  Yol yok!" << endl;
        return;
    }

    vector<int> path;
    for (int v = target; v != -1; v = parent[v])
        path.push_back(v);
    reverse(path.begin(), path.end());

    cout << "  En kısa yol (" << src << " → " << target
         << ", maliyet=" << dist[target] << "): ";
    for (int i = 0; i < (int)path.size(); i++) {
        cout << path[i];
        if (i + 1 < (int)path.size()) cout << " → ";
    }
    cout << endl;
}

// ─────────────────────────────────────────────
//  3) FLOYD-WARSHALL
//
//  NEDEN FLOYD-WARSHALL?
//  Tüm düğüm çiftleri arasındaki en kısa yol lazımsa.
//  "i'den j'ye en kısa yol?" sorusuna O(1)'de cevap vermek için
//  O(V³) ön işlemle tabloyu doldur.
//
//  Mantık: Her k düğümünü ara nokta olarak dene:
//    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])
//
//  Graf adjMatrix olarak kurulur, dist_fw üzerinde in-place çalışır.
//  Negatif kenarlarla çalışır; dist_fw[i][i] < 0 ise negatif döngü var.
//
//  Karmaşıklık: O(V³) → sadece küçük graflar için (V ≤ 500)
// ─────────────────────────────────────────────
void initFloydWarshall(int n) {
    // Başlangıç: tüm mesafeler INF, aynı düğüme 0
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            dist_fw[i][j] = (i == j) ? 0 : INF;
}

void floydWarshallAddEdge(int u, int v, long long w) {
    // Çoklu kenar varsa en küçüğü al
    dist_fw[u][v] = min(dist_fw[u][v], w);
    // Yönsüz ise:
    dist_fw[v][u] = min(dist_fw[v][u], w);
}

void floydWarshall(int n) {
    for (int k = 1; k <= n; k++) {        // Ara nokta
        for (int i = 1; i <= n; i++) {    // Kaynak
            for (int j = 1; j <= n; j++) { // Hedef
                if (dist_fw[i][k] < INF && dist_fw[k][j] < INF) {
                    dist_fw[i][j] = min(dist_fw[i][j],
                                        dist_fw[i][k] + dist_fw[k][j]);
                }
            }
        }
    }
}

void printFloydMatrix(int n) {
    cout << "=== Floyd-Warshall Mesafe Matrisi ===" << endl;
    cout << "     ";
    for (int j = 1; j <= n; j++) cout << j << "     ";
    cout << endl;
    for (int i = 1; i <= n; i++) {
        cout << i << "  [";
        for (int j = 1; j <= n; j++) {
            if (dist_fw[i][j] >= INF / 2) cout << "  INF";
            else cout << "  " << dist_fw[i][j] << "  ";
        }
        cout << "]" << endl;
    }
}

// ─────────────────────────────────────────────
//  MAIN — Test
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Graf:
    //  1 --(4)--> 2 --(1)--> 4
    //  |                     ^
    //  └--(2)--> 3 --(5)-----┘
    //
    //  En kısa: 1→2→4 = 5,   1→3→4 = 7,   1→2 = 4
    V = 4; E = 4;
    addEdgeDirected(1, 2, 4);
    addEdgeDirected(1, 3, 2);
    addEdgeDirected(2, 4, 1);
    addEdgeDirected(3, 4, 5);

    // ── Dijkstra ──
    cout << "=== Dijkstra (kaynak: 1) ===" << endl;
    vector<long long> dijkDist;
    dijkstra(1, dijkDist);
    for (int i = 1; i <= V; i++) {
        cout << "  1 → " << i << " : ";
        if (dijkDist[i] == INF) cout << "INF" << endl;
        else cout << dijkDist[i] << endl;
    }

    cout << "\n=== Dijkstra Yol Geri İzleme ===" << endl;
    vector<long long> distPath;
    vector<int> parent;
    dijkstraWithPath(1, 4, distPath, parent);

    // ── Bellman-Ford ──
    cout << "\n=== Bellman-Ford (kaynak: 1) ===" << endl;
    vector<long long> bfDist;
    bool negCycle = bellmanFord(1, bfDist);
    if (negCycle) {
        cout << "  UYARI: Negatif döngü tespit edildi!" << endl;
    } else {
        for (int i = 1; i <= V; i++) {
            cout << "  1 → " << i << " : ";
            if (bfDist[i] == INF) cout << "INF" << endl;
            else cout << bfDist[i] << endl;
        }
    }

    // ── Floyd-Warshall ──
    cout << "\n=== Floyd-Warshall (tüm çiftler) ===" << endl;
    int fwV = 4;
    initFloydWarshall(fwV);
    floydWarshallAddEdge(1, 2, 4);
    floydWarshallAddEdge(1, 3, 2);
    floydWarshallAddEdge(2, 4, 1);
    floydWarshallAddEdge(3, 4, 5);
    floydWarshall(fwV);
    printFloydMatrix(fwV);

    return 0;
}

/*
 * ÖZET:
 *
 * BFS          → Ağırlıksız en kısa (kenar sayısı)
 * Dijkstra     → Ağırlıklı, pozitif kenar, tek kaynak  ← En çok kullanılan
 * Bellman-Ford → Negatif kenar, negatif döngü tespiti
 * Floyd        → Küçük grafta tüm çiftler
 *
 * Relax formülü (hepsinde ortak):
 *   if dist[u] + w < dist[v]:
 *       dist[v] = dist[u] + w
 */
