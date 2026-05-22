# Data Structures & Algorithms (DSA) – Graph Algorithms Reference

Bu kılavuz, **Competitive Programmer's Handbook** kitabının **II. Bölümü (Graph Algorithms - Konu 11 - 20)** içinde yer alan tüm konuların, algoritmaların, mülakat odaklı açıklamaların ve C++ kod çözümlerinin detaylı bir referansıdır.

---

## 11. Basics of Graphs (Graf Temelleri)

Graf teorisi, nesneler (düğümler/köşeler) ve aralarındaki ilişkileri (kenarlar) modellemek için kullanılan matematiksel yapılardır.

### 11.1 Graph Terminology (Graf Terimleri)
- **Vertex (Düğüm/Köşe - $V$):** Grafın temel elemanlarıdır.
- **Edge (Kenar - $E$):** İki düğüm arasındaki bağlantıdır. Yönlü (directed) veya yönsüz (undirected) olabilir.
- **Weight (Ağırlık):** Kenarlara atanan maliyet, uzunluk veya kapasite değeridir.
- **Degree (Derece):** Bir düğüme bağlı kenar sayısıdır. Yönlü graflarda **in-degree** (içeri giren kenar sayısı) ve **out-degree** (dışarı çıkan kenar sayısı) olarak ikiye ayrılır.
- **Path (Yol):** Bir düğümden diğerine kenarlar üzerinden gidilen düğümler dizisidir.
- **Cycle (Çevrim/Döngü):** Başladığı düğümde biten ve en az bir kenarı olan yoldur.
- **Connected Graph (Bağlantılı Graf):** Herhangi iki düğüm arasında en az bir yol bulunan yönsüz graftır.
- **Bipartite Graph (İki Kümeli Graf):** Düğümleri iki gruba ayrılabilen ve aynı gruptaki düğümler arasında kenar bulunmayan graftır (2-boyanabilir graf).

### 11.2 Graph Representation (Graf Temsili)
Grafik verilerini bellekte tutmanın üç temel yolu vardır:

1. **Adjacency List (Komşuluk Listesi):** Her düğüm için komşu olduğu düğümlerin listesini tutar. Seyrek (sparse) graflar için en verimli gösterim şeklidir. Uzay karmaşıklığı $O(V + E)$'dir.
2. **Adjacency Matrix (Komşuluk Matrisi):** $V \times V$ boyutunda bir 2D matristir. `matrix[u][v] = 1` (veya ağırlık değeri) bağlantıyı gösterir. Yoğun (dense) graflarda hızlıdır, ancak $O(V^2)$ bellek gerektirir.
3. **Edge List (Kenar Listesi):** Tüm kenarları çiftler halinde bir listede tutar. Özellikle kenarları sıralamamız gereken Kruskal veya Bellman-Ford gibi algoritmalar için idealdir.

#### C++ Graf Gösterim Yapısı:
```cpp
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

// Grafların C++ üzerinde temsili
struct GraphRepresentation {
    int V; // Düğüm sayısı
    
    // 1. Adjacency List (Komşuluk Listesi - Ağırlıklı)
    vector<vector<pair<int, int>>> adjList;
    
    // 2. Adjacency Matrix (Komşuluk Matrisi)
    vector<vector<int>> adjMatrix;
    
    // 3. Edge List (Kenar Listesi)
    vector<tuple<int, int, int>> edgeList; // {u, v, weight}

    GraphRepresentation(int n) : V(n) {
        adjList.resize(n);
        adjMatrix.assign(n, vector<int>(n, 0));
    }

    void addEdge(int u, int v, int w = 1, bool directed = false) {
        // Komşuluk listesi ekleme
        adjList[u].push_back({v, w});
        if (!directed) {
            adjList[v].push_back({u, w});
        }

        // Komşuluk matrisi ekleme
        adjMatrix[u][v] = w;
        if (!directed) {
            adjMatrix[v][u] = w;
        }

        // Kenar listesi ekleme
        edgeList.push_back({u, v, w});
    }
};
```

---

## 12. Graph Traversal (Graf Gezinme)

Graf gezinme algoritmaları, bir düğümden başlayarak graf üzerindeki tüm erişilebilir düğümleri sistematik bir şekilde ziyaret etmek için kullanılır.

### 12.1 Depth-First Search (Derinlemesine Arama - DFS)
DFS, başladığı düğümden gidebildiği kadar derine (yolun sonuna) kadar gider, çıkmaza girdiğinde geri adımlar atarak (backtracking) ziyaret edilmemiş diğer yolları arar. Yığın (Stack) mantığıyla çalışır ve genellikle rekürsif (özyinelemeli) olarak kodlanır. Zaman Karmaşıklığı: $O(V + E)$.

