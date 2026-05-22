// ============================================================
//  18 - Ağaç Sorguları (Tree Queries)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 18
//
//  ✅ struct/class YOK → global adjList, ancestor, depth dizileri
//  ✅ Binary Lifting ile k. ata bulma
//  ✅ Euler Tour (flat array dönüşümü)
//  ✅ LCA (Lowest Common Ancestor) — en yakın ortak ata
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
// ─────────────────────────────────────────────
const int MAXN = 100005;
const int LOG  = 17; // log2(10^5) ≈ 17

// Yönsüz ağaç
vector<int> adjList[MAXN];

// Binary Lifting tablosu: ancestor[u][i] = u'nun 2^i üst atası
// ancestor[u][0] = u'nun parent'ı
int ancestor[MAXN][LOG];

int depth[MAXN];      // Kökten derinlik
int startTime[MAXN];  // Euler tour giriş zamanı
int endTime[MAXN];    // Euler tour çıkış zamanı
int timer_val = 0;

int V; // Düğüm sayısı

// ─────────────────────────────────────────────
//  YARDIMCI
// ─────────────────────────────────────────────
void addEdge(int u, int v) {
    adjList[u].push_back(v);
    adjList[v].push_back(u);
}

// ─────────────────────────────────────────────
//  1) Binary Lifting — Ağacı Hazırla
//
//  NEDEN KULLANILIR?
//  "x düğümünün k. atası kim?" sorusunu O(log k)'da cevapla.
//  LCA (ortak ata) hesabının temelini oluşturur.
//  Ağaçlarda hızlı atlama (jumping) sorguları için.
//
//  ancestor[u][i]:
//    i=0 → u'nun parent'ı
//    i=1 → u'nun 2. atası (parent'ının parent'ı)
//    i=2 → u'nun 4. atası
//    ...
//    i   → u'nun 2^i. atası
//
//  Formül:
//  ancestor[u][i] = ancestor[ ancestor[u][i-1] ][i-1]
//  "2^i adım = 2^(i-1) adım + 2^(i-1) adım"
// ─────────────────────────────────────────────
void dfsPrecompute(int u, int parent, int d) {
    depth[u]       = d;
    ancestor[u][0] = parent; // Direkt ata: parent

    // Tüm seviyeleri doldur
    for (int i = 1; i < LOG; i++) {
        if (ancestor[u][i-1] != -1)
            ancestor[u][i] = ancestor[ ancestor[u][i-1] ][i-1];
        else
            ancestor[u][i] = -1; // Kök üstü yok
    }

    startTime[u] = ++timer_val; // Euler tour girişi

    for (int v : adjList[u]) {
        if (v != parent) {
            dfsPrecompute(v, u, d + 1);
        }
    }

    endTime[u] = timer_val; // Euler tour çıkışı
}

// ─────────────────────────────────────────────
//  2) k. Atayı Bul (Binary Lifting sorgusu)
//
//  k'yı ikili sistemde yaz:
//  k = 2^a + 2^b + ...  → Her biti için o adımı at
// ─────────────────────────────────────────────
int getKthAncestor(int u, int k) {
    for (int i = 0; i < LOG; i++) {
        if (k & (1 << i)) {  // k'nın i. biti 1 mi?
            u = ancestor[u][i];
            if (u == -1) return -1; // Kök aşıldı
        }
    }
    return u;
}

// ─────────────────────────────────────────────
//  3) LCA — Lowest Common Ancestor
//
//  NEDEN KULLANILIR?
//  "u ile v'nin ağaçtaki en yakın ortak atası kim?"
//  u'dan v'ye yol mesafesi: depth[u] + depth[v] - 2*depth[LCA(u,v)]
//
//  Kullanım: Ağaç üzerinde yol sorguları, RMQ, ağırlıklı yol hesabı.
//
//  Algoritma:
//  1. Derin olan düğümü diğerinin seviyesine getir (binary lifting)
//  2. İkisi eşit derinlikte ama farklı düğümse:
//     Binary lifting ile ebeveynleri farklı olacak şekilde birlikte yukarı çık
//  3. Her iki düğümün parent'ı = LCA
//
//  Karmaşıklık: O(N log N) hazırlık, O(log N) sorgu
// ─────────────────────────────────────────────
int getLCA(int u, int v) {
    // Derinlikleri eşitle: derin olan düğümü yukarı çek
    if (depth[u] < depth[v]) swap(u, v);

    int diff = depth[u] - depth[v];
    u = getKthAncestor(u, diff); // u'yu v'nin seviyesine getir

    // Aynı düğümdeler → LCA bu
    if (u == v) return u;

    // İkisi birden en büyük adımdan başlayarak yukarı çık
    // (ebeveynleri farklı olduğu sürece)
    for (int i = LOG - 1; i >= 0; i--) {
        if (ancestor[u][i] != ancestor[v][i]) {
            u = ancestor[u][i];
            v = ancestor[v][i];
        }
    }

    // u ve v'nin parent'ları artık ortak → o parent LCA
    return ancestor[u][0];
}

