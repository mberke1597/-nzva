# 15. Kapsayan Ağaçlar (Spanning Trees / MST)

> **C++ Kodu:** [cpp/15_spanning_trees.cpp](./cpp/15_spanning_trees.cpp)

---

## 🤔 Neden MST Kullanmalıyım?

**MST (Minimum Spanning Tree):** Bir grafın tüm düğümlerini kapsayan, toplam kenar ağırlığı en az olan döngüsüz alt grafı.

Gerçek dünya kullanımları:
- **Elektrik şebekesi tasarımı** → En az kablo ile tüm evleri bağla
- **Bilgisayar ağı** → En az bant genişliğiyle tüm sunucuları bağla
- **Kümeleme (Clustering)** → En uzak bağlantıyı kesis, kümeler oluşur
- **Görüntü segmentasyonu** → Pikselleri MST ile grupla

---

## 15.1 DSU — Disjoint Set Union (Union-Find)

MST algoritmalarının temeli. "Bu iki düğüm aynı bileşende mi?" sorusunu neredeyse O(1)'de cevaplar.

**İki optimizasyon:**
1. **Path Compression:** `find()` çağrısında her düğümü doğrudan köke bağla
2. **Union by Size:** Küçük ağacı büyüğünün altına bağla

```
find(x):  x'in kümesinin kökü kim?
unite(a,b): a ve b kümelerini birleştir
            → false dönerse: zaten aynı kümede = döngü oluşurdu!
```

---

## 15.2 Kruskal Algoritması

**Ne zaman seç?** Seyrek (sparse) graf, E << V² durumlarında.

**Adımlar:**
1. Tüm kenarları ağırlığa göre küçükten büyüğe sırala
2. Her kenar için DSU ile kontrol: döngü oluşturur mu?
3. Oluşturmuyorsa → MST'ye ekle
4. V-1 kenar eklenince bitti

**Karmaşıklık:** O(E log E) (sıralama baskın)

---

## 15.3 Prim Algoritması

**Ne zaman seç?** Yoğun (dense) graf veya adjList zaten hazırsa.

**Adımlar:**
1. Bir başlangıç düğümü seç, MST'ye ekle
2. MST'deki düğümlerden dışarı giden en ucuz kenarı min-heap ile seç
3. Hedef düğüm zaten MST'de mi? → atla
4. Değilse → MST'ye ekle, kenarlarını heap'e at

**Karmaşıklık:** O((V + E) log V)

---

## 15.4 Kruskal vs Prim

| Özellik | Kruskal | Prim |
|---------|---------|------|
| Veri yapısı | Edge List + DSU | Adj List + Min-Heap |
| Seyrek graf | ✅ Daha hızlı | ✅ de iyi |
| Yoğun graf | Yavaşlar | ✅ Daha hızlı |
| Implementasyon | Biraz daha kısa | Dijkstra'ya benzer |
| Negatif ağırlık | ✅ Çalışır | ✅ Çalışır |
