# ⚡ Bit Manipulation & DP with Bitmask – Tam Referans Kılavuzu

> **Kaynak:** Competitive Programmer's Handbook – Antti Laaksonen (Bölüm 10)  
> **Dil:** C++17 | **Stil:** struct/class yok → global diziler kullanılır

---

## 📁 Klasör Yapısı

```
Bitwise/
├── README.md
├── 10_Bit_Manipulation.md          ← Bit teori + operasyonlar + set gösterimi
├── 10_DP_with_Bitmask.md           ← Bitmask DP teorisi + ne zaman kullanılır?
└── cpp/
    ├── 10_01_bit_basics.cpp        ← Temel bit operasyonları + __builtin fonksiyonlar
    ├── 10_02_bit_tricks.cpp        ← Sık kullanılan bit hileleri
    ├── 10_03_set_representation.cpp← Kümeleri bitmask ile temsil etme
    ├── 10_04_hamming_distance.cpp  ← Hamming mesafesi + bit optimizasyonu
    ├── 10_05_subgrid_count.cpp     ← Alt ızgara sayımı + bit optimizasyonu
    ├── 10_06_dp_optimal_selection.cpp ← Bitmask DP: Ürün seçimi
    ├── 10_07_dp_elevator.cpp       ← Bitmask DP: Asansör / Permütasyon→Subset
    └── 10_08_dp_subset_sum.cpp     ← Bitmask DP: Alt küme toplamları
```

---

## 🗺️ Hızlı Konu Haritası

| # | Konu | Ne Zaman Kullanılır? |
|---|------|----------------------|
| 10.1 | Temel Bit Operasyonları | AND, OR, XOR, NOT, Shift |
| 10.2 | Bit Hileleri | Tek/çift kontrol, LSB, 2^k testi |
| 10.3 | Set Temsili | n ≤ 30 küme elemanı → tek int |
| 10.4 | Hamming Mesafesi | XOR + popcount ile O(1) mesafe |
| 10.5 | Alt Izgara Sayımı | Satırları bitmask olarak sakla |
| 10.6 | DP: Optimal Seçim | Subset DP — ürün/gün seçimi |
| 10.7 | DP: Asansör Problemi | Permütasyon→Subset dönüşümü |
| 10.8 | DP: Alt Küme Toplamı | SOS (Sum over Subsets) DP |

---

## ⚡ Karmaşıklık Özeti

| Algoritma | Zaman | Uzay |
|-----------|-------|------|
| Bit operasyonu | O(1) | O(1) |
| Subset iterasyonu | O(2^n) | O(1) |
| Hamming (bit opt.) | O(n²/64) | O(n) |
| Alt Izgara (bit opt.) | O(n³/64) | O(n²/64) |
| DP Optimal Seçim | O(n · 2^k · k) | O(2^k · n) |
| DP Asansör | O(2^n · n) | O(2^n) |
| DP Subset Toplam (SOS) | O(2^n · n) | O(2^n) |

---

## 🔑 En Sık Kullanılan Bit Operasyonları

```cpp
x & (1 << k)      // k. bit 1 mi?
x | (1 << k)      // k. biti 1 yap
x & ~(1 << k)     // k. biti 0 yap
x ^ (1 << k)      // k. biti çevir (toggle)
x & (x - 1)       // En sağdaki 1 bitini sıfırla
x & (-x)          // Sadece en sağdaki 1 biti tut (LSB)
x & (x - 1) == 0  // x, 2'nin kuvveti mi?
__builtin_popcount(x)  // Kaç tane 1 biti var?
```

---

## 🔑 Bitmask DP Şablonu

```cpp
// dp[mask] = mask kümesi için optimal değer
int dp[1 << N];
fill(dp, dp + (1 << N), INF);
dp[0] = 0; // Başlangıç

for (int mask = 0; mask < (1 << N); mask++) {
    if (dp[mask] == INF) continue;
    for (int i = 0; i < N; i++) {
        if (mask & (1 << i)) continue;     // i zaten maskede
        int newMask = mask | (1 << i);
        dp[newMask] = min(dp[newMask], dp[mask] + cost[i]);
    }
}
// Cevap: dp[(1<<N) - 1]  (tüm elemanlar seçildi)
```
