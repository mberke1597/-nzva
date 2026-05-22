# 10. DP with Bitmask (Bitmask ile Dinamik Programlama)

> **C++ Kodları:** [cpp/10_06_dp_optimal_selection.cpp](./cpp/10_06_dp_optimal_selection.cpp), [cpp/10_07_dp_elevator.cpp](./cpp/10_07_dp_elevator.cpp), [cpp/10_08_dp_subset_sum.cpp](./cpp/10_08_dp_subset_sum.cpp)

---

## 🤔 Neden Bitmask DP Kullanmalıyım?

Bazı problemlerde durum **"hangi elemanları kullandık?"** sorusuna bağlıdır. Bunu normal dizi ile tutmak O(n!) zaman alır çünkü sıra önemlidir. Bitmask DP ise **sıra yerine kümeyi** saklar:

```
n = 20:
  n! = 2.4 × 10¹⁸  ← imkânsız
  2ⁿ = 1.048.576   ← çok hızlı!
```

**Ne zaman Bitmask DP?**
- n ≤ 20 (DP tablosu 2^20 = 1M hücre)
- "Hangi elemanlar kullanıldı?" durumu önemli
- Hamilton yolu, gezgin satıcı, kaynak atama problemleri

---

## 10.6 Optimal Seçim (Ürün/Gün DP)

**Problem:** k ürün, n gün. Her ürün bir kez alınacak, günde en fazla 1 ürün. Minimum toplam fiyat?

**Durum:** `dp[mask][d]` = mask kümesindeki ürünler alınmış, d. güne kadar minimum maliyet

```
dp[mask][d] = min(
    dp[mask][d-1],                        // Bugün hiçbir şey alma
    dp[mask ^ (1<<x)][d-1] + price[x][d] // Bugün x'i al (x ∈ mask)
)
```

**Karmaşıklık:** O(n · 2^k · k)

---

## 10.7 Permütasyon → Subset (Asansör Problemi)

**Problem:** n kişi, kapasite W asansör. Minimum tur sayısı?

**Neden Bitmask?** Hangi kişiler gruplanır sorusu → subset.

**Durum:** `best[mask]` = {minimum_tur, son_turun_ağırlığı}

```
for each mask S:
    for each person p ∈ S:
        option = best[S ^ (1<<p)]
        if option.weight + weight[p] ≤ W:
            add p to last ride
        else:
            new ride for p
        best[S] = min(best[S], option)
```

**Karmaşıklık:** O(2^n · n)

---

## 10.8 SOS DP — Alt Küme Toplamları (Sum over Subsets)

**Problem:** Her S kümesine `value[S]` atanmış. `sum[S]` = S'nin tüm alt kümelerinin değer toplamı.

**Naive:** O(4^n) — tüm (S, A⊆S) çiftleri  
**SOS DP:** O(2^n · n)

```cpp
// Başlangıç
for (int s = 0; s < (1<<n); s++) sum[s] = value[s];

// Bit bit güncelle
for (int k = 0; k < n; k++) {
    for (int s = 0; s < (1<<n); s++) {
        if (s & (1<<k))
            sum[s] += sum[s ^ (1<<k)]; // k. biti çıkar ve ekle
    }
}
```

---

## Karşılaştırma Tablosu

| Problem Tipi | Normal DP | Bitmask DP |
|-------------|-----------|------------|
| Hangi elemanlar kullanıldı? | O(n!) | **O(2^n · n)** |
| Hamilton yolu | O(n!) | **O(2^n · n²)** |
| Asansör / gruplama | O(n!) | **O(2^n · n)** |
| Alt küme toplamı | O(4^n) | **O(2^n · n)** |

**Sınır:** n ≤ 20 için 2^n ≈ 10^6 → hızlı. n > 25 ise bellek taşar.
