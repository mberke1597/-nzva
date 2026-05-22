# 🧠 DP & Greedy Algorithms – Tam Referans Kılavuzu

> **Kaynak:** Competitive Programmer's Handbook – Antti Laaksonen (Bölüm II, Konu 6–7)  
> **Dil:** C++17 | **Stil:** struct/class yok → global diziler kullanılır

---

## 📁 Klasör Yapısı

```
DPGreedy/
├── README.md                          ← Bu dosya (navigasyon)
├── 16_Greedy.md                       ← Greedy teori + neden kullanmalıyım?
├── 17_Dynamic_Programming.md          ← DP teori + neden kullanmalıyım?
└── cpp/
    ├── 16_01_coin_change_greedy.cpp   ← Açgözlü para değişimi
    ├── 16_02_scheduling.cpp           ← Etkinlik zamanlama
    ├── 16_03_tasks_deadlines.cpp      ← Görev & deadline
    ├── 16_04_minimizing_sums.cpp      ← Medyan / Ortalama
    ├── 16_05_huffman.cpp              ← Huffman kodlaması
    ├── 17_01_coin_change_dp.cpp       ← DP para değişimi
    ├── 17_02_counting_solutions.cpp   ← Yol sayısı sayma
    ├── 17_03_lis.cpp                  ← En Uzun Artan Altdizi (LIS)
    ├── 17_04_grid_paths.cpp           ← Izgara üzerinde maksimum yol
    ├── 17_05_knapsack.cpp             ← Sırt çantası (0/1 + Subset Sum)
    ├── 17_06_edit_distance.cpp        ← Levenshtein düzenleme mesafesi
    └── 17_07_tiling.cpp               ← Izgara kaplama (Bitmask DP)
```

---

## 🗺️ Hızlı Konu Haritası

### Bölüm 16 — Greedy (Açgözlü)

| # | Konu | Ne Zaman Kullanılır? |
|---|------|----------------------|
| 16.1 | Coin Change (Greedy) | Standart para setlerinde (Euro gibi) |
| 16.2 | Etkinlik Zamanlama | Çakışmayan maks. etkinlik sayısı |
| 16.3 | Görev & Deadline | Toplam skoru maximize et |
| 16.4 | Toplamları Minimize | Medyan / Ortalama soruları |
| 16.5 | Huffman Kodlama | Karakter frekansıyla sıkıştırma |

### Bölüm 17 — Dynamic Programming (DP)

| # | Konu | Ne Zaman Kullanılır? |
|---|------|----------------------|
| 17.1 | Coin Change (DP) | Genel para setleri, minimum para |
| 17.2 | Çözüm Sayısı Sayma | Kaç farklı yol var? |
| 17.3 | LIS | En uzun artan altdizi |
| 17.4 | Izgara Yolları | Sol-üst'ten sağ-alta maksimum toplam |
| 17.5 | Knapsack | Subset sum, 0/1 çanta |
| 17.6 | Edit Distance | İki string arası minimum işlem |
| 17.7 | Izgara Kaplama | Bitmask DP ile domino sayma |

---

## ⚡ Karmaşıklık Özeti

| Algoritma | Zaman | Uzay |
|-----------|-------|------|
| Greedy Coin | O(k log k) | O(1) |
| Etkinlik Zamanlama | O(n log n) | O(n) |
| Huffman | O(n log n) | O(n) |
| Coin DP | O(n·k) | O(n) |
| LIS (O(n²)) | O(n²) | O(n) |
| LIS (Binary Search) | O(n log n) | O(n) |
| Izgara Yolları | O(n·m) | O(n·m) |
| Knapsack 0/1 | O(n·W) | O(W) |
| Edit Distance | O(n·m) | O(n·m) |
| Tiling (Bitmask) | O(n · 2^m) | O(n · 2^m) |

---

## 🔑 Greedy vs DP: Ne zaman hangisi?

```
Problemi analiz et:
│
├── Her adımda "en iyi" seçim açıkça görülüyor mu?
│   VE geri dönmeye gerek yok mu?
│   └── EVET → Greedy dene
│           Ama kanıtla! (Exchange argument veya induction)
│
└── Alt problemler tekrarlıyor mu?
    Optimal alt yapı var mı?
    └── EVET → Dynamic Programming kullan
```

| Özellik | Greedy | DP |
|---------|--------|----|
| Hız | Genellikle O(n log n) | Genellikle O(n²) veya daha fazla |
| Geri dönüş | Yok | Tüm alt problemleri çözer |
| Doğruluk | Kanıt gerekir | Tüm durumları kapsar |
| Bellek | Az | Daha fazla (tablo) |
| Örnek | Huffman, Zamanlama | LIS, Edit Distance, Knapsack |

---

## 🔑 DP Şablonu (Genel)

```cpp
// Bottom-up (Tabulation) — Genellikle önerilen
vector<int> dp(N + 1, INF); // veya 0, duruma göre
dp[0] = BASE_CASE;

for (int x = 1; x <= N; x++) {
    for (auto& secenek : secenekler) {
        dp[x] = min/max(dp[x], dp[x - secenek] + MALIYET);
    }
}
// Cevap: dp[N]
```
