# 16. Yönlü Graflar (Directed Graphs / DAG)

> **C++ Kodu:** [cpp/16_directed_graphs.cpp](./cpp/16_directed_graphs.cpp)

---

## 🤔 Neden Yönlü Graf Kullanmalıyım?

Bazı ilişkiler tek yönlüdür:
- "A dersi verilmeden B dersi alınamaz" → DAG
- Web sayfası linkleri → Directed Graph
- Görev bağımlılıkları → DAG üzerinde topo sıralama
- Fonksiyonel graflar → Her düğümden tam 1 çıkış

---

## 16.1 Topolojik Sıralama

"Eğer u→v kenarı varsa, u her zaman v'den önce gelir."

**Koşul:** Yalnızca DAG'larda (döngüsüz yönlü graflarda) mümkündür.

| Yöntem | Mantık | Döngü Tespiti |
|--------|--------|----------------|
| DFS Post-Order | Biten düğümü listeye ekle, ters çevir | State=1'e ulaşılırsa |
| Kahn's BFS | Indegree=0 olanlardan başla | Tüm düğüm işlenmediyse |

---

## 16.2 DAG Üzerinde DP

DAG'da döngü yoktur → Topolojik sıraya göre DP yaparken önceki durumlar her zaman hazırdır.

```cpp
// Topo sırada işle:
for (int u : topo) {
    for (int v : adjList[u]) {
        longestPath[v] = max(longestPath[v], longestPath[u] + 1);
    }
}
```

---

## 16.3 Binary Lifting (Ardıl Yolu)

Her düğümden tam 1 çıkış olan "fonksiyonel graf"ta:
`succ[i][x]` = x'ten **2^i adım** sonraki düğüm

```
succ[0][x] = next[x]                    // 1 adım
succ[i][x] = succ[i-1][succ[i-1][x]]   // 2^i adım = iki kez 2^(i-1)
```

**Sorgu:** k'yı binary'de aç, her 1 biti için o adımı at → O(log k)

---

## 16.4 Döngü Tespiti (Yönlü Graf)

DFS'te düğümlerin 3 durumu:
- `0` = ziyaret edilmedi
- `1` = şu an işleniyor (recursion stack'te)
- `2` = tamamen bitti

Eğer `1` durumundaki bir düğüme tekrar ulaşılırsa → **Geri kenar (back-edge) = Döngü!**
