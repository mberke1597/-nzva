// ============================================================
//  16 - Yönlü Graflar (Directed Graphs / DAG)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 16
//
//  ✅ struct/class YOK → global adjList, state, succ dizileri
//  ✅ Topolojik sıralama (DFS post-order + Kahn's indegree)
//  ✅ Yönlü döngü tespiti
//  ✅ Binary Lifting (Successor / Functional Graph)
//  ✅ DAG üzerinde DP (en uzun yol)
// ============================================================

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
// ─────────────────────────────────────────────
const int MAXN    = 100005;
const int LOG     = 20;    // Binary lifting için log2(max_k)
const long long INF = 1e18;

// Yönlü adjacency list
vector<int> adjList[MAXN];

// DFS state: 0=unvisited, 1=visiting (stack'te), 2=done
int  state[MAXN];

// Topolojik sıralama sonucu
vector<int> topoOrder;

// Kahn's algoritması için giriş derecesi
int  indegree[MAXN];

// Binary Lifting tablosu: succ[i][x] = x'ten 2^i adım sonraki düğüm
// succ[0][x] = x'in tek ardılı (functional graph)
int  succ[LOG][MAXN];

// DAG DP için en uzun yol mesafesi
long long longestPath[MAXN];

int V; // Düğüm sayısı

// ─────────────────────────────────────────────
//  YARDIMCI
// ─────────────────────────────────────────────
void addEdge(int u, int v) {
    adjList[u].push_back(v);
    indegree[v]++; // Kahn's için
}

// ─────────────────────────────────────────────
//  1) Topolojik Sıralama — DFS (Post-Order)
//
//  NEDEN KULLANILIR?
//  Görev sıralaması: "A bitmeden B başlamaz" ilişkisini sırala.
//  Derleme bağımlılıkları, ders planı, build sistemleri.
//
//  Mantık:
//  DFS'te bir düğümün tüm komşuları işlendikten sonra onu listeye ekle.
//  Sonunda listeyi ters çevir → topolojik sıra.
//
//  State 1 (visiting) → 1'e tekrar ulaşırsak → DÖNGÜ VAR!
//
//  Karmaşıklık: O(V + E)
// ─────────────────────────────────────────────
bool hasCycle = false;

void dfsTopo(int u) {
    state[u] = 1; // Şu an işleniyor (stack'te)

    for (int v : adjList[u]) {
        if (state[v] == 1) {
            hasCycle = true; // Geri kenar = döngü!
            return;
        }
        if (state[v] == 0) {
            dfsTopo(v);
            if (hasCycle) return;
        }
    }

    state[u] = 2; // Tamamen bitti
    topoOrder.push_back(u); // Post-order: sonuna ekle
}

// Tüm bileşenlerden başlat
vector<int> topologicalSortDFS() {
    fill(state + 1, state + V + 1, 0);
    topoOrder.clear();
    hasCycle = false;

    for (int i = 1; i <= V; i++) {
        if (state[i] == 0) {
            dfsTopo(i);
            if (hasCycle) return {}; // Döngü varsa boş döndür
        }
    }

    reverse(topoOrder.begin(), topoOrder.end()); // Post-order → topo sıra
    return topoOrder;
}

// ─────────────────────────────────────────────
//  2) Topolojik Sıralama — Kahn's Algoritması (BFS tabanlı)
//
//  NEDEN KAHN'S?
//  Döngü yoksa BFS ile de topolojik sıra bulunur.
//  "Kaç farklı topolojik sıra var?" sayımında da kullanışlı.
//  Indegree 0 olan düğümlerden başla, kuyruğa at.
//
//  Karmaşıklık: O(V + E)
// ─────────────────────────────────────────────
vector<int> topologicalSortKahn() {
    // indegree[] addEdge'de güncellendi
    queue<int> q;
    vector<int> order;

    for (int i = 1; i <= V; i++) {
        if (indegree[i] == 0) q.push(i);
    }

    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);

        for (int v : adjList[u]) {
            indegree[v]--;
            if (indegree[v] == 0) q.push(v);
        }
    }

    // Tüm düğümler işlenmediyse → döngü var
    if ((int)order.size() != V) return {};
    return order;
}

// ─────────────────────────────────────────────
//  3) DAG Üzerinde En Uzun Yol (DP)
//
//  NEDEN KULLANILIR?
//  DAG'da döngü yoktur → DP güvenle uygulanabilir.
//  "Kaynaktan hedefe en fazla kaç adım?" — kritik yol analizi,
//  proje yönetiminde bağımlılık zincirinin maksimum süresi.
//
//  Mantık:
//  Topolojik sıraya göre düğümleri işle:
//  longestPath[v] = max(longestPath[u] + 1) için tüm u→v kenarları
// ─────────────────────────────────────────────
void calcLongestPath(const vector<int>& topo) {
    fill(longestPath + 1, longestPath + V + 1, 0);

    for (int u : topo) {
        for (int v : adjList[u]) {
            longestPath[v] = max(longestPath[v], longestPath[u] + 1);
        }
    }
}

