# 18. Ağaç Sorguları (Tree Queries)

> **C++ Kodu:** [cpp/18_tree_queries.cpp](./cpp/18_tree_queries.cpp)

---

## 🤔 Neden Ağaç Sorguları?

Büyük ağaçlarda her sorgu için DFS yapmak çok yavaş olur. Binary Lifting ve Euler Tour ile sorgu süresi O(N) → O(log N) veya O(1) olur.

---

## 18.1 Binary Lifting (k. Ata)

`ancestor[u][i]` = u'nun **2^i. atası**

```
ancestor[u][0] = parent[u]
ancestor[u][i] = ancestor[ ancestor[u][i-1] ][i-1]
```

**k. atayı bul:** k'yı ikili açılımla parçala, her 1 biti için o adımı at → O(log k)

---

## 18.2 LCA — Lowest Common Ancestor

u ve v'nin en yakın ortak atası.

```
1. Derin olanı diğerinin seviyesine getir (getKthAncestor kullan)
2. Aynı düğümdeyse → LCA bulundu
3. Değilse birlikte yukarı çık (ebeveynleri farklı kaldığı sürece)
4. Son durumda parent[u] = LCA
```

**Yol mesafesi:** `depth[u] + depth[v] - 2 × depth[LCA(u,v)]`

---

## 18.3 Euler Tour (Alt Ağaç Düzleştirme)

DFS sırasında her düğüme giriş (`startTime`) ve çıkış (`endTime`) zamanı ata.

```
u'nun alt ağacındaki tüm düğümler v için:
startTime[u] ≤ startTime[v] ≤ endTime[u]
```

Bu sayede alt ağaç sorguları → düz dizi üzerinde range query → Fenwick/Segment Tree ile O(log N).
