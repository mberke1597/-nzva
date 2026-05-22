# 17. Kuvvetli Bağlantı (Strong Connectivity / SCC)

> **C++ Kodu:** [cpp/17_strong_connectivity.cpp](./cpp/17_strong_connectivity.cpp)

---

## 🤔 Neden SCC Kullanmalıyım?

**SCC (Strongly Connected Component):** Yönlü bir grafta herhangi iki düğüm arasında çift yönlü yol bulunan maksimum düğüm kümesi.

- Web crawler: Birbirini gösteren sayfa grupları bul
- **2-SAT problemi:** Mantıksal formülleri çözmek
- Graf sıkıştırma: Her SCC tek düğüme indir → DAG elde et, DP uygula
- Sosyal ağ: "Bu insanlar birbirini kesinlikle takip ediyor" grupları

---

## 17.1 Kosaraju Algoritması

**3 Adım:**
1. **Orijinal grafta DFS** → Bitirme zamanlarına göre düğümleri kaydet
2. Grafın **tüm kenarlarını ters çevir**
3. Kaydettiğin sıradan (en son biten = kaynak SCC) **ters grafta DFS** → Her DFS çağrısı = 1 SCC

**Neden çalışır?** En son biten SCC, ters grafta "kaynak"tır — başka SCC'lerden giriş almaz, sadece kendi içine ulaşılabilir.

**Karmaşıklık:** O(V + E)

---

## 17.2 Condensation Graph

Her SCC tek bir "süper düğüm"e indirilir. Elde edilen yeni graf her zaman bir **DAG**'dır. Bu DAG üzerinde DP, en uzun yol, topolojik sıralama uygulanabilir.

---

## 17.3 2-SAT

`(x₁ ∨ x₂) ∧ (¬x₁ ∨ x₃)` gibi 2-CNF formülleri çözme:
- Her `(a ∨ b)` → `(¬a → b)` ve `(¬b → a)` kenarlarına dönüştür
- Yönlü graf üzerinde SCC bul
- Eğer herhangi bir `x` ve `¬x` **aynı SCC**'deyse → **ÇÖZÜMSÜZ**
- Aksi halde condensation DAG'ından atama yap