### 12.2 Breadth-First Search (Genişlemesine Arama - BFS)
BFS, başladığı düğümün önce tüm komşularını (1 birim uzaktakileri), sonra onların komşularını (2 birim uzaktakileri) gezer. Kuyruk (Queue) yapısını kullanır. En önemli özelliği, **ağırlıksız graflarda** kaynak düğümden diğer düğümlere olan en kısa yolu bulmasıdır. Zaman Karmaşıklığı: $O(V + E)$.

### 12.3 Applications (Uygulamalar)
- **Bağlantı Kontrolü (Connectivity):** Grafın kaç bileşenden oluştuğunu bulma.
- **Döngü Tespiti (Cycle Detection):** Yönsüz veya yönlü graflarda döngü olup olmadığını bulma.
- **İki Kümelilik Kontrolü (Bipartite Check):** Grafı 2 renge boyamaya çalışarak çakışma olup olmadığını inceleme.

#### C++ Çözümü:
```cpp
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct GraphTraversal {
    int V;
    vector<vector<int>> adj;
    vector<bool> visited;
    vector<int> dist;
    vector<int> color; // 0: boyanmamış, 1 veya -1 boyalı

    GraphTraversal(int n) : V(n), adj(n), visited(n, false), dist(n, -1), color(n, 0) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Rekürsif DFS
    void dfs(int s) {
        visited[s] = true;
        for (int u : adj[s]) {
            if (!visited[u]) {
                dfs(u);
            }
        }
    }

    // BFS (Ağırlıksız en kısa mesafeleri hesaplar)
    void bfs(int start) {
        queue<int> q;
        visited[start] = true;
        dist[start] = 0;
        q.push(start);
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
    }

    // Undirected Graph Döngü Tespiti (DFS ile)
    bool hasCycleDFS(int u, int parent) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) {
                if (hasCycleDFS(v, u)) return true;
            } else if (v != parent) {
                return true; // Ziyaret edilmiş ve parent olmayan komşu döngü olduğunu gösterir
            }
        }
        return false;
    }

    // Bipartite (İki Kümelilik) Kontrolü (BFS ile)
    bool checkBipartite(int start) {
        queue<int> q;
        color[start] = 1;
        q.push(start);
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (color[v] == 0) {
                    color[v] = -color[u]; // Ters renge boya
                    q.push(v);
                } else if (color[v] == color[u]) {
                    return false; // Aynı renkte komşu varsa bipartite değildir
                }
            }
        }
        return true;
    }
};
```

---

## 13. Shortest Paths (En Kısa Yollar)

Ağırlıklı graflarda iki düğüm arasındaki en kısa yolu (toplam kenar ağırlığı en az olan yol) bulmak için kullanılan algoritmalardır.

### 13.1 Bellman-Ford Algoritması
Her adımda tüm kenarları gevşeterek (relax) en kısa yolları günceller.
- **Özelliği:** Negatif kenar ağırlıklarıyla çalışabilir.
- **Negatif Çevrim Tespiti (Negative Cycle):** $V-1$ kez tüm kenarları gevşettikten sonra $V$. kez gevşetme yapıldığında hala bir düğümün mesafesi azalıyorsa grafikte negatif ağırlıklı bir döngü vardır.
- **Zaman Karmaşıklığı:** $O(V \cdot E)$.

### 13.2 Dijkstra Algoritması
Bir başlangıç noktasından diğer tüm noktalara giden en kısa yolu bulur (Single-source shortest path).
- **Özelliği:** Negatif kenar ağırlıkları olduğunda çalışmaz. Açgözlü (greedy) yaklaşım ve min-heap kullanır.
- **Zaman Karmaşıklığı:** $O(V + E \log E)$ veya $O(V + E \log V)$.

### 13.3 Floyd-Warshall Algoritması
Grafikteki her düğüm çifti arasındaki en kısa yolu bulur (All-pairs shortest path).
- **Özelliği:** Dynamic Programming yaklaşımıdır. Çok basit 3 içiçe döngüyle kurulur.
- **Zaman Karmaşıklığı:** $O(V^3)$.

