// ============================================================
//  17 - Kuvvetli Bağlantı (Strong Connectivity / SCC)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 17
//
//  ✅ struct/class YOK → global adjList, adjRev, visited, component
//  ✅ Kosaraju Algoritması → O(V+E), iki DFS
//  ✅ SCC condensation graph (yoğunlaştırılmış DAG)
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
// ─────────────────────────────────────────────
const int MAXN = 100005;

// Orijinal yönlü graf
vector<int> adjList[MAXN];

// Ters yönlü graf (transpose): her u→v kenarı v→u olur
vector<int> adjRev[MAXN];

bool visited[MAXN];
int  component[MAXN]; // Her düğümün ait olduğu SCC id'si (-1 = atanmamış)
vector<int> finishOrder; // DFS bitiş sırasına göre düğümler

int V; // Düğüm sayısı

// ─────────────────────────────────────────────
//  YARDIMCI
// ─────────────────────────────────────────────
void addEdge(int u, int v) {
    adjList[u].push_back(v);
    adjRev[v].push_back(u); // Ters kenar da ekle
}

// ─────────────────────────────────────────────
//  KOSARAJU ALGORITMASI
//
//  NEDEN KULLANILIR?
//  "Birbirinden ulaşılabilen düğüm grupları (SCC) nelerdir?"
//  Web crawler: güçlü bağlantılı sayfaları grupla.
//  2-SAT çözümü için SCC gereklidir.
//  Graf sıkıştırma: Her SCC tek düğüme indirilir → DAG elde edilir.
//
//  Mantık (3 Adım):
//  1. Orijinal grafta DFS → bitiş zamanlarına göre düğümleri stack'e at
//  2. Ters grafı oluştur (addEdge'de adjRev zaten hazırlandı)
//  3. Stack'teki sıraya göre ters grafta DFS → Her DFS = 1 SCC
//
//  Neden çalışır?
//  - Orijinal grafta en son biten SCC, ters grafta "kaynak SCC"dir
//    (diğer SCC'lere giden kenar yoktur). Oradan başlayarak ters
//    grafta yalnızca kendi SCC'sine ulaşılabilir.
//
//  Karmaşıklık: O(V + E)
// ─────────────────────────────────────────────

// Adım 1: Orijinal grafta DFS, bitiş sırasına göre push
void dfs1(int u) {
    visited[u] = true;
    for (int v : adjList[u]) {
        if (!visited[v]) dfs1(v);
    }
    finishOrder.push_back(u); // Post-order: tamamen bitince ekle
}

// Adım 3: Ters grafta DFS, aynı SCC'ye ait düğümleri bul
void dfs2(int u, int compId) {
    component[u] = compId;
    for (int v : adjRev[u]) {
        if (component[v] == -1) dfs2(v, compId);
    }
}

// Tüm SCC'leri bul, kaç tane olduğunu döndür
int kosarajuSCC() {
    // Adım 1: Orijinal grafta tüm düğümleri DFS ile gez
    fill(visited + 1, visited + V + 1, false);
    finishOrder.clear();
    for (int i = 1; i <= V; i++) {
        if (!visited[i]) dfs1(i);
    }

    // Adım 2: finishOrder'ın sonundan başlayarak (en geç biten = kaynak SCC)
    fill(component + 1, component + V + 1, -1);
    int numSCC = 0;
    for (int i = V - 1; i >= 0; i--) {
        int u = finishOrder[i];
        if (component[u] == -1) {
            dfs2(u, numSCC);
            numSCC++;
        }
    }

    return numSCC;
}

// SCC'leri yazdır
void printSCCs(int numSCC) {
    vector<vector<int>> sccs(numSCC);
    for (int i = 1; i <= V; i++) {
        sccs[component[i]].push_back(i);
    }

    cout << "  " << numSCC << " SCC bulundu:" << endl;
    for (int i = 0; i < numSCC; i++) {
        cout << "  SCC " << i << ": { ";
        for (int u : sccs[i]) cout << u << " ";
        cout << "}" << endl;
    }
}

// ─────────────────────────────────────────────
//  MAIN — Test
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Graf:
    //  1 → 2 → 3 → 1   (SCC: {1,2,3})
    //          ↓
    //          4 → 5 → 4   (SCC: {4,5})
    //          ↓
    //          6          (SCC: {6})
    V = 6;
    addEdge(1, 2); addEdge(2, 3); addEdge(3, 1); // 1-2-3 döngüsü
    addEdge(3, 4);
    addEdge(4, 5); addEdge(5, 4); // 4-5 döngüsü
    addEdge(5, 6);

    cout << "=== Kosaraju SCC ===" << endl;
    int numSCC = kosarajuSCC();
    printSCCs(numSCC);

    // Kondensasyon grafiği (SCC → SCC kenarları)
    cout << "\n=== Condensation Graph (SCC→SCC kenarlar) ===" << endl;
    set<pair<int,int>> condEdges;
    for (int u = 1; u <= V; u++) {
        for (int v : adjList[u]) {
            if (component[u] != component[v]) {
                condEdges.insert({component[u], component[v]});
            }
        }
    }
    for (auto [u, v] : condEdges) {
        cout << "  SCC" << u << " → SCC" << v << endl;
    }

    return 0;
}

/*
 * ÖZET:
 *
 * SCC = Strongly Connected Component
 *     = Birbirinden karşılıklı ulaşılabilen maksimum düğüm kümesi
 *
 * Kosaraju:
 *   1. Orijinal grafta DFS → bitiş sırasını kaydet
 *   2. Ters grafta, bitiş sırasının tersinden DFS
 *   3. Her DFS araması = bir SCC
 *
 * Condensation Graph:
 *   Her SCC tek düğüme indirilir → döngüsüz DAG elde edilir
 *   DAG üzerinde DP, topo sıralama uygulanabilir
 *
 * 2-SAT bağlantısı:
 *   x ve ¬x aynı SCC'deyse → ÇÖZÜMSÜZ
 *   Aksi halde → SCC condensation DAG'ından değer ata
 */
