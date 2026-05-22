# 17. Dynamic Programming (Dinamik Programlama)

> **C++ Kodları:** [cpp/](./cpp/) klasörüne bakın (17_01 → 17_07)

---

## 🤔 Neden DP Kullanmalıyım?

DP, **kaba kuvvetin doğruluğunu** + **Greedy'nin hızını** birleştirir. Bir problem DP ile çözülebilmek için iki özelliğe sahip olmalıdır:

| Özellik | Açıklama | Test Sorusu |
|---------|----------|-------------|
| **Overlapping Subproblems** | Aynı alt problem defalarca çözülüyor | "Rekürsif çözümde aynı parametrelerle tekrar çağrılıyor mu?" |
| **Optimal Substructure** | Optimal çözüm, alt problemlerin optimal çözümlerinden oluşur | "Parçaların optimali bütünün optimali mi?" |

**DP iki şekilde kullanılır:**
1. **Optimizasyon:** En küçük/büyük değeri bul (`min`, `max`)
2. **Sayma:** Kaç farklı yol/çözüm var? (`count`)

---

## 17.1 Coin Problem — DP (Genel Durum)

**Problem:** `coins = {c1,...,ck}`, hedef `n`. Minimum para sayısı.

**Greedy ile farkı:** DP genel para setleri için her zaman doğru sonucu verir.

**Özyinelemeli formül:**
```
dp[x] = min(dp[x - c] + 1) for all c ∈ coins where x - c ≥ 0
dp[0] = 0
dp[x] = INF (başlangıç)
```

**Çözüm geri izleme:** `first[x]` = x toplamında kullanılan son para.

**C++ Kodu:** [cpp/17_01_coin_change_dp.cpp](./cpp/17_01_coin_change_dp.cpp)

---

## 17.2 Çözüm Sayısı Sayma (Counting Solutions)

**Problem:** Hedef `n` toplamını kaç **farklı sırada** oluşturabilirsin?

**Fark:** Optimizasyon değil, **sayma** problemi.

```
count[x] = Σ count[x - c]   for all c ∈ coins
count[0] = 1
```

**Dikkat:** Sıra önemliyse (permutasyon) iç döngü = coins; sıra önemsizse (kombinasyon) iç-dış döngü yer değiştirir.

**C++ Kodu:** [cpp/17_02_counting_solutions.cpp](./cpp/17_02_counting_solutions.cpp)

---

## 17.3 LIS — En Uzun Artan Altdizi

**Problem:** `a[0..n-1]` dizisinde, soldan sağa **strictly increasing** en uzun altdizi uzunluğu.

**Örnek:** `[6,2,5,1,7,4,8,3]` → LIS = `[2,5,7,8]`, uzunluk = **4**

### Yaklaşım 1: O(n²) DP
```
length[k] = 1 + max(length[i])  for all i < k where a[i] < a[k]
```

### Yaklaşım 2: O(n log n) Binary Search
- `tails[i]` = uzunluğu `i+1` olan tüm artan alt dizilerin en küçük son elemanı
- Her yeni eleman için `lower_bound` ile pozisyon bul → güncelle veya ekle
- `tails.size()` = LIS uzunluğu

**Ne zaman hangisini seç?**
- n ≤ 5000 → O(n²) yeterli, kodu kısa
- n ≤ 10^5 → O(n log n) şart

**C++ Kodu:** [cpp/17_03_lis.cpp](./cpp/17_03_lis.cpp)

---

## 17.4 Paths in a Grid — Izgara Üzerinde Yol

**Problem:** `n×m` ızgarada sol-üstten sağ-alta **sadece sağ veya aşağı** giderek maksimum toplam.

**Formül:**
```
dp[y][x] = max(dp[y-1][x], dp[y][x-1]) + grid[y][x]
dp[0][0] = grid[0][0]
```

**Neden çalışır?** (y,x) hücresine sadece 2 yönden gelinebilir → optimal gelen nereden gelirse gelsin önceki optimal yoldan geliyordur.