#### C++ Çözümü:
```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>

using namespace std;

const long long INF_VAL = 1e18;

struct ShortestPaths {
    int V;
    vector<vector<pair<int, int>>> adj; // adj[u] = {v, w}
    vector<tuple<int, int, int>> edges; // {u, v, w}

    ShortestPaths(int n) : V(n), adj(n) {}

    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        edges.push_back({u, v, w});
    }

    // Bellman-Ford Algoritması (Negatif döngü varsa true döner)
    bool bellmanFord(int src, vector<long long>& dist) {
        dist.assign(V, INF_VAL);
        dist[src] = 0;
        
        for (int i = 1; i <= V - 1; ++i) {
            for (auto edge : edges) {
                int u, v, w;
                tie(u, v, w) = edge;
                if (dist[u] != INF_VAL && dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                }
            }
        }
        
        // Negatif Çevrim Tespiti
        for (auto edge : edges) {
            int u, v, w;
            tie(u, v, w) = edge;
            if (dist[u] != INF_VAL && dist[u] + w < dist[v]) {
                return true; // Negatif döngü var!
            }
        }
        return false;
    }

    // Dijkstra Algoritması (Min-heap ile optimize edilmiş)
    void dijkstra(int src, vector<long long>& dist) {
        dist.assign(V, INF_VAL);
        dist[src] = 0;
        
        // pair: {mesafe, düğüm}
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        pq.push({0, src});
        
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            
            if (d > dist[u]) continue;
            
            for (auto edge : adj[u]) {
                int v = edge.first;
                int w = edge.second;
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
    }

    // Floyd-Warshall Algoritması (Matris üzerinde in-place günceller)
    void floydWarshall(vector<vector<long long>>& distMatrix) {
        int n = distMatrix.size();
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (distMatrix[i][k] < INF_VAL && distMatrix[k][j] < INF_VAL) {
                        distMatrix[i][j] = min(distMatrix[i][j], distMatrix[i][k] + distMatrix[k][j]);
                    }
                }
            }
        }
    }
};
```

---

## 14. Tree Algorithms (Ağaç Algoritmaları)

Ağaçlar (trees), döngü içermeyen ve bağlantılı (connected) olan özel grafik sınıflarıdır. $N$ düğümlü bir ağaçta her zaman tam olarak $N-1$ kenar bulunur.

### 14.1 Tree Traversal (Ağaç Gezinme)
Ağaçlarda döngü olmadığı için DFS yaparken `visited` dizisi yerine sadece ana fonksiyona bir önceki ziyaret edilen düğümü parent (`p`) olarak göndermek yeterlidir.

### 14.2 Tree Diameter (Ağaç Çapı)
Bir ağaçtaki en uzak iki düğüm arasındaki mesafedir. Çapı bulmanın iki yolu vardır:
1. **İki DFS Yöntemi:** Herhangi bir $x$ düğümünden başlayarak DFS ile en uzak düğüm olan $y$ bulunur. Ardından $y$ düğümünden başlanarak yapılan ikinci bir DFS ile en uzak düğüm $z$ bulunur. $y$ ile $z$ arasındaki mesafe çapı verir. (Bu yöntem sadece pozitif kenar ağırlıklarında çalışır).
2. **Dynamic Programming (Ağaç DP):** Her düğüm için o düğümden aşağıya giden en uzun iki yaprak yolunu bulup toplayarak maksimum çapı günceller.

### 14.3 All Longest Paths (Tüm En Uzun Yollar)
Ağaçtaki her düğüm için, o düğümden başlayarak gidilebilecek en uzun yolun mesafesini bulmaktır. Çap uç noktaları kullanılarak veya Ağaç DP yöntemiyle çözülebilir.

