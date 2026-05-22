// ============================================================
//  12 - Graf Gezinme: BFS & DFS (Graph Traversal)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 12
//
//  ✅ struct/class YOK → global adjList kullanılır
//  ✅ BFS ile mesafe hesaplama (ağırlıksız en kısa yol)
//  ✅ DFS ile mesafe/derinlik hesaplama
//  ✅ Döngü tespiti (yönsüz graf)
//  ✅ Bipartite kontrol (BFS ile 2-renklendirme)
//  ✅ Bağlı bileşen sayısı
// ============================================================

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
// ─────────────────────────────────────────────
const int MAXN = 100005;
const int INF  = 1e9;

// Adjacency List — ağırlıksız komşuluk listesi
// adjList[u] = { v1, v2, ... }   (yönsüz kenarlar için her iki yöne eklenir)
vector<int> adjList[MAXN];

bool   visited[MAXN];   // BFS/DFS ziyaret takibi
int    dist[MAXN];      // BFS: ağırlıksız en kısa mesafe
int    dfsDist[MAXN];   // DFS: derinlik/mesafe takibi
int    color[MAXN];     // Bipartite: 0=boyanmamış, 1 veya -1
int    state[MAXN];     // Döngü tespiti: 0=unvisited,1=visiting,2=done
int    componentId[MAXN]; // Bağlı bileşen numarası

int V, E; // Düğüm ve kenar sayısı

// ─────────────────────────────────────────────
//  YARDIMCI: Kenar Ekle
// ─────────────────────────────────────────────
void addEdge(int u, int v) {
    adjList[u].push_back(v);
    adjList[v].push_back(u); // Yönsüz graf
}

// ─────────────────────────────────────────────
//  1) BFS — Ağırlıksız En Kısa Mesafe
//
//  NEDEN BFS?
//  BFS, katman katman (level by level) ilerler.
//  start'tan ilk ulaşılan yol daima en kısa yoldur
//  çünkü her kenar eşit maliyet (1) taşır.
//
//  dist[v] = start'tan v'ye kenar sayısı (en kısa)
//  dist[v] == -1 → v erişilemez (farklı bileşen)
//
//  Karmaşıklık: O(V + E)
// ─────────────────────────────────────────────
void bfs(int start) {
    // Tüm mesafeleri "erişilmedi" olarak işaretle
    fill(dist + 1, dist + V + 1, -1);
    fill(visited + 1, visited + V + 1, false);

    queue<int> q;
    dist[start]    = 0;
    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adjList[u]) {
            if (!visited[v]) {
                visited[v] = true;
                dist[v]    = dist[u] + 1; // Bir adım daha uzakta
                q.push(v);
            }
        }
    }
}

// BFS en kısa yol GERİ İZİ: start'tan hedef'e gidilen düğümleri yazar
vector<int> bfsShortestPath(int start, int target) {
    fill(dist + 1, dist + V + 1, -1);

    vector<int> parent(V + 1, -1);
    queue<int> q;

    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == target) break;

        for (int v : adjList[u]) {
            if (dist[v] == -1) {
                dist[v]    = dist[u] + 1;
                parent[v]  = u;
                q.push(v);
            }
        }
    }

    // Yol yok
    if (dist[target] == -1) return {};

    // Yolu geri iz sürerek oluştur
    vector<int> path;
    for (int v = target; v != -1; v = parent[v])
        path.push_back(v);
    reverse(path.begin(), path.end());
    return path;
}

// ─────────────────────────────────────────────
//  2) DFS — Rekürsif (Derinlik/Mesafe ile)
//
//  NEDEN DFS?
//  - Döngü tespiti
//  - Topolojik sıralama
//  - Bağlı bileşen sayma
//  - Ağaç algoritmalarında (çap, alt ağaç vs.)
//
//  dfsDist[v] = start'tan v'ye DFS ile gidilen derinlik.
//  !! Ağırlıksız en kısa yolu garantilemez — bunun için BFS kullan.
//  !! Ağaçlarda derinlik == en kısa yol (döngü olmadığı için).
//
//  Karmaşıklık: O(V + E)
// ─────────────────────────────────────────────
void dfs(int u, int depth) {
    visited[u]  = true;
    dfsDist[u]  = depth;

    for (int v : adjList[u]) {
        if (!visited[v]) {
            dfs(v, depth + 1); // Bir katman daha derine in
        }
    }
}

// ─────────────────────────────────────────────
//  3) Bağlı Bileşen Sayısı
//
//  NEDEN KULLANILIR?
//  "Grafta kaç ayrı ada var?" sorusunu cevaplar.
//  Her ziyaret edilmemiş düğümden yeni BFS başlat.
//  Her başlangıç = yeni bir bileşen.
//
//  Örnek: Sosyal ağda birbirinden kopuk gruplar kaç tane?
// ─────────────────────────────────────────────
int countComponents() {
    fill(visited + 1, visited + V + 1, false);
    fill(componentId + 1, componentId + V + 1, -1);

    int comp = 0;
    for (int i = 1; i <= V; i++) {
        if (!visited[i]) {
            // Bu düğümden BFS başlat — hepsini aynı bileşene ata
            queue<int> q;
            q.push(i);
            visited[i]     = true;
            componentId[i] = comp;

            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : adjList[u]) {
                    if (!visited[v]) {
                        visited[v]     = true;
                        componentId[v] = comp;
                        q.push(v);
                    }
                }
            }
            comp++;
        }
    }
    return comp; // Toplam bileşen sayısı
}