**Ne zaman kullanılır?** Robot hareket problemi, puana göre yol seçimi, bölge optimizasyonu.

**C++ Kodu:** [cpp/17_04_grid_paths.cpp](./cpp/17_04_grid_paths.cpp)

---

## 17.5 Knapsack (Sırt Çantası)

### Subset Sum (Hangi toplamlar elde edilebilir?)
```
possible[0] = true
for each weight w:
    for x from W down to w:     // Tersten → her eleman 1 kez kullanılır
        if possible[x-w]: possible[x] = true
```

### 0/1 Knapsack (Maksimum değer)
```
dp[0] = 0
for each item (w, v):
    for x from W down to w:     // Tersten → 0/1 (bir kez kullanım)
        dp[x] = max(dp[x], dp[x-w] + v)
```

**Neden tersten güncelleriz?** Sağdan sola gidersek aynı etki `i`'yi iki kez kullanamayız. Soldan sağa gidersek "unbounded knapsack" (sınırsız kullanım) olur.

**C++ Kodu:** [cpp/17_05_knapsack.cpp](./cpp/17_05_knapsack.cpp)

---

## 17.6 Edit Distance (Levenshtein)

**Problem:** `x` stringini `y` stringine dönüştürmek için minimum işlem sayısı.
Operasyonlar: **Ekle, Sil, Değiştir** (hepsi maliyet=1)

**Formül:**
```
dp[i][j] = x[0..i-1] ile y[0..j-1] arasındaki edit distance

dp[i][j] = min(
    dp[i][j-1] + 1,            // y'ye karakter ekle
    dp[i-1][j] + 1,            // x'ten karakter sil
    dp[i-1][j-1] + cost        // değiştir (cost=0 eğer x[i]==y[j])
)
```

**Taban durumlar:** `dp[i][0] = i`, `dp[0][j] = j` (biri boş string)

**Nerede kullanılır?** Yazım denetimi (spell check), DNA dizi karşılaştırma, diff araçları (git diff).

**C++ Kodu:** [cpp/17_06_edit_distance.cpp](./cpp/17_06_edit_distance.cpp)

---

## 17.7 Counting Tilings — Bitmask DP

**Problem:** `n×m` ızgarayı `1×2` dominolarla kapla. Kaç farklı yol var?

**Çözüm:** Izgara satır satır doldurulur. Her satırın durumu **m bitlik bitmask** ile temsil edilir.
- Bit `1` = üst satırdan dikey domino taşması var
- Bit `0` = bu hücre boş (yatay domino veya bu satırda dikey başlayacak)

**Zaman:** O(n × 2^m) → m ≤ 20 için uygulanabilir (kısa kenarı m yap!)

**C++ Kodu:** [cpp/17_07_tiling.cpp](./cpp/17_07_tiling.cpp)

---

## 17.8 DP Kalıpları Özet

| Kalıp | Formül | Örnek |
|-------|--------|-------|
| Tek boyutlu DP | `dp[i] = f(dp[i-1], dp[i-2], ...)` | Fibonacci, Coin |
| İki boyutlu DP | `dp[i][j] = f(dp[i-1][j], dp[i][j-1])` | Edit Distance, Grid |
| Aralık DP | `dp[l][r] = f(dp[l][k], dp[k+1][r])` | Matrix Chain |
| Bitmask DP | `dp[mask][u]` | Hamilton, Tiling |
| Ağaç DP | `dp[u] = f(dp[children])` | Alt ağaç boyutu |

### Bottom-up vs Top-down

| | Bottom-up (Tabulation) | Top-down (Memoization) |
|-|------------------------|------------------------|
| Yön | Küçük → Büyük | Büyük → Küçük |
| Bellek | Genellikle optimize edilebilir | Rekürsif çağrı yığını |
| Hız | Genellikle daha hızlı | Yalnızca gereken hücreleri hesaplar |
| Implementasyon | Döngü | Recursion + cache |
| **Tercih** | **Yarışmacı programlamada genellikle tercih** | Geçiş karmaşıksa |
