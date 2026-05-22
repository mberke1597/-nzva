# 20. Akışlar ve Kesiler (Flows and Cuts)

> **C++ Kodu:** [cpp/20_flows_and_cuts.cpp](./cpp/20_flows_and_cuts.cpp)

---

## 🤔 Neden Max-Flow Kullanmalıyım?

**"Kaynaktan hedefe maksimum ne kadar taşıyabilirim?"** problemi.

- **Boru hattı / su şebekesi:** Maksimum akış kapasitesi
- **İnternet ağ yönlendirme:** Maksimum bant genişliği  
- **Bipartite eşleştirme:** Her eşleştirme = 1 birimlik akış
- **Ayrık yol sayısı:** Kapasite=1 → max-flow = kaç bağımsız yol var?
- **Proje seçimi:** Hangi projeleri yapıp hangilerini bırakmalı (min-cut)

---

## 20.1 Residual Graph (Artık Graf) Nedir?

Normal graftan farklı olarak, her u→v kenarına ek olarak v→u "ters kenarı" da tutulur. Ters kenar kapasitesi başta 0'dır.

```
Akış gönderince:
  capacity[u][v] -= akış   (ileri kenar azalır)
  capacity[v][u] += akış   (ters kenar artar = akışı geri alabilirsin)
```

Bu "geri alma" özelliği olmadan algoritma yanlış sonuç verebilir.

---

## 20.2 Edmonds-Karp Algoritması

Ford-Fulkerson'ın BFS kullanan versiyonu. DFS yerine BFS kullanmak O(V·E²) karmaşıklığını garanti eder.

**Her iterasyon:**
1. BFS → augmenting path bul (residual kapasite > 0 olan yol)
2. Yolun darboğaz kapasitesini hesapla (min kapasite)
3. Kapasiteleri güncelle
4. Toplam akışa ekle

---

## 20.3 Max-Flow = Min-Cut

```
Min-Cut: S ile T'yi birbirinden ayıran minimum kapasiteli kenar kümesi
Max-Flow = bu minimum kesim kapasitesine EŞİTTİR
```

Pratikte: Algoritma bittikten sonra kaynak tarafında kalan düğümler S kümesini, öte tarafkiler T kümesini oluşturur. S'den T'ye giden kenarlar = min-cut kenarlarıdır.

---

## 20.4 Bipartite Maximum Matching

```
İşçiler: {A, B, C}   Görevler: {X, Y, Z}

     S
    /|\
   A B C
  /|\ |\
 X Y Y Z Z
    \|/
     T

Her kenar kapasitesi = 1
Max-Flow(S, T) = Maksimum Eşleştirme Sayısı
```

**Dilworth Teoremi:** DAG'da minimum yol kaplama = N - Maksimum Eşleştirme
