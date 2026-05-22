// ============================================================
//  14 - Ağaç Algoritmaları (Tree Algorithms)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 14
//
//  ✅ struct/class YOK → global adjList kullanılır
//  ✅ Alt ağaç boyutu (subtree size)
//  ✅ Çap (diameter) — iki DFS yöntemi
//  ✅ Ağaç DP — her düğümden en uzak yaprak
//  ✅ İkili ağaç gezinme (Preorder, Inorder, Postorder)
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
// ─────────────────────────────────────────────
const int MAXN = 100005;

// Ağırlıksız adjacency list (ağaçlar genellikle ağırlıksız)
vector<int> adjList[MAXN];

// Ağırlıklı adjacency list (ağırlıklı ağaç çapı için)
vector<pair<int,int>> adjListW[MAXN]; // {v, w}

int subtreeSize[MAXN]; // Alt ağaç büyüklüğü
int toLeaf[MAXN];      // Bu düğümden en uzak yaprak mesafesi
int depth[MAXN];       // Kökten derinlik

int V; // Düğüm sayısı

// ─────────────────────────────────────────────
//  YARDIMCI: Kenar Ekle
// ─────────────────────────────────────────────
void addEdge(int u, int v) {
    adjList[u].push_back(v);
    adjList[v].push_back(u);
}

void addEdgeWeighted(int u, int v, int w) {
    adjListW[u].push_back({v, w});
    adjListW[v].push_back({u, w});
}

// ─────────────────────────────────────────────
//  1) Alt Ağaç Boyutu (Subtree Size)
//
//  NEDEN KULLANILIR?
//  "Bu düğümün altında kaç düğüm var?" sorusunu cevaplar.
//  Ağırlıklı merkeziyet, ağaç bölme (centroid decomposition) için temel.
//
//  Mantık: Yapraklarda subtreeSize=1, yukarı doğru toplanır.
// ─────────────────────────────────────────────
void calcSubtreeSize(int u, int parent) {
    subtreeSize[u] = 1; // Kendisi
    for (int v : adjList[u]) {
        if (v == parent) continue; // Parent'a geri gitme
        calcSubtreeSize(v, u);
        subtreeSize[u] += subtreeSize[v];
    }
}

// ─────────────────────────────────────────────
//  2) Ağaç Çapı — Yöntem 1: İki DFS
//
//  NEDEN KULLANILIR?
//  "Bu ağaçtaki en uzak iki düğüm arası mesafe nedir?"
//  Kablosuz ağ tasarımı, sosyal ağ analizi, oyun haritaları.
//
//  Mantık:
//  1. Herhangi bir u'dan DFS → en uzak düğüm x'i bul
//  2. x'ten DFS → en uzak düğüm y'yi bul
//  3. x ile y arasındaki mesafe = çap
//
//  !! Sadece pozitif ağırlıklarda çalışır.
// ─────────────────────────────────────────────

// {en_uzak_mesafe, o_düğüm} döner
pair<int,int> findFarthest(int u, int parent, int dist) {
    pair<int,int> best = {dist, u};
    for (int v : adjList[u]) {
        if (v != parent) {
            best = max(best, findFarthest(v, u, dist + 1));
        }
    }
    return best;
}

int getDiameterTwoDFS() {
    // 1. Adım: 0'dan en uzak düğümü bul
    auto [d1, farthestNode] = findFarthest(1, -1, 0);
    // 2. Adım: O düğümden en uzak düğümü bul
    auto [diameter, end] = findFarthest(farthestNode, -1, 0);
    return diameter;
}

// ─────────────────────────────────────────────
//  3) Ağaç Çapı — Yöntem 2: DP (Her düğüm için)
//
//  NEDEN KULLANILIR?
//  Her düğümden gidilebilen en uzun yolu hesaplar.
//  İki DFS yönteminden daha genel: ağırlıklı ağaçlarda da çalışır.
//
//  Mantık:
//  Her düğüm u için, alt ağacından aşağıya giden EN UZUN iki yolu bul.
//  Bu iki yolun toplamı, u'dan geçen en uzun yoldur.
//  Bunların maksimumu = ağaç çapı.
// ─────────────────────────────────────────────
int diameterDP = 0; // DP sırasında güncellenir

void calcDiameterDP(int u, int parent) {
    toLeaf[u] = 0;

    // En uzun iki alt yolu bul
    int longest1 = 0, longest2 = 0;

    for (int v : adjList[u]) {
        if (v == parent) continue;
        calcDiameterDP(v, u);

        int childDist = toLeaf[v] + 1;
        if (childDist >= longest1) {
            longest2 = longest1;
            longest1 = childDist;
        } else if (childDist > longest2) {
            longest2 = childDist;
        }
    }

    toLeaf[u] = longest1; // Bu düğümden en uzak yaprak
    // u üzerinden geçen en uzun yol
    diameterDP = max(diameterDP, longest1 + longest2);
}

