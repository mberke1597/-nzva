// ============================================================
//  10.8 — SOS DP: Alt Küme Toplamları (Sum over Subsets)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 10
//
//  ✅ struct/class YOK → global diziler kullanılır
//  ✅ O(4^n) naive → O(2^n × n) SOS DP
//  ✅ Bit bit güncelleme stratejisi açıklanır
//
//  PROBLEM:
//  Her S ⊆ {0..n-1} kümesine value[S] atanmış.
//  sum[S] = Σ value[A] for all A ⊆ S
//  Tüm sum[S] değerlerini hesapla.
//
//  NEDEN SOS DP?
//  Naive: O(4^n) — her (S,A) çifti
//  SOS:   O(2^n × n) — bit bit güncelle
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

const int MAXN = 20;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR
// ─────────────────────────────────────────────
int  val[1 << MAXN]; // value[S]
long long sumArr[1 << MAXN]; // sum[S] = S'nin alt kümeleri toplamı
int  N;              // Eleman sayısı

// ─────────────────────────────────────────────
//  NAİF ÇÖZÜM: O(4^n)
// ─────────────────────────────────────────────
void naiveSubsetSum(int n) {
    int states = 1 << n;
    for (int s = 0; s < states; s++) {
        sumArr[s] = 0;
        // s'nin tüm alt kümelerini gez
        for (int sub = s; ; sub = (sub - 1) & s) {
            sumArr[s] += val[sub];
            if (sub == 0) break;
        }
    }
}

// ─────────────────────────────────────────────
//  SOS DP: O(2^n × n)
//
//  partial(S, k) = S'nin alt kümelerinin toplamı,
//                 ama sadece 0..k bitleri çıkarılabilir
//
//  Geçiş:
//  Eğer k ∈ S: partial(S,k) = partial(S,k-1) + partial(S\{k},k-1)
//  Eğer k ∉ S: partial(S,k) = partial(S,k-1)
//
//  Kod:
//  for k in 0..n-1:
//    for s in 0..2^n-1:
//      if k ∈ s: sum[s] += sum[s ^ (1<<k)]
// ─────────────────────────────────────────────
void sosDP(int n) {
    int states = 1 << n;

    // Başlangıç: sum[S] = value[S]
    for (int s = 0; s < states; s++) sumArr[s] = val[s];

    // Bit bit güncelle
    for (int k = 0; k < n; k++) {
        for (int s = 0; s < states; s++) {
            if (s & (1 << k)) {
                // k biti S içinde → k'sız versiyonu ekle
                sumArr[s] += sumArr[s ^ (1 << k)];
            }
        }
    }
}

// ─────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Kitap örneği: n=3 ──
    // value[]={3,1,4,5,5,3,1,3} (∅,{0},{1},{0,1},{2},{0,2},{1,2},{0,1,2})
    // sum[{0,2}] = value[∅]+value[{0}]+value[{2}]+value[{0,2}] = 3+1+5+1 = 10
    cout << "=== Kitap Örneği: n=3 ===" << endl;
    N = 3;
    int vals[] = {3, 1, 4, 5, 5, 3, 1, 3};
    // mask: 000=∅, 001={0}, 010={1}, 011={0,1}, 100={2}, 101={0,2}, 110={1,2}, 111={0,1,2}
    for (int s = 0; s < (1 << N); s++) val[s] = vals[s];

    cout << "  value[] tablosu:" << endl;
    for (int s = 0; s < (1 << N); s++) {
        cout << "    value[{";
        for (int i = 0; i < N; i++) if (s & (1<<i)) cout << i << " ";
        cout << "}] = " << val[s] << endl;
    }

    // Naive
    vector<long long> naiveResult(1 << N);
    naiveSubsetSum(N);
    for (int s = 0; s < (1 << N); s++) naiveResult[s] = sumArr[s];

    // SOS DP
    sosDP(N);

    cout << "\n  Sonuçlar (Naive vs SOS):" << endl;
    bool allMatch = true;
    for (int s = 0; s < (1 << N); s++) {
        cout << "    sum[{";
        for (int i = 0; i < N; i++) if (s & (1<<i)) cout << i << " ";
        cout << "}] = " << naiveResult[s];
        if (naiveResult[s] == sumArr[s]) cout << " ✅";
        else { cout << " ❌ (SOS=" << sumArr[s] << ")"; allMatch = false; }
        cout << endl;
    }

    cout << "\n  sum[{0,2}] = " << naiveResult[(1<<0)|(1<<2)]
         << "  (beklenen: 10)" << endl;
    cout << "  Tüm değerler eşleşiyor: " << (allMatch ? "✅" : "❌") << endl;

    // ── Karmaşıklık karşılaştırması ──
    cout << "\n=== Karmaşıklık Karşılaştırması ===" << endl;
    cout << "  n  | Naive O(4^n) | SOS O(2^n × n)" << endl;
    cout << "  ---+-------------+-----------------" << endl;
    for (int n : {5, 10, 15, 20}) {
        long long naive = 1;
        for (int i = 0; i < 2*n; i++) naive *= 2; // 4^n = 2^(2n)
        long long sos = (1LL << n) * n;
        cout << "  " << n << "  | " << naive << " | " << sos << endl;
    }

    return 0;
}

/*
 * SOS DP MANTIGI:
 *   partial(S, k): S'nin alt kümelerinin toplamı,
 *                  sadece 0..k elemanları çıkarılabilir
 *
 *   k=0: Sadece 0. eleman çıkarılabilir → sum[S] = val[S] + (0∈S ? val[S\{0}] : 0)
 *   k=1: 0 ve 1 çıkarılabilir → ...
 *   ...
 *   k=n-1: Tüm elemanlar çıkarılabilir = tam alt küme toplamı
 *
 *   Kodun güzelliği: Her k için 2^n işlem → toplam O(2^n × n)
 */
