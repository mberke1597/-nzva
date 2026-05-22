// ============================================================
//  17.6 — Edit Distance (Levenshtein Mesafesi)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 7
//
//  ✅ struct/class YOK → global 2D dizi kullanılır
//  ✅ 3 operasyon: ekle, sil, değiştir (hepsi maliyet=1)
//  ✅ DP tablosunu görsel yazdırma
//  ✅ Operasyon geri izleme (hangi adımlar atıldı?)
//
//  NEDEN KULLANILIR?
//  Yazım denetimi (spell check), DNA dizi karşılaştırma,
//  git diff, fuzzy string matching, doğal dil işleme.
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

const int MAXL = 1005;

int  dp[MAXL][MAXL]; // dp[i][j] = x[0..i-1] → y[0..j-1] edit distance
char op[MAXL][MAXL]; // Operasyon geri izleme: 'M'=match,'R'=replace,'I'=insert,'D'=delete

// ─────────────────────────────────────────────
//  EDIT DISTANCE DP
//
//  dp[i][j] = min(
//    dp[i][j-1] + 1,           // y'ye ekle
//    dp[i-1][j] + 1,           // x'ten sil
//    dp[i-1][j-1] + cost       // değiştir (cost=0 eğer x[i]==y[j])
//  )
//
//  Taban: dp[i][0] = i, dp[0][j] = j
//  Karmaşıklık: O(n * m)
// ─────────────────────────────────────────────
int editDistance(const string& x, const string& y) {
    int n = x.size(), m = y.size();

    // Taban durumlar
    for (int i = 0; i <= n; i++) { dp[i][0] = i; op[i][0] = 'D'; }
    for (int j = 0; j <= m; j++) { dp[0][j] = j; op[0][j] = 'I'; }
    op[0][0] = 'S'; // Start

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            int cost = (x[i-1] == y[j-1]) ? 0 : 1;
            int ins  = dp[i][j-1] + 1;     // Insert y[j]
            int del  = dp[i-1][j] + 1;     // Delete x[i]
            int rep  = dp[i-1][j-1] + cost;// Replace/Match

            dp[i][j] = min({ins, del, rep});

            if (dp[i][j] == rep) op[i][j] = (cost == 0) ? 'M' : 'R';
            else if (dp[i][j] == del) op[i][j] = 'D';
            else op[i][j] = 'I';
        }
    }

    return dp[n][m];
}

// DP tablosunu yazdır
void printDPTable(const string& x, const string& y) {
    int n = x.size(), m = y.size();
    cout << "  DP Tablosu:" << endl;
    cout << "       ";
    for (char ch : y) cout << "  " << ch;
    cout << endl;

    for (int i = 0; i <= n; i++) {
        cout << "  ";
        if (i == 0) cout << "  ";
        else cout << x[i-1] << " ";
        for (int j = 0; j <= m; j++) {
            cout << "  " << dp[i][j];
        }
        cout << endl;
    }
}

// Operasyonları geri izle
void printOperations(const string& x, const string& y) {
    int i = x.size(), j = y.size();
    vector<string> ops;

    while (i > 0 || j > 0) {
        char o = op[i][j];
        if (o == 'M') {
            ops.push_back("Eşleş '" + string(1, x[i-1]) + "'");
            i--; j--;
        } else if (o == 'R') {
            ops.push_back("Değiştir '" + string(1, x[i-1]) + "' → '" + string(1, y[j-1]) + "'");
            i--; j--;
        } else if (o == 'D') {
            ops.push_back("Sil '" + string(1, x[i-1]) + "'");
            i--;
        } else { // 'I'
            ops.push_back("Ekle '" + string(1, y[j-1]) + "'");
            j--;
        }
    }

    reverse(ops.begin(), ops.end());
    cout << "  Operasyonlar:" << endl;
    for (int k = 0; k < (int)ops.size(); k++) {
        cout << "    " << k+1 << ". " << ops[k] << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Kitap örneği: LOVE → MOVIE = 2
    cout << "=== Edit Distance: LOVE → MOVIE ===" << endl;
    string x1 = "LOVE", y1 = "MOVIE";
    int d1 = editDistance(x1, y1);
    printDPTable(x1, y1);
    cout << "  Edit Distance: " << d1 << endl;
    printOperations(x1, y1);

    // Test 2: ABC → ABC = 0
    cout << "\n=== Edit Distance: ABC → ABC ===" << endl;
    string x2 = "ABC", y2 = "ABC";
    cout << "  Edit Distance: " << editDistance(x2, y2) << " (beklenen: 0)" << endl;

    // Test 3: Boş string
    cout << "\n=== Edit Distance: \"\" → HELLO ===" << endl;
    string x3 = "", y3 = "HELLO";
    cout << "  Edit Distance: " << editDistance(x3, y3) << " (beklenen: 5)" << endl;

    // Test 4: Pratik örnek (spell check)
    cout << "\n=== Spell Check: kitten → sitting ===" << endl;
    string x4 = "kitten", y4 = "sitting";
    int d4 = editDistance(x4, y4);
    cout << "  Edit Distance: " << d4 << " (beklenen: 3)" << endl;
    printOperations(x4, y4);

    return 0;
}
