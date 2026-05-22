# 11. Graf Temelleri (Basics of Graphs)

> **C++ Kodu:** [cpp/11_graph_basics.cpp](./cpp/11_graph_basics.cpp)

---

## 🤔 Neden Graf Kullanmalıyım?

Gerçek dünyadaki pek çok problem aslında "**nesneler arası ilişki**" problemidir:
- Şehirler arası yol haritası → Grafın düğümleri şehirler, kenarları yollardır.
- Sosyal ağ → Düğümler insanlar, kenarlar arkadaşlık bağlarıdır.
- Bağımlılık grafiği → "A paketi kurulmadan B kurulamaz" ilişkisi.
- Oyun haritaları, devre şemaları, internet topolojisi…

Graf, bu tür problemleri **ortak bir dil**de ifade etmenizi sağlar. Bir kez graf modeline döküldüğünde, onlarca iyi bilinen algoritmayı doğrudan kullanabilirsiniz.

---

## 11.1 Temel Kavramlar

| Terim | Açıklama |
|-------|----------|
| **Vertex / Düğüm (V)** | Grafın temel elemanı |
| **Edge / Kenar (E)** | İki düğüm arası bağlantı |
| **Weight / Ağırlık** | Kenara atanan maliyet/uzunluk |
| **Degree / Derece** | Bir düğüme bağlı kenar sayısı |
| **Path / Yol** | Düğümden düğüme giden kenar dizisi |
| **Cycle / Döngü** | Başladığı yerde biten yol |
| **Connected Graph** | Her çift düğüm arasında yol var |
| **Bipartite Graph** | Düğümler iki renge boyanabilir |

---

## 11.2 Graf Gösterim Yöntemleri

### 1. Adjacency List (Komşuluk Listesi) ✅ En Çok Kullanılan

```
Graf:  1 -- 2 -- 3
            |
            4
```
```
adjList[1] = {2}
adjList[2] = {1, 3, 4}
adjList[3] = {2}
adjList[4] = {2}
```

- **Ne zaman kullanılır?** Seyrek (sparse) graflarda, yani E << V² olduğunda.
- **Uzay:** O(V + E)
- **Avantaj:** Komşuları hızlı iterate edebilirsin.

### 2. Adjacency Matrix (Komşuluk Matrisi)

```
     1  2  3  4
1  [ 0, 1, 0, 0 ]
2  [ 1, 0, 1, 1 ]
3  [ 0, 1, 0, 0 ]
4  [ 0, 1, 0, 0 ]
```

- **Ne zaman kullanılır?** Yoğun (dense) graflarda veya "u-v arası kenar var mı?" sorusunu O(1)'de cevaplamak gerektiğinde.
- **Uzay:** O(V²) → V büyükse bellek patlar!
- **Avantaj:** Floyd-Warshall gibi DP algoritmaları için idealdir.

### 3. Edge List (Kenar Listesi)

```
edges = { {1,2,5}, {2,3,7}, {2,4,6} }  // {u, v, weight}
```

- **Ne zaman kullanılır?** Kruskal, Bellman-Ford gibi "tüm kenarları sırayla işle" algoritmalarında.
- **Uzay:** O(E)

---

## 11.3 Graf Türleri

```
Yönsüz Graf        Yönlü Graf         Ağırlıklı Graf
  1 -- 2             1 --> 2            1 -5-> 2
  |    |             |                  |
  3 -- 4             v                  3
                     3
```

| Tür | Açıklama | Örnek Problem |
|-----|----------|---------------|
| Undirected | Kenarlar çift yönlü | Yol haritası |
| Directed (Digraph) | Kenarlar tek yönlü | Web sayfası linkleri |
| Weighted | Kenarlarda ağırlık var | GPS navigasyon |
| DAG | Yönlü + döngüsüz | Görev sıralama |
| Tree | Bağlantılı + döngüsüz | Aile ağacı |
| Bipartite | 2 renge boyanabilir | İş-işçi eşleştirme |

---

## 11.4 Hangi Gösterimi Seçmeliyim?

```
Graf büyük mü? (V > 1000)
    ├── Evet → Adjacency List kullan
    └── Hayır →
            Kenar kontrolü çok mu yapıyorsun?
            ├── Evet → Adjacency Matrix kullan
            └── Hayır → Adjacency List yeterli
```

| Durum | Öneri |
|-------|-------|
| Genel amaçlı (BFS, DFS, Dijkstra) | **Adjacency List** |
| Floyd-Warshall (tüm çiftler) | **Adjacency Matrix** |
| Kruskal, Bellman-Ford | **Edge List** |
| Kenar kontrolü O(1) gerekiyor | **Adjacency Matrix** |