// ─────────────────────────────────────────────
//  4) Ağaç DFS — Derinlik ve Parent Takibi
//
//  NEDEN KULLANILIR?
//  Temel ağaç gezinmesi: derinlik, parent bilgisi, LCA hazırlığı
//
//  Ağaçlarda visited[] dizisi gerekmez!
//  Sadece parent parametresini geçirmek yeterli.
// ─────────────────────────────────────────────
void treeDFS(int u, int parent, int d) {
    depth[u] = d;
    for (int v : adjList[u]) {
        if (v != parent) {
            treeDFS(v, u, d + 1);
        }
    }
}

// ─────────────────────────────────────────────
//  5) İkili Ağaç Gezinme
//
//  NEDEN KULLANILIR?
//  BST işlemleri, expression tree, JSON/XML parsing.
//
//  left[u] = u'nun sol çocuğu (-1 yoksa)
//  right[u] = u'nun sağ çocuğu (-1 yoksa)
// ─────────────────────────────────────────────
int leftChild[MAXN], rightChild[MAXN];

// Preorder: Kök → Sol → Sağ
void preorder(int u) {
    if (u == -1) return;
    cout << u << " ";
    preorder(leftChild[u]);
    preorder(rightChild[u]);
}

// Inorder: Sol → Kök → Sağ  (BST'de sıralı çıktı!)
void inorder(int u) {
    if (u == -1) return;
    inorder(leftChild[u]);
    cout << u << " ";
    inorder(rightChild[u]);
}

// Postorder: Sol → Sağ → Kök
void postorder(int u) {
    if (u == -1) return;
    postorder(leftChild[u]);
    postorder(rightChild[u]);
    cout << u << " ";
}

// ─────────────────────────────────────────────
//  MAIN — Test
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Ağaç:
    //       1
    //      / \
    //     2   3
    //    / \   \
    //   4   5   6
    //  /
    // 7
    V = 7;
    addEdge(1, 2); addEdge(1, 3);
    addEdge(2, 4); addEdge(2, 5);
    addEdge(3, 6);
    addEdge(4, 7);

    // Alt ağaç boyutları
    cout << "=== Alt Ağaç Boyutları ===" << endl;
    calcSubtreeSize(1, -1);
    for (int i = 1; i <= V; i++)
        cout << "  subtreeSize[" << i << "] = " << subtreeSize[i] << endl;

    // Ağaç çapı (iki DFS)
    cout << "\n=== Ağaç Çapı (İki DFS) ===" << endl;
    cout << "  Çap = " << getDiameterTwoDFS() << endl;

    // Ağaç çapı (DP)
    cout << "\n=== Ağaç Çapı (DP) ===" << endl;
    diameterDP = 0;
    calcDiameterDP(1, -1);
    cout << "  Çap = " << diameterDP << endl;

    // Derinlikler
    cout << "\n=== Derinlikler (kök=1) ===" << endl;
    treeDFS(1, -1, 0);
    for (int i = 1; i <= V; i++)
        cout << "  depth[" << i << "] = " << depth[i] << endl;

    // İkili ağaç gezinme
    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5
    cout << "\n=== İkili Ağaç Gezinme ===" << endl;
    leftChild[1] = 2; rightChild[1] = 3;
    leftChild[2] = 4; rightChild[2] = 5;
    leftChild[3] = -1; rightChild[3] = -1;
    leftChild[4] = -1; rightChild[4] = -1;
    leftChild[5] = -1; rightChild[5] = -1;

    cout << "  Preorder  (K-L-R): "; preorder(1);  cout << endl;
    cout << "  Inorder   (L-K-R): "; inorder(1);   cout << endl;
    cout << "  Postorder (L-R-K): "; postorder(1); cout << endl;

    return 0;
}

/*
 * ÖZET:
 *
 * Ağaçlarda visited[] GEREKMEZ → parent parametresi yeterli
 *
 * Çap bulma:
 *   İki DFS → Basit, hızlı, pozitif ağırlıklı ağaçlar için
 *   DP       → Daha genel, tüm düğümler için "en uzak yaprak" da hesaplanır
 *
 * subtreeSize[u] = 1 + Σ subtreeSize[child]
 * toLeaf[u] = max(toLeaf[child] + 1) için tüm çocuklar
 */
