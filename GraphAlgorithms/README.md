# 📊 Graph Algorithms – Tam Referans Kılavuzu

> **Kaynak:** Competitive Programmer's Handbook – Antti Laaksonen (Bölüm II, Konu 11–20)  
> **Dil:** C++17 | **Gösterim:** Adjacency List & Adjacency Matrix (struct/class yok)

---

## 📁 Klasör Yapısı

```
GraphAlgorithms/
├── README.md                  ← Bu dosya (navigasyon)
├── 11_Graph_Basics.md
├── 12_Graph_Traversal.md
├── 13_Shortest_Paths.md
├── 14_Tree_Algorithms.md
├── 15_Spanning_Trees.md
├── 16_Directed_Graphs.md
├── 17_Strong_Connectivity.md
├── 18_Tree_Queries.md
├── 19_Paths_and_Circuits.md
├── 20_Flows_and_Cuts.md
└── cpp/
    ├── 11_graph_basics.cpp
    ├── 12_graph_traversal.cpp
    ├── 13_shortest_paths.cpp
    ├── 14_tree_algorithms.cpp
    ├── 15_spanning_trees.cpp
    ├── 16_directed_graphs.cpp
    ├── 17_strong_connectivity.cpp
    ├── 18_tree_queries.cpp
    ├── 19_paths_and_circuits.cpp
    └── 20_flows_and_cuts.cpp
```

---

## 🗺️ Hızlı Konu Haritası

| # | Konu | Ne Zaman Kullanılır? | Dosya |
|---|------|----------------------|-------|
| 11 | Graf Temelleri | Grafı bellekte tutmak | [11_Graph_Basics.md](./11_Graph_Basics.md) |
| 12 | BFS / DFS Gezinme | Bağlantı, mesafe, renkleme | [12_Graph_Traversal.md](./12_Graph_Traversal.md) |
| 13 | En Kısa Yollar | Dijkstra, Bellman-Ford, Floyd | [13_Shortest_Paths.md](./13_Shortest_Paths.md) |
| 14 | Ağaç Algoritmaları | Çap, alt ağaç, ikili ağaç | [14_Tree_Algorithms.md](./14_Tree_Algorithms.md) |
| 15 | Kapsayan Ağaçlar (MST) | Kruskal, Prim, DSU | [15_Spanning_Trees.md](./15_Spanning_Trees.md) |
| 16 | Yönlü Graflar (DAG) | Topoloji, Binary Lifting | [16_Directed_Graphs.md](./16_Directed_Graphs.md) |
| 17 | Kuvvetli Bağlantı (SCC) | Kosaraju, 2-SAT | [17_Strong_Connectivity.md](./17_Strong_Connectivity.md) |
| 18 | Ağaç Sorguları | LCA, Euler Tour, Binary Lifting | [18_Tree_Queries.md](./18_Tree_Queries.md) |
| 19 | Yollar ve Çevrimler | Euler, Hamilton, De Bruijn | [19_Paths_and_Circuits.md](./19_Paths_and_Circuits.md) |
| 20 | Akışlar ve Kesiler | Max-Flow, Min-Cut, Eşleştirme | [20_Flows_and_Cuts.md](./20_Flows_and_Cuts.md) |

---

## ⚡ Karmaşıklık Özeti

| Algoritma | Zaman | Uzay |
|-----------|-------|------|
| BFS / DFS | O(V+E) | O(V) |
| Dijkstra (min-heap) | O((V+E) log V) | O(V) |
| Bellman-Ford | O(V·E) | O(V) |
| Floyd-Warshall | O(V³) | O(V²) |
| Kruskal | O(E log E) | O(V) |
| Prim (min-heap) | O((V+E) log V) | O(V) |
| Kosaraju SCC | O(V+E) | O(V) |
| LCA (Binary Lifting) | O(N log N) hazırlık, O(log N) sorgu | O(N log N) |
| Edmonds-Karp | O(V·E²) | O(V²) |

---

## 🔑 Temel Gösterim Kuralı

Bu repodaki **tüm kod**, `struct` veya `class` kullanmadan yazılmıştır.  
Bunun yerine global değişkenler tercih edilir:

```cpp
const int MAXN = 100005;

// Adjacency List (Ağırlıklı)
vector<pair<int,int>> adjList[MAXN];  // adjList[u] = {v, w}

// Adjacency Matrix (Küçük graflar için)
int adjMatrix[MAXN][MAXN];

// Edge List (Kruskal, Bellman-Ford için)
vector<tuple<int,int,int>> edges;     // {u, v, w}
```