// ─────────────────────────────────────────────
//  4) Euler Tour — Alt Ağaç Dönüşümü
//
//  NEDEN KULLANILIR?
//  Ağacı düz bir dizi (array) haline getir.
//  u'nun alt ağacındaki tüm düğümler [startTime[u], endTime[u]] aralığında!
//  Bu sayede "u'nun alt ağacındaki tüm değerlerin toplamı/maks/min"
//  gibi sorgular → Segment Tree veya Fenwick Tree ile O(log N) olur.
//
//  Örnek: subtree sum, subtree update gibi sorgular.
// ─────────────────────────────────────────────
bool isAncestor(int u, int v) {
    // u, v'nin atası mı? (v, u'nun alt ağacında mı?)
    return startTime[u] <= startTime[v] && endTime[v] <= endTime[u];
}

// u'dan v'ye ağaç yol mesafesi
int treeDist(int u, int v) {
    int lca = getLCA(u, v);
    return depth[u] + depth[v] - 2 * depth[lca];
}

// ─────────────────────────────────────────────
//  MAIN — Test
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Ağaç:
    //           1
    //          / \
    //         2   3
    //        / \   \
    //       4   5   6
    //      /
    //     7
    V = 7;
    addEdge(1, 2); addEdge(1, 3);
    addEdge(2, 4); addEdge(2, 5);
    addEdge(3, 6);
    addEdge(4, 7);

    // Binary Lifting hazırlığı
    dfsPrecompute(1, -1, 0);

    // ── Derinlikler ──
    cout << "=== Derinlikler ===" << endl;
    for (int i = 1; i <= V; i++)
        cout << "  depth[" << i << "] = " << depth[i] << endl;

    // ── k. Ata Sorguları ──
    cout << "\n=== k. Ata Sorguları ===" << endl;
    cout << "  7'nin 1. atası: " << getKthAncestor(7, 1) << "  (beklenen: 4)" << endl;
    cout << "  7'nin 2. atası: " << getKthAncestor(7, 2) << "  (beklenen: 2)" << endl;
    cout << "  7'nin 3. atası: " << getKthAncestor(7, 3) << "  (beklenen: 1)" << endl;

    // ── LCA Sorguları ──
    cout << "\n=== LCA (En Yakın Ortak Ata) ===" << endl;
    cout << "  LCA(4, 5) = " << getLCA(4, 5) << "  (beklenen: 2)" << endl;
    cout << "  LCA(7, 6) = " << getLCA(7, 6) << "  (beklenen: 1)" << endl;
    cout << "  LCA(7, 5) = " << getLCA(7, 5) << "  (beklenen: 2)" << endl;
    cout << "  LCA(6, 3) = " << getLCA(6, 3) << "  (beklenen: 3)" << endl;

    // ── Yol Mesafeleri ──
    cout << "\n=== Yol Mesafeleri (depth[u]+depth[v]-2*depth[LCA]) ===" << endl;
    cout << "  dist(7, 6) = " << treeDist(7, 6) << "  (beklenen: 5)" << endl;
    cout << "  dist(4, 5) = " << treeDist(4, 5) << "  (beklenen: 2)" << endl;
    cout << "  dist(7, 5) = " << treeDist(7, 5) << "  (beklenen: 3)" << endl;

    // ── Euler Tour Aralıkları ──
    cout << "\n=== Euler Tour [start, end] ===" << endl;
    for (int i = 1; i <= V; i++)
        cout << "  " << i << ": [" << startTime[i] << ", " << endTime[i] << "]" << endl;

    // ── Alt Ağaç Kontrolü ──
    cout << "\n=== Alt Ağaç Kontrolü (isAncestor) ===" << endl;
    cout << "  1, 7'nin atası mı? " << (isAncestor(1, 7) ? "EVET" : "HAYIR") << endl;
    cout << "  2, 6'nın atası mı? " << (isAncestor(2, 6) ? "EVET" : "HAYIR") << endl;
    cout << "  4, 7'nin atası mı? " << (isAncestor(4, 7) ? "EVET" : "HAYIR") << endl;

    return 0;
}

/*
 * ÖZET:
 *
 * Binary Lifting:
 *   ancestor[u][i] = ancestor[ancestor[u][i-1]][i-1]
 *   k. ata: k'yı binary'de aç, her 1 biti için atla
 *
 * LCA:
 *   1. Derin olanı diğerinin seviyesine getir
 *   2. Birlikte yukarı çık (ebeveynleri eşit olmadığı sürece)
 *   3. Parent = LCA
 *
 *   Yol mesafesi = depth[u] + depth[v] - 2 * depth[LCA(u,v)]
 *
 * Euler Tour:
 *   startTime[u] ≤ startTime[v] ≤ endTime[u]  →  v, u'nun alt ağacında
 */
