# 10. Bit Manipulation (Bit Manipülasyonu)

> **C++ Kodları:** [cpp/](./cpp/) klasörüne bakın (10_01 → 10_05)

---

## 🤔 Neden Bit Manipülasyonu Kullanmalıyım?

Bilgisayar zaten her şeyi bit olarak tutar — bit operasyonları **doğrudan donanım seviyesinde** çalışır:

- **Hız:** Bir bit operasyonu = 1 CPU komutu → toplamdan 30–100x hızlı olabilir
- **Bellek:** 32 elemanlı bir kümeyi tek `int`'te sakla (normal `set` yerine)
- **Şıklık:** Karmaşık küme işlemlerini tek satırda yaz

**Gerçek dünya örnekleri:**
- Izgarada siyah köşe sayımı: O(n³) → O(n³/64) bit optimizasyonuyla
- Hamming mesafesi: karakter karşılaştırma yerine `popcount(a^b)` ile O(1)
- Bitmask DP: permütasyon yerine subset iterasyonu → n! yerine 2^n

---

## 10.1 Bit Gösterimi

```
43  = 00000000000000000000000000101011  (32-bit int)
     bit31 ... bit5 bit4 bit3 bit2 bit1 bit0
                      1         1    0    1    1
     = 1×2⁵ + 1×2³ + 1×2¹ + 1×2⁰ = 32+8+2+1 = 43
```

| Tip | Bit | Aralık |
|-----|-----|--------|
| `int` | 32 | -2³¹ .. 2³¹-1 |
| `unsigned int` | 32 | 0 .. 2³²-1 |
| `long long` | 64 | -2⁶³ .. 2⁶³-1 |

---

## 10.2 Temel Bit Operasyonları

| Op | Sembol | Kural | Örnek |
|----|--------|-------|-------|
| AND | `&` | Her ikisi de 1 ise 1 | `22 & 26 = 18` |
| OR | `\|` | En az biri 1 ise 1 | `22 \| 26 = 30` |
| XOR | `^` | Tam olarak biri 1 ise 1 | `22 ^ 26 = 12` |
| NOT | `~` | Tüm bitleri çevir | `~29 = -30` |
| Sol kaydır | `<<` | k sıfır bit ekle (sağdan) | `14 << 2 = 56` |
| Sağ kaydır | `>>` | k biti at (sağdan) | `49 >> 3 = 6` |

---

## 10.3 Sık Kullanılan Bit Hileleri

```cpp
x & 1           // x tek mi? (1=tek, 0=çift)
x & (1 << k)    // k. bit 1 mi?
x | (1 << k)    // k. biti 1 yap
x & ~(1 << k)   // k. biti 0 yap
x ^ (1 << k)    // k. biti toggle et (0→1 veya 1→0)
x & (x-1)       // En sağdaki 1 bitini sıfırla
x & (-x)        // Sadece en sağdaki 1 biti tut (LSB isolate)
x & (x-1) == 0  // x > 0 ve 2'nin kuvveti mi?
x | (x-1)       // En sağdaki 1 bitten sonrakileri hepsini 1 yap
```

---

## 10.4 GCC Builtin Fonksiyonlar

```cpp
__builtin_popcount(x)  // Kaç tane 1 biti var?
__builtin_clz(x)       // Baştan kaç 0 biti var? (leading zeros)
__builtin_ctz(x)       // Sondan kaç 0 biti var? (trailing zeros)
__builtin_parity(x)    // 1 bitlerin sayısının paritesi (tek/çift)
```

64-bit versiyonları için `ll` suffix ekle: `__builtin_popcountll(x)`

---

## 10.5 Küme (Set) Gösterimi

`n ≤ 30` elemanlı kümeleri tek bir `int` ile temsil edebilirsin:

```
{1, 3, 4, 8} = bit1 + bit3 + bit4 + bit8
             = 2 + 8 + 16 + 256 = 282
             = 00000000000000000000000100011010
```

| Küme İşlemi | Matematiksel | Bit Kodu |
|-------------|--------------|----------|
| Kesişim (∩) | a ∩ b | `a & b` |
| Birleşim (∪) | a ∪ b | `a \| b` |
| Tümleyen (ā) | ā | `~a` |
| Fark (\\) | a \\ b | `a & (~b)` |
| Eleman ekle | a ∪ {k} | `a \| (1 << k)` |
| Eleman sil | a \\ {k} | `a & ~(1 << k)` |
| k ∈ a? | | `a & (1 << k)` |
| Boyut (\|a\|) | | `__builtin_popcount(a)` |

**Tüm alt kümeleri gez:**
```cpp
for (int b = 0; b < (1 << n); b++) { /* b bir alt küme */ }

// Tam olarak k elemanlı alt kümeler:
for (int b = 0; b < (1 << n); b++)
    if (__builtin_popcount(b) == k) { /* işle */ }

// x'in alt kümelerini gez:
int b = 0;
do { /* b işle */ } while (b = (b - x) & x);
```

---

## 10.6 Bit Optimizasyonları

### Hamming Mesafesi
```
Naif:   O(k) karakter karşılaştırması
Bit:    O(1) → popcount(a ^ b)
```
XOR bit farklılıklarını 1 yapar → popcount ile say.

### Alt Izgara Sayımı
Her satırı N bitlik bloklar halinde sakla.
`N = 64` ile `popcount(color[a][i] & color[b][i])` → 64 sütunu birden işle.
**Sonuç:** O(n³) → O(n³/64)
