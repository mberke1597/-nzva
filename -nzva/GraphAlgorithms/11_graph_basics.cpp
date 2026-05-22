// ============================================================
//  11 - Graf Gösterimi (Graph Representation)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 11
//
//  ✅ struct/class YOK → global adjList / adjMatrix kullanılır
//  ✅ Ağırlıklı ve ağırlıksız kenar ekleme örnekleri
//  ✅ Edge List (Kruskal/Bellman-Ford için)
// ============================================================

#include <iostream>
#include <vector>
#include <tuple>
using namespace std;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
//  (struct/class yerine doğrudan global diziler)
// ─────────────────────────────────────────────
const int MAXN = 100005;

// 1) Adjacency List — ağırlıklı: adjList[u] = { {v, w}, ... }
//    Seyrek (sparse) graflar için idealdir. O(V+E) bellek.
vector<pair<int,int>> adjList[MAXN];

// 2) Adjacency Matrix — adjMatrix[u][v] = w (0 ise kenar yok)
//    Küçük, yoğun graflar için kullan. O(V²) bellek.
//    Not: V > ~3000 ise bellek taşar; dikkatli ol.
int adjMatrix[1005][1005];  // küçük graflar için boyutu küçük tuttuk

// 3) Edge List — {w, u, v} (Kruskal'da ağırlığa göre sıralarız)
vector<tuple<int,int,int>> edgeList;   // {u, v, w}

// Toplam düğüm ve kenar sayısı
int V, E;

// ─────────────────────────────────────────────
//  KENAR EKLEME FONKSİYONLARI
// ─────────────────────────────────────────────

// Yönsüz ağırlıklı kenar ekle (her iki yönde de)
void addEdgeUndirected(int u, int v, int w = 1) {
    adjList[u].push_back({v, w});
    adjList[v].push_back({u, w});

    adjMatrix[u][v] = w;
    adjMatrix[v][u] = w;

    edgeList.push_back({u, v, w});
}

// Yönlü (directed) ağırlıklı kenar ekle (sadece u→v)
void addEdgeDirected(int u, int v, int w = 1) {
    adjList[u].push_back({v, w});

    adjMatrix[u][v] = w;

    edgeList.push_back({u, v, w});
}

// ─────────────────────────────────────────────
//  YAZDIRMA FONKSİYONLARI
// ─────────────────────────────────────────────

void printAdjList() {
    cout << "=== Adjacency List ===" << endl;
    for (int u = 1; u <= V; u++) {
        cout << u << " → ";
        for (auto [v, w] : adjList[u]) {
            cout << "(" << v << ", w=" << w << ")  ";
        }
        cout << endl;
    }
}

void printAdjMatrix() {
    cout << "=== Adjacency Matrix ===" << endl;
    cout << "   ";
    for (int j = 1; j <= V; j++) cout << j << "  ";
    cout << endl;
    for (int i = 1; i <= V; i++) {
        cout << i << "  ";
        for (int j = 1; j <= V; j++) {
            cout << adjMatrix[i][j] << "  ";
        }
        cout << endl;
    }
}

void printEdgeList() {
    cout << "=== Edge List ===" << endl;
    for (auto [u, v, w] : edgeList) {
        cout << u << " --(" << w << ")-- " << v << endl;
    }
}

// ─────────────────────────────────────────────
//  MAIN — Örnek Graf
//
//  Graf:
//    1 --(5)-- 2
//    1 --(3)-- 3
//    2 --(7)-- 4
//    3 --(2)-- 4
//    4 --(1)-- 5
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    V = 5; E = 5;

    addEdgeUndirected(1, 2, 5);
    addEdgeUndirected(1, 3, 3);
    addEdgeUndirected(2, 4, 7);
    addEdgeUndirected(3, 4, 2);
    addEdgeUndirected(4, 5, 1);

    printAdjList();
    cout << endl;
    printAdjMatrix();
    cout << endl;
    printEdgeList();

    return 0;
}

/*
 * ÇIKTI:
 * === Adjacency List ===
 * 1 → (2, w=5)  (3, w=3)
 * 2 → (1, w=5)  (4, w=7)
 * 3 → (1, w=3)  (4, w=2)
 * 4 → (2, w=7)  (3, w=2)  (5, w=1)
 * 5 → (4, w=1)
 *
 * NEDEN ADJ LIST?
 * → Kenar sayısı V² den az (seyrek graf) → Adjacency List daha verimli.
 * → BFS, DFS, Dijkstra, Prim hepsi adjList üzerinde çalışır.
 *
 * NEDEN ADJ MATRIX?
 * → "1-3 arası kenar var mı?" sorusunu O(1)'de cevaplamak için.
 * → Floyd-Warshall'da tüm çift mesafeleri hesaplamak için.
 */
