# 16. Greedy Algorithms (Açgözlü Algoritmalar)

> **C++ Kodları:** [cpp/](./cpp/) klasörüne bakın (16_01 → 16_05)

---

## 🤔 Neden Greedy Kullanmalıyım?

Greedy algoritması, her adımda **o an için en iyi görünen seçimi** yaparak bütünde optimal çözüme ulaşmaya çalışır. Çözüm doğrudan inşa edilir, hiçbir zaman geri dönülmez.

**Avantajları:**
- Genellikle çok hızlı: O(n log n) veya daha az
- Implementasyonu basit
- Bellek tüketimi düşük

**Dezavantajı:**
- Her problemde çalışmaz! Greedy stratejinin doğruluğu **kanıtlanmalıdır**

**Kanıt yöntemleri:**
- **Exchange Argument:** İki elementin yerini değiştirince ne olduğunu göster
- **Greedy Stays Ahead:** Greedy'nin her adımda diğerlerinden geri kalmadığını kanıtla
- **Kontraörnekle çürütme:** Greedy çalışmıyorsa bir karşıt örnek bul

---

## 16.1 Coin Problem — Greedy Sınırları

**Problem:** `coins = {c1, c2, ..., ck}` ve hedef `n`. Minimum para sayısını bul.

**Greedy strateji:** Her adımda kalan toplamı aşmayan en büyük parayı seç.

**Ne zaman çalışır?** Euro seti `{1,2,5,10,20,50,100,200}` gibi **özel** para setlerinde.

**Ne zaman çalışmaz?** Genel para setlerinde. Örnek: `coins={1,3,4}`, `n=6`
- Greedy: `4+1+1 = 3 para` ❌
- Optimal: `3+3 = 2 para` ✅

> **Bu yüzden genel para değişimi → DP kullan! (bkz. 17.1)**

**C++ Kodu:** [cpp/16_01_coin_change_greedy.cpp](./cpp/16_01_coin_change_greedy.cpp)

---

## 16.2 Scheduling — Etkinlik Zamanlama

**Problem:** `[start_i, end_i]` aralıklı `n` etkinlik var. Çakışmadan **maksimum** etkinlik say.

**Neden kullanılır?** Toplantı odası planlaması, CPU görev atama, spor turnuvası programı.

**Üç strateji ve doğrulukları:**

| Strateji | Doğru mu? | Neden? |
|---------|-----------|--------|
| En kısa etkinlik önce | ❌ | Kısa ama yere sıkışmış olabilir |
| En erken başlayan önce | ❌ | `[1,100]` tüm günü kaplayabilir |
| **En erken biten önce** | **✅** | Gelecek için en geniş zamanı bırakır |

**Kanıt (Exchange Argument):** Diyelim optimal çözümde bitiş zamanı `e1` olan A yerine bitiş zamanı `e2 > e1` olan B var. A'yı B ile değiştirince A'dan sonra seçilebilecek etkinlikler kümesi **daralır** (çünkü `e2 > e1`). Yani en erken biten her zaman en az o kadar iyidir.

**C++ Kodu:** [cpp/16_02_scheduling.cpp](./cpp/16_02_scheduling.cpp)

---

## 16.3 Tasks and Deadlines — Görev Sıralama

**Problem:** Her görevin `duration` ve `deadline`'ı var. Görev bitince `deadline - bitiş_zamanı` puan kazanılır. Tüm görevleri yaparak **maksimum** puanı elde et.

**Greedy strateji:** Görevleri **sürelerine göre küçükten büyüğe** sırala.

**Kanıt (Exchange Argument):**
Süresi `a` olan X, süresi `b` olan Y ardışık sıradaysa ve `a > b`:
- X sonra Y yaparsak: Y `a` birim erken biter (+a puan), X `b` birim geç biter (-b puan)
- Net fark: `a - b > 0` → kısa olanı öne almak **her zaman daha iyi**

**Önemli Not:** Deadline değerleri sadece toplam skoru etkiler, sıralama kararını etkilemez!

**C++ Kodu:** [cpp/16_03_tasks_deadlines.cpp](./cpp/16_03_tasks_deadlines.cpp)

---

## 16.4 Minimizing Sums — Toplamı Minimize Eden x

**Problem:** `Σ |aᵢ - x|^c` toplamını minimum yapan `x`'i bul.

| c değeri | Optimal x | Neden? |
|---------|-----------|--------|
| `c = 1` | **Medyan** | x medyandan sola kayarsa sol taraftaki fark artar, sağa kayarsa sağ taraftaki |
| `c = 2` | **Aritmetik Ortalama** | `f(x) = nx² - 2xΣaᵢ + Σaᵢ²` → türev=0 → `x = Σaᵢ/n` |

**Ne zaman kullanılır?** İstatistik, ML merkez hesaplama, teslimat noktası seçimi.

**C++ Kodu:** [cpp/16_04_minimizing_sums.cpp](./cpp/16_04_minimizing_sums.cpp)

---

## 16.5 Huffman Coding — Veri Sıkıştırma

**Problem:** Sık geçen karakterlere kısa, nadir geçenlere uzun bit kodu ata → toplam bit sayısını minimize et.

**Prefix-free şartı:** Hiçbir karakterin kodu diğerinin öneki olamaz (benzersiz decode için).

**Greedy strateji:** Her adımda en düşük frekanslı iki düğümü birleştir.

**Algoritma:**
1. Her karakter için frekans say → min-heap'e at
2. En küçük 2 düğümü çek → toplamları kadar ağırlıklı yeni iç düğüm oluştur → tekrar ekle
3. Kalan tek düğüm = Huffman ağacının kökü
4. Kökten yapraklara: sol=`0`, sağ=`1` → kodları üret

**Neden optimal?** Daha sık kullanılan karakterin ağaçtaki derinliği (kod uzunluğu) daha az olur. İki düğümü birleştirme her zaman iki en küçüğü seçmek optimal prefix-free kodu garanti eder (kanıt: induction on tree structure).

**C++ Kodu:** [cpp/16_05_huffman.cpp](./cpp/16_05_huffman.cpp)

---

## 16.6 Greedy'nin Çalışmadığı Durumlar — Ne Zaman DP'ye Geç?

```
Greedy dene:
│
├── Karşıt örnek bulabildin mi?
│   └── EVET → DP kullan
│
└── Kanıt var mı? (exchange argument / induction)
    ├── EVET → Greedy güvenli
    └── HAYIR → Greedy'ye güvenme, DP dene
```

| Problem | Greedy? | DP? |
|---------|---------|-----|
| Coin Change (Euro) | ✅ | ✅ |
| Coin Change (Genel) | ❌ | ✅ |
| Etkinlik Zamanlama | ✅ | - |
| Knapsack | ❌ | ✅ |
| LIS | ❌ | ✅ |
| Huffman | ✅ | - |