// ─────────────────────────────────────────────
//  4) Döngü Tespiti — Yönsüz Graf (DFS ile)
//
//  NEDEN KULLANILIR?
//  "Bu grafikte döngü var mı?" — MST'de, DAG kontrolünde lazım.
//
//  Mantık: DFS sırasında ziyaret edilmiş ama parent'ımız olmayan
//  bir komşuya tekrar ulaşırsak → geri kenar (back-edge) → döngü!
// ─────────────────────────────────────────────
bool hasCycleDFS(int u, int parent) {
    visited[u] = true;

    for (int v : adjList[u]) {
        if (!visited[v]) {
            if (hasCycleDFS(v, u)) return true;
        } else if (v != parent) {
            // Ziyaret edilmiş + parent değil = döngü!
            return true;
        }
    }
    return false;
}

bool detectCycle() {
    fill(visited + 1, visited + V + 1, false);
    for (int i = 1; i <= V; i++) {
        if (!visited[i]) {
            if (hasCycleDFS(i, -1)) return true;
        }
    }
    return false;
}

// ─────────────────────────────────────────────
//  5) Bipartite Kontrol (BFS ile 2-Renklendirme)
//
//  NEDEN KULLANILIR?
//  "Bu graf iki renge boyanabilir mi?"
//  → İş-işçi eşleştirme, çift-tek renk oyunları, vs.
//
//  Mantık: BFS'te her komşuya ters renk ver.
//  Aynı renkte iki komşu bulursan → bipartite değil.
// ─────────────────────────────────────────────
bool checkBipartite() {
    fill(color + 1, color + V + 1, 0);
    bool isBipartite = true;

    for (int start = 1; start <= V; start++) {
        if (color[start] != 0) continue; // Zaten renklendirilmiş

        queue<int> q;
        color[start] = 1;
        q.push(start);

        while (!q.empty() && isBipartite) {
            int u = q.front(); q.pop();

            for (int v : adjList[u]) {
                if (color[v] == 0) {
                    color[v] = -color[u]; // Ters renk
                    q.push(v);
                } else if (color[v] == color[u]) {
                    isBipartite = false; // Çakışma! Aynı renk komşu
                    break;
                }
            }
        }
    }
    return isBipartite;
}

// ─────────────────────────────────────────────
//  MAIN — Tüm fonksiyonları test et
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Test Grafı 1: Bağlı, döngüsüz gibi görünen ──
    //   1 - 2 - 4
    //   |
    //   3 - 5
    V = 5;
    addEdge(1, 2);
    addEdge(1, 3);
    addEdge(2, 4);
    addEdge(3, 5);

    cout << "=== BFS Mesafeleri (başlangıç: 1) ===" << endl;
    bfs(1);
    for (int i = 1; i <= V; i++) {
        cout << "  dist[" << i << "] = " << dist[i] << endl;
    }

    cout << "\n=== BFS En Kısa Yol: 1 → 5 ===" << endl;
    vector<int> path = bfsShortestPath(1, 5);
    for (int i = 0; i < (int)path.size(); i++) {
        cout << path[i];
        if (i + 1 < (int)path.size()) cout << " → ";
    }
    cout << "  (uzunluk=" << dist[5] << ")" << endl;

    cout << "\n=== DFS Derinlikleri (başlangıç: 1) ===" << endl;
    fill(visited + 1, visited + V + 1, false);
    dfs(1, 0);
    for (int i = 1; i <= V; i++) {
        cout << "  dfsDist[" << i << "] = " << dfsDist[i] << endl;
    }

    cout << "\n=== Bağlı Bileşen Sayısı ===" << endl;
    cout << "  " << countComponents() << " bileşen" << endl;

    cout << "\n=== Döngü Var mı? ===" << endl;
    cout << "  " << (detectCycle() ? "EVET - döngü var" : "HAYIR - döngü yok") << endl;

    cout << "\n=== Bipartite mi? ===" << endl;
    cout << "  " << (checkBipartite() ? "EVET - bipartite" : "HAYIR - bipartite değil") << endl;

    // ── Test Grafı 2: Döngülü ──
    cout << "\n\n=== YENİ GRAF (döngülü): 1-2-3-1 ===" << endl;
    // Önceki adjList'i temizle
    for (int i = 1; i <= V; i++) adjList[i].clear();
    V = 3;
    addEdge(1, 2);
    addEdge(2, 3);
    addEdge(3, 1); // Döngü burada!

    cout << "=== BFS Mesafeleri (başlangıç: 1) ===" << endl;
    bfs(1);
    for (int i = 1; i <= V; i++)
        cout << "  dist[" << i << "] = " << dist[i] << endl;

    cout << "\n=== Döngü Var mı? ===" << endl;
    cout << "  " << (detectCycle() ? "EVET - döngü var" : "HAYIR - döngü yok") << endl;

    cout << "\n=== Bipartite mi? (Tek döngü - bipartite değil) ===" << endl;
    cout << "  " << (checkBipartite() ? "EVET" : "HAYIR - bipartite değil") << endl;

    return 0;
}

/*
 * ÖZET:
 *
 * BFS → Ağırlıksız en kısa mesafe, bipartite kontrol
 * DFS → Döngü tespiti, bileşen sayma, ağaç algoritmaları
 *
 * Mesafe formülleri:
 *   BFS: dist[v] = dist[u] + 1   (kuyruktan çıkarken)
 *   DFS: dfsDist[v] = depth + 1  (her rekürsif çağrıda)
 *
 * Ağırlıklı en kısa yol için → Dijkstra (bkz. 13_shortest_paths.cpp)
 */
