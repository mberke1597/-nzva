# 13. En Kısa Yollar (Shortest Paths)

> **C++ Kodu:** [cpp/13_shortest_paths.cpp](./cpp/13_shortest_paths.cpp)

---

## 🤔 Neden En Kısa Yol Algoritmaları Kullanmalıyım?

Ağırlıklı graflarda BFS artık yetmez — kenarların maliyetleri farklıdır.  
Kullanım alanları:
- **GPS Navigasyon** → Şehirler arası en kısa/hızlı rota
- **Ağ paket yönlendirme** → En az gecikme ile veriyi ilet
- **Oyunlarda AI** → En az hamle ile hedefe ulaş
- **Bağımlılık zinciri** → Hangi görevden önce ne yapılmalı

---

## 13.1 Hangi Algoritmayı Seçmeliyim?

```
Negatif kenar ağırlığı var mı?
├── EVET →
│       Negatif döngü var mı?
│       ├── Kontrol etmek istiyorum → Bellman-Ford
│       └── Tüm çiftler lazım     → Floyd-Warshall
└── HAYIR →
        Tek kaynaktan tüm düğümlere mi?
        ├── EVET → Dijkstra  (hızlı, O((V+E) log V))
        └── Tüm çiftler      → Floyd-Warshall  (O(V³))
```

---

## 13.2 Bellman-Ford

**Ne yapar?** Kaynak düğümden tüm diğer düğümlere en kısa mesafeyi bulur.  
**Farkı?** Negatif ağırlıklı kenarlarla çalışır.

**Nasıl çalışır?**
- V-1 kez tüm kenarları "gevşet" (relax): `dist[v] = min(dist[v], dist[u] + w)`
- V. kez gevşetmede hala bir mesafe azalıyorsa → **negatif döngü var!**

**Ne zaman Bellman-Ford?**
- Graf negatif ağırlıklı kenar içeriyor
- Negatif döngü tespiti yapman gerekiyor
- Kenar sayısı düğüm sayısına yakın (sparse değil)

**Karmaşıklık:** O(V · E)

---

## 13.3 Dijkstra

**Ne yapar?** Tek kaynaktan tüm düğümlere en kısa yol.  
**Farkı?** Negatif ağırlık olmadığında Bellman-Ford'dan çok daha hızlı.

**Nasıl çalışır?**
- Min-heap (priority_queue) ile her adımda en yakın düğümü seç
- Seçilen düğümün komşularını güncelle (relax)
- Bir düğüm heap'ten çıktıktan sonra mesafesi kesinleşir

**Ne zaman Dijkstra?**
- Kenar ağırlıkları ≥ 0 (negatif yok!)
- Hız önemli (GPS, oyun AI)
- Rekabetçi programlamada default tercih

**Karmaşıklık:** O((V + E) log V)

---

## 13.4 Floyd-Warshall

**Ne yapar?** Her çift (u, v) için en kısa mesafeyi hesaplar (all-pairs).

**Nasıl çalışır?**
- 3 içiçe döngü: `dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])`
- Her k'yı "ara nokta" olarak dene

**Ne zaman Floyd-Warshall?**
- Graf küçük (V ≤ 500 civarı)
- Tüm çiftler arasındaki mesafe lazım
- Implementasyon basitliği önemli (3 döngü yeterli!)

**Karmaşıklık:** O(V³)

---

## 13.5 Karşılaştırma Tablosu

| Algoritma | Karmaşıklık | Negatif Kenar | Negatif Döngü | Kullanım |
|-----------|-------------|---------------|----------------|----------|
| BFS | O(V+E) | ❌ (ağırlıksız) | ❌ | Ağırlıksız en kısa |
| Dijkstra | O((V+E)logV) | ❌ | ❌ | Tek kaynak, pozitif ağırlık |
| Bellman-Ford | O(V·E) | ✅ | ✅ tespit | Negatif kenarlı tek kaynak |
| Floyd-Warshall | O(V³) | ✅ | ✅ tespit | Tüm çiftler, küçük graf |
