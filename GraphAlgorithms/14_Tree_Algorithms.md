# 14. Ağaç Algoritmaları (Tree Algorithms)

> **C++ Kodu:** [cpp/14_tree_algorithms.cpp](./cpp/14_tree_algorithms.cpp)

---

## 🤔 Neden Ağaç Algoritmaları?

Ağaçlar, döngü içermeyen bağlantılı graflardır. Özel özellikleri sayesinde genel graflardan çok daha verimli çözümler mümkündür:
- N düğümlü ağaçta tam olarak N-1 kenar vardır
- Herhangi iki düğüm arasında **tam olarak bir yol** vardır
- `visited[]` dizisi gerekmez — sadece parent parametresi yeterlidir

---

## 14.1 Ağaç Gezinme

Ağaçlarda DFS'i `parent` parametresiyle yaz, `visited[]` gereksiz:

```cpp
void dfs(int u, int parent) {
    for (int v : adjList[u]) {
        if (v != parent)  // Parent'a geri gitme
            dfs(v, u);
    }
}
```

---

## 14.2 Ağaç Çapı (Tree Diameter)

"Bu ağaçtaki en uzak iki düğüm arası mesafe?" → Çap

### Yöntem 1: İki DFS (Basit, Hızlı)
1. Herhangi bir düğümden DFS → en uzak `x`'i bul
2. `x`'ten DFS → en uzak `y`'yi bul
3. `x-y` arası mesafe = **çap**

### Yöntem 2: DP (Her düğüm için)
- Her düğüm `u`'nun alt ağacından çıkan en uzun iki yolu bul
- Bu iki yolun toplamı, `u` üzerinden geçen en uzun yoldur

| Yöntem | Avantaj | Dezavantaj |
|--------|---------|------------|
| İki DFS | Kısa kod | Negatif ağırlık çalışmaz |
| DP | Genel, her düğüm için hesaplar | Biraz daha uzun |

---

## 14.3 İkili Ağaç Gezinme Sırası

```
     1
    / \
   2   3
  / \
 4   5
```

| Gezinme | Sıra | Çıktı | Kullanım |
|---------|------|-------|----------|
| Preorder | Kök→Sol→Sağ | 1 2 4 5 3 | Ağacı kopyalama |
| Inorder | Sol→Kök→Sağ | 4 2 5 1 3 | BST'de sıralı çıktı |
| Postorder | Sol→Sağ→Kök | 4 5 2 3 1 | Ağacı silme, ifade ağacı |

---

## 14.4 Önemli Formüller

```
subtreeSize[u] = 1 + Σ subtreeSize[child]    // Her alt çocuktan topla
toLeaf[u] = max(toLeaf[child] + 1)           // En uzak yaprak
diameter ≥ toLeaf[u] + toLeaf[v]             // u üzerinden geçen yol
                                              // (iki en uzun alt yol)
```