### 14.4 Binary Trees (İkili Ağaçlar)
Her düğümün en fazla 2 çocuğunun olduğu özel ağaçlardır. Ziyaret sıralarına göre:
- **Preorder:** Kök $\rightarrow$ Sol $\rightarrow$ Sağ
- **Inorder:** Sol $\rightarrow$ Kök $\rightarrow$ Sağ (BST'lerde sıralı çıktı üretir)
- **Postorder:** Sol $\rightarrow$ Sağ $\rightarrow$ Kök

#### C++ Çözümü:
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct TreeAlgorithms {
    int V;
    vector<vector<int>> adj;
    vector<int> subtreeSize;
    vector<int> toLeaf;

    TreeAlgorithms(int n) : V(n), adj(n), subtreeSize(n, 0), toLeaf(n, 0) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Ağaç üzerinde basit DFS (Parent düğümünü geçirerek)
    void getSubtreeSize(int u, int p) {
        subtreeSize[u] = 1;
        for (int v : adj[u]) {
            if (v == p) continue; // Parent'a geri gitme
            getSubtreeSize(v, u);
            subtreeSize[u] += subtreeSize[v];
        }
    }

    // Ağaç Çapı için en uzak düğümü bulan DFS yardımcı fonksiyonu
    // {uzaklık, düğüm} çifti döner
    pair<int, int> findFarthestNode(int u, int p, int depth) {
        pair<int, int> farthest = {depth, u};
        for (int v : adj[u]) {
            if (v != p) {
                farthest = max(farthest, findFarthestNode(v, u, depth + 1));
            }
        }
        return farthest;
    }

    // Çift DFS ile Ağaç Çapı Bulma
    int getDiameter() {
        // 1. Adım: Herhangi bir düğümden (örn. 0) en uzak düğümü (u) bul
        auto p1 = findFarthestNode(0, -1, 0);
        int u = p1.second;
        // 2. Adım: u düğümünden en uzak düğümü (v) bul
        auto p2 = findFarthestNode(u, -1, 0);
        return p2.first; // u ile v arasındaki mesafe çaptır
    }

    // Ağaç DP ile En Derin Yaprak Mesafesini Hesaplama (toLeaf)
    void calculateToLeaf(int u, int p) {
        toLeaf[u] = 0;
        for (int v : adj[u]) {
            if (v == p) continue;
            calculateToLeaf(v, u);
            toLeaf[u] = max(toLeaf[u], toLeaf[v] + 1);
        }
    }
};
```

---

## 15. Spanning Trees (Kapsayan Ağaçlar)

Bir grafın tüm düğümlerini kapsayan ve döngü içermeyen alt grafına **Spanning Tree (Kapsayan Ağaç)** denir. Kenar ağırlıklarının toplamı en az olanına ise **Minimum Spanning Tree (MST)** denir.

### 15.1 Kruskal Algoritması
Grafın tüm kenarlarını ağırlıklarına göre küçükten büyüğe sıralar. Ardından her kenarı sırayla kontrol eder. Eğer eklenen kenar grafikte döngü oluşturmuyorsa, o kenarı MST'ye dahil eder. Döngü kontrolü için **Union-Find (DSU)** veri yapısı kullanılır. Zaman Karmaşıklığı: $O(E \log E)$.

### 15.2 Union-Find / DSU (Ayrık Küme Birleştirme)
Dinamik olarak kümeleri birleştirmek ve iki elemanın aynı kümede olup olmadığını kontrol etmek için kullanılır. İki ana optimizasyon içerir:
1. **Path Compression (Yol Sıkıştırma):** Her aramada düğümleri doğrudan köke bağlar ($O(1)$ amortize süre).
2. **Union by Size/Rank:** Küçük ağacı büyük ağacın altına bağlar.

### 15.3 Prim Algoritması
Açgözlü (greedy) bir yaklaşımdır. Başlangıç düğümünü seçip MST'ye ekler. Sonra MST'de bulunan düğümlerden henüz MST'de olmayan düğümlere giden en ucuz kenarı min-heap ile seçerek ilerler. Zaman Karmaşıklığı: $O(V + E \log V)$.

#### C++ Çözümü:
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <queue>
#include <numeric>

using namespace std;

// Ayrık Küme (Disjoint Set Union) Veri Yapısı
struct DisjointSet {
    vector<int> parent;
    vector<int> size;

    DisjointSet(int n) {
        parent.resize(n);
        iota(parent.begin(), parent.end(), 0); // parent[i] = i
        size.assign(n, 1);
    }

    int find(int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]); // Path compression
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        
        if (size[a] < size[b]) swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        return true;
    }
};

struct SpanningTrees {
    int V;
    vector<tuple<int, int, int>> edges; // {w, u, v}
    vector<vector<pair<int, int>>> adj; // adj[u] = {v, w}

    SpanningTrees(int n) : V(n), adj(n) {}

    void addEdge(int u, int v, int w) {
        edges.push_back({w, u, v});
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    // Kruskal ile MST Ağırlığını Hesaplama
    int kruskalMST() {
        int mst_weight = 0;
        sort(edges.begin(), edges.end()); // Kenarları ağırlığa göre sırala
        
        DisjointSet dsu(V);
        for (auto edge : edges) {
            int w, u, v;
            tie(w, u, v) = edge;
            if (dsu.unite(u, v)) {
                mst_weight += w;
            }
        }
        return mst_weight;
    }

    // Prim ile MST Ağırlığını Hesaplama
    int primMST(int src = 0) {
        int mst_weight = 0;
        vector<bool> inMST(V, false);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        
        pq.push({0, src}); // {weight, node}
        
        while (!pq.empty()) {
            auto [w, u] = pq.top(); pq.pop();
            if (inMST[u]) continue;
            
            inMST[u] = true;
            mst_weight += w;
            
            for (auto edge : adj[u]) {
                int v = edge.first;
                int weight = edge.second;
                if (!inMST[v]) {
                    pq.push({weight, v});
                }
            }
        }
        return mst_weight;
    }
};
```

---

## 16. Directed Graphs (Yönlü Graflar)

Kenarların tek yönlü olduğu grafik yapılarıdır. En önemli alt yapısı **DAG (Directed Acyclic Graph - Yönlü Çevrimsiz Graf)**'tır.

### 16.1 Topological Sorting (Topolojik Sıralama)
Yönlü kenarlara göre düğümleri doğrusal bir sıraya dizmektir. Eğer $u \rightarrow v$ kenarı varsa, sıralamada $u$ mutlaka $v$'den önce gelmelidir. Grafikte bir döngü varsa topolojik sıralama yapılamaz. DFS ile veya Kahn'ın Giriş Derecesi (indegree) kuyruk yöntemi ile çözülebilir.

### 16.2 Dynamic Programming (DAG Üzerinde DP)
DAG'larda çevrim (loop) olmadığı için durum geçişleri (transitions) güvenle yapılabilir. En kısa yollar, en uzun yollar veya $a$ noktasından $b$ noktasına giden farklı yolların sayısı DP ile hesaplanabilir.

### 16.3 Successor Paths (Ardıl Yollar ve Binary Lifting)
Her düğümün tam olarak bir tane dışarı çıkan kenarının olduğu graflara **Functional Graph** denir. $x$ düğümünden başlayıp $k$ adım attığımızda hangi düğüme varacağımızı sorgulamak için **Binary Lifting (İkili Atlama)** kullanılır. $O(N \log K)$ ön hazırlık ile her sorgu $O(\log K)$ sürede yanıtlanır.

### 16.4 Cycle Detection (Yönlü Graflarda Döngü Tespiti)
Düğümler DFS sırasında 3 duruma ayrılır:
- `0 (Unvisited):` Henüz ziyaret edilmemiş.
- `1 (Visiting):` DFS yığınında (recursion stack), yani o an işleniyor.
- `2 (Visited):` Tamamen işlenmiş ve çıkılmış.
Eğer bir DFS adımında `1` durumundaki bir düğüme tekrar ulaşılırsa grafikte döngü (back-edge) vardır.

#### C++ Çözümü:
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct DirectedGraphs {
    int V;
    vector<vector<int>> adj;
    vector<int> state; // 0: unvisited, 1: visiting, 2: visited
    vector<int> topoOrder;
    
    // Binary lifting ardıl tablosu
    vector<vector<int>> succ; 

    DirectedGraphs(int n) : V(n), adj(n), state(n, 0) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    // Topolojik Sıralama ve Döngü Tespiti (DFS)
    bool dfsTopo(int u) {
        state[u] = 1; // Ziyaret ediliyor
        for (int v : adj[u]) {
            if (state[v] == 1) return true; // Döngü bulundu!
            if (state[v] == 0) {
                if (dfsTopo(v)) return true;
            }
        }
        state[u] = 2; // İşlem bitti
        topoOrder.push_back(u);
        return false;
    }

    vector<int> getTopologicalSort() {
        topoOrder.clear();
        state.assign(V, 0);
        for (int i = 0; i < V; i++) {
            if (state[i] == 0) {
                if (dfsTopo(i)) return {}; // Döngü varsa boş liste dön
            }
        }
        reverse(topoOrder.begin(), topoOrder.end());
        return topoOrder;
    }

    // Successor Paths (Ardıllar için Binary Lifting tablosu hazırlama)
    // next_node[x] = x düğümünden sonraki doğrudan gidilen tek düğüm
    void buildSuccessorTable(const vector<int>& next_node, int max_steps = 30) {
        int n = next_node.size();
        succ.assign(max_steps, vector<int>(n, 0));
        
        for (int x = 0; x < n; x++) {
            succ[0][x] = next_node[x]; // 2^0 = 1 adım sonrası
        }
        
        for (int i = 1; i < max_steps; i++) {
            for (int x = 0; x < n; x++) {
                // x'ten 2^i adım gitmek = x'ten 2^(i-1) gidip, varılan yerden 2^(i-1) gitmektir
                succ[i][x] = succ[i - 1][succ[i - 1][x]];
            }
        }
    }

    int getSuccessor(int x, int k, int max_steps = 30) {
        for (int i = 0; i < max_steps; i++) {
            if (k & (1 << i)) {
                x = succ[i][x];
            }
        }
        return x;
    }
};
```

---

## 17. Strong Connectivity (Kuvvetli Bağlantı)

Yönlü bir grafikte, herhangi bir düğümden diğer her düğüme yollar bulunabiliyorsa bu graf **Strongly Connected (Kuvvetli Bağlantılı)**'dır. Bir grafın en büyük kuvvetli bağlantılı alt graflarına **SCC (Strongly Connected Components)** denir.

### 17.1 Kosaraju Algoritması
SCC'leri bulmak için iki kez DFS kullanan verimli bir algoritmadır:
1. Orijinal graf üzerinde DFS çalıştırılır ve düğümler işlem bitiş zamanlarına (post-order) göre bir listeye dizilir.
2. Grafın tüm kenarlarının yönü tersine çevrilir (transpose graph).
3. İlk adımda elde edilen listenin sonundan başlayarak (en geç biten düğümlerden) ters graf üzerinde DFS çalıştırılır. Her DFS aramasının ulaştığı düğümler kümesi birer SCC oluşturur.
- **Zaman Karmaşıklığı:** $O(V + E)$.

### 17.2 2-SAT Problemi
Mantıksal önermelerin $(x_1 \lor x_2) \land (\neg x_1 \lor x_3) \land ...$ biçimindeki formüllerinin doğru (true) yapılıp yapılamayacağı problemidir.
- **Graf Dönüşümü:** Her $(a \lor b)$ ifadesi iki adet implikasyona (çıkarım) dönüştürülür: $(\neg a \implies b)$ ve $(\neg b \implies a)$.
- Bu çıkarımlar düğümleri değişkenler ve onların değilleri olan yönlü bir graf haline getirilir.
- Grafikte SCC'ler bulunur. Eğer herhangi bir $x$ değişkeni ve onun değili $\neg x$ aynı SCC içerisine düşmüşse, $x \iff \neg x$ çelişkisi oluşur ve bu formül **sağlanamaz (unsatisfiable)**. Aksi halde bir doğruluk değeri ataması yapılabilir.

#### C++ Çözümü:
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct KosarajuSCC {
    int V;
    vector<vector<int>> adj;
    vector<vector<int>> adj_rev; // Ters yönlü graf
    vector<bool> visited;
    vector<int> order;
    vector<int> component; // Her düğümün ait olduğu SCC id'si

    KosarajuSCC(int n) : V(n), adj(n), adj_rev(n), visited(n, false), component(n, -1) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj_rev[v].push_back(u);
    }

    void dfs1(int u) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) dfs1(v);
        }
        order.push_back(u);
    }

    void dfs2(int u, int rep) {
        component[u] = rep;
        for (int v : adj_rev[u]) {
            if (component[v] == -1) dfs2(v, rep);
        }
    }

    int findSCCs() {
        visited.assign(V, false);
        order.clear();
        for (int i = 0; i < V; i++) {
            if (!visited[i]) dfs1(i);
        }
        
        int comp_id = 0;
        component.assign(V, -1);
        for (int i = V - 1; i >= 0; i--) {
            int u = order[i];
            if (component[u] == -1) {
                dfs2(u, comp_id++);
            }
        }
        return comp_id; // Toplam bileşen sayısı
    }
};
```

---

## 18. Tree Queries (Ağaç Sorguları)

Ağaç veri yapıları üzerinde hızlı aralık, ata veya yol sorgulamaları yapmak için kullanılan ileri seviye tekniklerdir.

### 18.1 Finding Ancestors (Ata Bulma ve Binary Lifting)
Bir ağaçta bir $x$ düğümünün $k$. atasını (yukarı doğru $k$ adım) bulmak için binary lifting kullanılır. `ancestor[x][i]`, $x$ düğümünün $2^i$ kadar yukarıdaki atasıdır:
$$ancestor[x][i] = ancestor[ancestor[x][i-1]][i-1]$$
Bu tablo sayesinde $k$. ata sorguları $O(\log K)$ sürede çözülür.

### 18.2 Subtrees and Paths (Alt Ağaçlar ve Yol Düzleştirme - Euler Tour)
Ağacı bir DFS gezinmesiyle düz bir diziye (array) dönüştürme işlemidir. DFS sırasında her düğüme giriş zamanı (`start[x]`) ve çıkış zamanı (`end[x]`) kaydedilir. Bu sayede $x$'in alt ağacındaki tüm düğümler dizide `[start[x], end[x]]` indis aralığına sıkışır. Alt ağaç sorguları ve güncellemeleri Fenwick Tree veya Segment Tree ile $O(\log N)$ sürede yapılabilir hale gelir.

### 18.3 Lowest Common Ancestor (En Yakın Ortak Ata - LCA)
Ağaçtaki iki $u$ ve $v$ düğümünün yukarı doğru ilk birleştikleri ortak ata düğümdür.
- **Çözüm:** Önce derinliği fazla olan düğüm binary lifting kullanılarak diğer düğümün derinlik seviyesine getirilir. Eğer eşitlendiklerinde aynı düğüm olurlarsa LCA odur. Değilse, her ikisi birden yukarı doğru en büyük adımdan başlayarak ($2^i$) ebeveynleri eşit olmayacak şekilde birlikte yukarı fırlatılır. En son ulaştıkları yerin bir üst ebeveyni LCA'dir.
- **Zaman Karmaşıklığı:** Ön hazırlık $O(N \log N)$, her sorgu $O(\log N)$.

#### C++ Çözümü:
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct TreeQueries {
    int V;
    vector<vector<int>> adj;
    vector<int> depth;
    vector<vector<int>> ancestor;
    
    // Euler Tour zamanları
    vector<int> start_time, end_time;
    int timer;

    TreeQueries(int n) : V(n), adj(n), depth(n, 0), start_time(n, 0), end_time(n, 0) {
        int logV = log2(n) + 2;
        ancestor.assign(n, vector<int>(logV, -1));
        timer = 0;
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Ağacı yapılandıran DFS sorgusu
    void dfsPrecompute(int u, int p, int d) {
        depth[u] = d;
        ancestor[u][0] = p;
        start_time[u] = ++timer;
        
        for (int i = 1; i < (int)ancestor[u].size(); i++) {
            if (ancestor[u][i - 1] != -1) {
                ancestor[u][i] = ancestor[ancestor[u][i - 1]][i - 1];
            } else {
                ancestor[u][i] = -1;
            }
        }
        
        for (int v : adj[u]) {
            if (v != p) {
                dfsPrecompute(v, u, d + 1);
            }
        }
        end_time[u] = timer; // Alt ağacı tamamlandı
    }

    // LCA Bulma Fonksiyonu
    int getLCA(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        
        int logV = ancestor[u].size();
        // 1. Derinlikleri eşitle
        for (int i = logV - 1; i >= 0; i--) {
            if (ancestor[u][i] != -1 && depth[ancestor[u][i]] >= depth[v]) {
                u = ancestor[u][i];
            }
        }
        
        if (u == v) return u;
        
        // 2. Birlikte yukarı çık
        for (int i = logV - 1; i >= 0; i--) {
            if (ancestor[u][i] != ancestor[v][i]) {
                u = ancestor[u][i];
                v = ancestor[v][i];
            }
        }
        return ancestor[u][0]; // Ebeveynleri ortaktır
    }
};
```

---

## 19. Paths and Circuits (Yollar ve Çevrimler)

Grafın tüm kenarlarını veya tüm düğümlerini tam olarak bir kez ziyaret etmeye dayalı klasik grafik problemleridir.

### 19.1 Eulerian Paths and Circuits (Euler Yolları ve Çevrimleri)
Grafikteki **her kenarı** tam olarak bir kez geçen yollara **Eulerian Path (Euler Yolu)**, başladığı yere dönen çevrimlere ise **Eulerian Circuit (Euler Çevrimi)** denir.
- **Koşul (Yönsüz Graf):**
  - Çevrim için: Tüm düğümlerin dereceleri çift olmalı ve bağlantılı olmalı.
  - Yol için: Derecesi tek olan düğüm sayısı tam olarak 0 veya 2 olmalı.
- **Algoritma (Hierholzer Algoritması):** DFS mantığıyla ziyaret edilmemiş kenarlardan ilerlenir, gidilecek kenar kalmadığında düğüm yığına atılır. Sonunda yığın ters çevrilerek Euler yolu elde edilir. Zaman Karmaşıklığı: $O(V + E)$.

### 19.2 Hamiltonian Paths (Hamilton Yolları)
Grafikteki **her düğümü** tam olarak bir kez ziyaret eden yollardır. NP-Hard bir problemdir.
- **Çözüm:** $N \le 20$ gibi küçük grafikler için dinamik programlama ve bitmask (durum sıkıştırma) ile $O(N^2 2^N)$ sürede çözülebilir.

### 19.3 De Bruijn Sequences (De Bruijn Dizileri)
Belirli bir alfabedeki tüm $K$ uzunluğundaki kelimeleri tam olarak bir kez alt dizi (substring) olarak içeren en kısa dairesel dizidir. Bu problem, $K-1$ boyutundaki öneklerin düğüm olduğu ve kenarların harf geçişlerini temsil ettiği bir graf üzerinde **Euler Çevrimi** bulmaya indirgenir.

### 19.4 Knight's Tours (Şövalye Turu)
Bir satranç tahtasında bir atın, her kareye tam olarak bir kez basacak şekilde gezinmesidir. Warnsdorff kuralı adı verilen açgözlü (greedy) yaklaşım ile çözülür: Şövalye bir sonraki adımda, oradan gidilebilecek en az hamle seçeneği olan kareye yönelir.

#### C++ Çözümü:
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Eulerian Circuit / Path Bulma (Hierholzer Algoritması)
struct EulerFinder {
    int V;
    vector<vector<pair<int, int>>> adj; // {komşu, kenar_id}
    vector<bool> edgeUsed;
    vector<int> path;

    EulerFinder(int n) : V(n), adj(n) {}

    void addEdge(int u, int v, int edge_id) {
        adj[u].push_back({v, edge_id});
        adj[v].push_back({u, edge_id});
    }

    void dfsEuler(int u) {
        while (!adj[u].empty()) {
            auto [v, idx] = adj[u].back();
            adj[u].pop_back();
            
            if (edgeUsed[idx]) continue;
            edgeUsed[idx] = true;
            dfsEuler(v);
        }
        path.push_back(u); // Geri dönüş sırasında ekle
    }

    vector<int> findCircuit(int total_edges, int start_node = 0) {
        edgeUsed.assign(total_edges, false);
        path.clear();
        dfsEuler(start_node);
        reverse(path.begin(), path.end());
        return path;
    }
};

// Hamiltonian Path Sayısını Dinamik Programlama + Bitmask ile Bulma (O(N^2 * 2^N))
int countHamiltonianPaths(int n, const vector<vector<int>>& matrix) {
    // dp[mask][u] = 'mask' kümesindeki düğümler gezilmiş ve son durak 'u' iken yol sayısı
    vector<vector<int>> dp(1 << n, vector<int>(n, 0));
    dp[1][0] = 1; // 0. düğümden başla
    
    for (int mask = 1; mask < (1 << n); mask++) {
        for (int u = 0; u < n; u++) {
            if (!(mask & (1 << u))) continue;
            if (dp[mask][u] == 0) continue;
            
            for (int v = 0; v < n; v++) {
                if (matrix[u][v] && !(mask & (1 << v))) {
                    int next_mask = mask | (1 << v);
                    dp[next_mask][v] = (dp[next_mask][v] + dp[mask][u]) % 1000000007;
                }
            }
        }
    }
    // Tüm düğümler gezilmiş ((1<<n)-1) ve son durak n-1. düğüm iken toplam yol
    return dp[(1 << n) - 1][n - 1];
}
```

---

## 20. Flows and Cuts (Akışlar ve Kesiler)

Ağ sistemlerinde kaynak (source) düğümden hedef (sink) düğüme taşınabilecek maksimum veri/su miktarını hesaplayan konulardır.

### 20.1 Ford-Fulkerson (Edmonds-Karp) Algoritması
Ağ üzerindeki maksimum akışı (Max-Flow) bulur.
- **Yöntem:** BFS yardımıyla kaynaktan hedefe giden ve hala kapasitesi olan (residual capacity > 0) yollar (augmenting paths) arar. Bulunan yol üzerindeki en küçük kenar kapasitesi kadar akış gönderilir ve kenarların kapasitelerinden çıkarılır, ters kenarlara eklenir. BFS kullanan Edmonds-Karp versiyonunun zaman karmaşıklığı $O(V \cdot E^2)$'dir.
- **Max-Flow Min-Cut Teoremi:** Bir ağdaki maksimum akış değeri, kaynağı hedeften tamamen ayıran kenarların minimum kesi (Min-Cut) ağırlığına eşittir.

### 20.2 Disjoint Paths (Ayrık Yollar)
- **Edge-Disjoint Paths:** İki düğüm arasında ortak hiçbir kenar kullanmayan maksimum yol sayısıdır. Kenar kapasiteleri 1 yapılarak Max-Flow bulunarak çözülür.
- **Node-Disjoint Paths:** İki düğüm arasında başlangıç ve bitiş hariç ortak hiçbir düğüm kullanmayan maksimum yol sayısıdır. Her düğüm $u$, aralarında kapasitesi 1 olan bir kenarla bağlı iki sanal düğüme ($u_{in} \rightarrow u_{out}$) bölünerek Max-Flow ile çözülür.

### 20.3 Maximum Matchings (Maksimum Eşleştirme)
İki kümeli bir grafikte (bipartite graph) kenarları çakışmayacak şekilde seçilebilecek maksimum kenar sayısıdır. Kaynak düğümü sol kümeye, hedef düğümü sağ kümeye bağlayıp tüm kenar kapasitelerini 1 yaparak Max-Flow ile çözülür.

### 20.4 Path Covers (Yol Kaplamaları)
Yönlü bir grafın tüm düğümlerini kapsayan en az sayıda yol kümesini bulmaktır. DAG'larda minimum yol kaplama boyutu, düğüm bölme tekniği ve maksimum eşleştirme yardımıyla çözülür (Dilworth Teoremi).

#### C++ Çözümü (Edmonds-Karp Max-Flow):
```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct EdmondsKarp {
    int V;
    vector<vector<int>> capacity;
    vector<vector<int>> adj;

    EdmondsKarp(int n) : V(n), capacity(n, vector<int>(n, 0)), adj(n) {}

    void addEdge(int u, int v, int cap) {
        adj[u].push_back(v);
        adj[v].push_back(u); // Ters kenarı da ekle (akış iadesi için)
        capacity[u][v] += cap;
    }

    // BFS ile genişletilebilir yol arama
    int bfs(int s, int t, vector<int>& parent) {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = -2; // Kaynak düğüm işareti
        
        // pair: {düğüm, o ana kadarki darboğaz akışı}
        queue<pair<int, int>> q;
        q.push({s, 1e9});
        
        while (!q.empty()) {
            auto [u, flow] = q.front(); q.pop();
            
            for (int v : adj[u]) {
                if (parent[v] == -1 && capacity[u][v] > 0) {
                    parent[v] = u;
                    int new_flow = min(flow, capacity[u][v]);
                    if (v == t) return new_flow; // hedefe varıldı
                    q.push({v, new_flow});
                }
            }
        }
        return 0;
    }

    // Maksimum Akışı (Max Flow) hesaplar
    int getMaxFlow(int s, int t) {
        int total_flow = 0;
        vector<int> parent(V);
        int new_flow;
        
        while ((new_flow = bfs(s, t, parent)) > 0) {
            total_flow += new_flow;
            int curr = t;
            while (curr != s) {
                int prev = parent[curr];
                capacity[prev][curr] -= new_flow; // Kapasiteyi düşür
                capacity[curr][prev] += new_flow; // Ters yönde akış iadesi ekle
                curr = prev;
            }
        }
        return total_flow;
    }
};
```
