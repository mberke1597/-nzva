# 19. Yollar ve Çevrimler (Paths and Circuits)

> **C++ Kodu:** [cpp/19_paths_and_circuits.cpp](./cpp/19_paths_and_circuits.cpp)

---

## 🤔 Euler mi, Hamilton mı?

| | Euler | Hamilton |
|---|---|---|
| **Hedef** | Her KENAR tam 1 kez | Her DÜĞÜM tam 1 kez |
| **Zorluk** | Polinom (O(V+E)) | NP-Hard |
| **Koşul** | Derece kontrolü yeterli | Genel kural yok |

---

## 19.1 Euler Path / Circuit

**Koşullar (Yönsüz Graf):**
- **Euler Circuit (Çevrim):** Tüm düğümlerin derecesi çift
- **Euler Path (Yol):** Tam olarak 2 düğümün derecesi tek (bunlar başlangıç ve bitiş olur)

**Hierholzer Algoritması:** DFS yap, gidilecek kenar kalmadığında düğümü stack'e at. Stack'i ters çevir → Euler yolu.

---

## 19.2 Hamilton Path

**NP-Hard** → Genel çözüm yoktur. Küçük N için Bitmask DP:

```
dp[mask][u] = mask kümesini ziyaret edip u'da son duraksak yol sayısı
```

N ≤ 20 için O(N² × 2^N) ile çözülebilir.

---

# 20. Akışlar ve Kesiler (Flows and Cuts)

> **C++ Kodu:** [cpp/20_flows_and_cuts.cpp](./cpp/20_flows_and_cuts.cpp)

---

## 🤔 Neden Max-Flow?

- Boru hattı tasarımı: Kaynaktan hedefe ne kadar su akabilir?
- İnternet ağı: Maksimum bant genişliği
- Bipartite eşleştirme: Max-Flow'a indirgeme
- Ayrık yol sayısı (Menger Teoremi)

---

## 20.1 Edmonds-Karp (BFS Ford-Fulkerson)

**Mantık:**
1. BFS ile residual grafta augmenting path bul
2. Path üzerindeki darboğaz (minimum) kapasite kadar akış gönder
3. Kapasiteleri güncelle (ileri kenarda azalt, ters kenarda artır)
4. Yol bulunamayana kadar tekrar et

**Residual Graph:** Her u→v kenarının bir de v→u ters kenarı vardır. Ters kenarlar "gönderilen akışı geri alma" olanağı sağlar.

**Karmaşıklık:** O(V · E²)

---

## 20.2 Max-Flow Min-Cut Teoremi

```
Maksimum Akış = Minimum Kenar Kesimi Kapasitesi
```

S kümesini T kümesinden ayıran minimum kapasiteli kenar seti = Max-Flow değeri.

---

## 20.3 Bipartite Matching → Max-Flow

```
Sol küme (1..L) ──── Sağ küme (1..R)

S → sol düğümler  (kapasite 1)
Sağ düğümler → T  (kapasite 1)
Sol-Sağ kenarları (kapasite 1)

Max-Flow(S,T) = Maksimum Eşleştirme
```
