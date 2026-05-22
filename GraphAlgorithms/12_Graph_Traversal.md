# 12. Graf Gezinme (Graph Traversal)

> **C++ Kodu:** [cpp/12_graph_traversal.cpp](./cpp/12_graph_traversal.cpp)

---

## 🤔 Neden BFS veya DFS Kullanmalıyım?

Graf üzerindeki düğümleri **sistematik şekilde ziyaret etmek** gerektiğinde BFS veya DFS kullanırsın. Bu iki algoritma onlarca başka algoritmanın **temelidir**:

- Grafın kaç bileşenden oluştuğunu bul → DFS/BFS
- Kaynak düğümden diğerlerine **en kısa mesafeyi** bul (ağırlıksız) → BFS
- Döngü var mı? → DFS
- İki renge boyanabilir mi? (Bipartite) → BFS
- Hangi düğümler erişilebilir? → DFS/BFS

---

## 12.1 DFS – Derinlemesine Arama

**Mantık:** "Git gittiğin kadar git, çıkmaza girince geri dön."

```
Graf:  1 - 2 - 4
       |
       3 - 5

DFS(1): 1 → 2 → 4 (geri) → (geri) → 3 → 5
```

### Ne zaman DFS seçilir?
| Durum | Neden DFS? |
|-------|------------|
| Döngü tespiti | Geri kenarlar (back-edge) DFS'te kolay görülür |
| Topolojik sıralama | Post-order DFS doğrudan topo sıra verir |
| SCC (Kosaraju) | DFS tabanlı |
| Ağaç çapı, alt ağaç boyutu | Rekürsif DFS doğal |
| Labirent çözme | Bir yolu tamamen keşfeder |

**Karmaşıklık:** O(V + E)  
**Uzay:** O(V) — rekürsif çağrı yığını

### DFS ile Mesafe Hesaplama
DFS'te mesafe takibi yapılabilir ama **ağırlıksız graflarda en kısa yolu garantilemez** — sadece "bir yol bulur". Ağırlıksız en kısa mesafe için BFS kullan.

```
dist[v] = dist[u] + 1   // DFS sırasında derinlik = mesafe (ağaçlarda geçerli)
```

---

## 12.2 BFS – Genişlemesine Arama

**Mantık:** "Önce tüm yakınları ziyaret et, sonra uzaklara geç."

```
Graf:  1 - 2 - 4
       |
       3 - 5

BFS(1): 
  Tur 1 (mesafe=1): 2, 3
  Tur 2 (mesafe=2): 4, 5
```

### Ne zaman BFS seçilir?
| Durum | Neden BFS? |
|-------|------------|
| **En kısa mesafe (ağırlıksız)** | BFS katman katman ilerler, ilk varış = en kısa |
| Bipartite kontrol | Komşuları ters renge boyarken kuyruk kullanılır |
| Sosyal ağ "kaç derece uzakta?" | Katman = derece uzaklığı |
| 0-1 BFS (kenar ağırlığı 0 veya 1) | Deque ile Dijkstra'dan hızlı |

**Karmaşıklık:** O(V + E)  
**Uzay:** O(V) — kuyruk

### BFS ile Mesafe Hesaplama ✅ (Ağırlıksız Graflar için Standart Yol)
```
dist[start] = 0
dist[v] = dist[u] + 1  (u'dan v'ye kenar varsa)
```
Ziyaret edilmemiş düğümler için dist başlangıçta -1 (ya da INF) set edilir.

---

## 12.3 Uygulama Alanları

### Bağlı Bileşen Sayısı
Kaç ayrı "ada" var? Her ziyaret edilmemiş düğümden yeni BFS/DFS başlat.

### Döngü Tespiti (Yönsüz Graf)
DFS'te ziyaret edilmiş ama parent olmayan bir komşuya ulaşırsan → döngü var.

### Bipartite Kontrol (2-Renklendirme)
BFS ile renk ver: komşu aynı renkse → bipartite değil.

---

## 12.4 BFS vs DFS Karşılaştırması

| Özellik | BFS | DFS |
|---------|-----|-----|
| Veri yapısı | Kuyruk (Queue) | Yığın (Stack) / Rekürsiyon |
| En kısa yol (ağırlıksız) | ✅ Garantilenmiş | ❌ Garanti yok |
| Bellek (geniş graf) | Daha fazla | Daha az |
| Topolojik sıra | ❌ (Kahn's var ama karmaşık) | ✅ Doğal |
| Döngü tespiti | Zor | ✅ Kolay |
| Implementasyon | Biraz uzun | Kısa (rekürsif) |