// ─────────────────────────────────────────────
//  4) Binary Lifting — Successor Paths
//
//  NEDEN KULLANILIR?
//  Her düğümün tam olarak 1 ardılı olan "fonksiyonel graf"ta:
//  "x'ten k adım gidince nereye varırım?" sorusunu O(log k) ile cevapla.
//  Örnek: Oyun simülasyonu, zincir takibi, LCA hesabı.
//
//  Ön hazırlık: succ[i][x] = x'ten 2^i adım sonraki düğüm
//  succ[0][x] = next[x]   (doğrudan ardıl)
//  succ[i][x] = succ[i-1][ succ[i-1][x] ]
//
//  Karmaşıklık: O(N log K) hazırlık, O(log K) sorgu
// ─────────────────────────────────────────────
// next_node[x] = x'in tek ardılı (her düğümden tam 1 kenar çıkar)
void buildBinaryLifting(const vector<int>& next_node) {
    int n = (int)next_node.size();

    // 2^0 = 1 adım: doğrudan ardıl
    for (int x = 0; x < n; x++)
        succ[0][x] = next_node[x];

    // 2^i adım: x'ten 2^(i-1) git, varılan yerden 2^(i-1) git
    for (int i = 1; i < LOG; i++) {
        for (int x = 0; x < n; x++) {
            succ[i][x] = succ[i-1][ succ[i-1][x] ];
        }
    }
}

// x'ten k adım gidince varılan düğüm
int getKthSuccessor(int x, long long k) {
    for (int i = 0; i < LOG; i++) {
        if (k & (1LL << i)) {  // k'nın i. biti 1 mi?
            x = succ[i][x];
        }
    }
    return x;
}

// ─────────────────────────────────────────────
//  MAIN — Test
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── DAG Testi:
    //  5 → 2 → 3 → 1
    //      ↘   ↗
    //       4 →
    //  0 → 1
    V = 6;
    addEdge(5, 2); addEdge(5, 0);
    addEdge(4, 0); addEdge(4, 1);
    addEdge(2, 3); addEdge(3, 1);

    // ── Topolojik Sıralama (DFS) ──
    cout << "=== Topolojik Sıralama (DFS) ===" << endl;
    vector<int> topo = topologicalSortDFS();
    if (topo.empty()) {
        cout << "  DÖNGÜ VAR — topolojik sıralama yapılamaz!" << endl;
    } else {
        cout << "  Sıra: ";
        for (int u : topo) cout << u << " ";
        cout << endl;
    }

    // ── En Uzun Yol (DAG DP) ──
    if (!topo.empty()) {
        cout << "\n=== DAG En Uzun Yol ===" << endl;
        calcLongestPath(topo);
        for (int i = 0; i < V; i++)
            cout << "  longestPath[" << i << "] = " << longestPath[i] << endl;
    }

    // ── Binary Lifting Testi ──
    // Fonksiyonel graf: 0→1→2→3→4→0 (döngü), 5→3
    cout << "\n=== Binary Lifting ===" << endl;
    // next_node[x] = x'in tek ardılı
    vector<int> nextNode = {1, 2, 3, 4, 0, 3}; // 0→1→2→3→4→0, 5→3
    buildBinaryLifting(nextNode);

    cout << "  0'dan 3 adım: " << getKthSuccessor(0, 3) << "  (beklenen: 3)" << endl;
    cout << "  0'dan 5 adım: " << getKthSuccessor(0, 5) << "  (beklenen: 0)" << endl;
    cout << "  5'ten 2 adım: " << getKthSuccessor(5, 2) << "  (beklenen: 0)" << endl;

    // ── Döngülü Graf Testi ──
    cout << "\n=== Döngülü Graf Testi ===" << endl;
    // adjList'i sıfırla
    for (int i = 0; i <= V; i++) { adjList[i].clear(); indegree[i] = 0; }
    V = 3;
    addEdge(1, 2); addEdge(2, 3); addEdge(3, 1); // 1→2→3→1 döngüsü

    vector<int> cycleTopo = topologicalSortDFS();
    cout << "  " << (cycleTopo.empty() ? "DÖNGÜ TESPİT EDİLDİ!" : "Döngü yok") << endl;

    return 0;
}

/*
 * ÖZET:
 *
 * DAG = Directed Acyclic Graph (Yönlü Döngüsüz Graf)
 *
 * Topolojik Sıralama → Görev sıralaması, bağımlılık çözümü
 * DFS post-order     → Basit implementasyon
 * Kahn's BFS         → İndegree tabanlı, döngü tespiti kolay
 *
 * Binary Lifting (Successor):
 *   succ[i][x] = succ[i-1][succ[i-1][x]]  (katlayarak hesapla)
 *   getKthSuccessor(x, k): k'yı binary'de aç, adım at
 *
 * DAG DP:
 *   Topolojik sırada işle → garantili önceki durumlar hazır
 */
