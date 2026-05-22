// ============================================================
//  17.4 — Izgara Üzerinde Yol (Paths in a Grid)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 7
//
//  ✅ struct/class YOK → global 2D dizi kullanılır
//  ✅ Sadece sağa ve aşağıya hareket izni
//  ✅ Yol geri izleme: hangi hücrelerden geçildi?
//
//  NEDEN KULLANILIR?
//  Robot navigasyonu, para toplama oyunları,
//  risk analizi (en karlı güzergah), bölge optimizasyonu.
//
//  FORMÜL:
//  dp[y][x] = max(dp[y-1][x], dp[y][x-1]) + grid[y][x]
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 1005;

int  grid[MAXN][MAXN]; // Izgara değerleri
int  dp[MAXN][MAXN];   // dp[y][x] = (0,0)'dan (y,x)'e maksimum toplam
char from[MAXN][MAXN]; // Geri izleme: 'L'=soldan, 'U'=yukarıdan

int N, M; // Satır ve sütun sayısı

// ─────────────────────────────────────────────
//  IZGARA DP
// ─────────────────────────────────────────────
int maxPathSum() {
    // İlk hücre
    dp[0][0] = grid[0][0];

    // İlk satır: sadece soldan gelebilir
    for (int x = 1; x < M; x++) {
        dp[0][x] = dp[0][x-1] + grid[0][x];
        from[0][x] = 'L';
    }

    // İlk sütun: sadece yukarıdan gelebilir
    for (int y = 1; y < N; y++) {
        dp[y][0] = dp[y-1][0] + grid[y][0];
        from[y][0] = 'U';
    }

    // Geri kalan hücreler
    for (int y = 1; y < N; y++) {
        for (int x = 1; x < M; x++) {
            if (dp[y-1][x] >= dp[y][x-1]) {
                dp[y][x] = dp[y-1][x] + grid[y][x];
                from[y][x] = 'U'; // Yukarıdan gel
            } else {
                dp[y][x] = dp[y][x-1] + grid[y][x];
                from[y][x] = 'L'; // Soldan gel
            }
        }
    }

    return dp[N-1][M-1];
}

// Izgarayı ve DP tablosunu yazdır
void printGrid() {
    cout << "  Izgara:" << endl;
    for (int y = 0; y < N; y++) {
        cout << "    ";
        for (int x = 0; x < M; x++) cout << grid[y][x] << " ";
        cout << endl;
    }
    cout << "  DP tablosu:" << endl;
    for (int y = 0; y < N; y++) {
        cout << "    ";
        for (int x = 0; x < M; x++) cout << dp[y][x] << " ";
        cout << endl;
    }
}

// Yolu geri izle
void printPath() {
    vector<pair<int,int>> path;
    int y = N-1, x = M-1;
    path.push_back({y, x});
    while (y > 0 || x > 0) {
        if (from[y][x] == 'U') y--;
        else x--;
        path.push_back({y, x});
    }
    reverse(path.begin(), path.end());
    cout << "  Yol: ";
    for (auto [r, c] : path) cout << "(" << r << "," << c << ")=" << grid[r][c] << " ";
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Test 1: 3x3 ızgara
    cout << "=== Izgara Yolu — 3x3 ===" << endl;
    N = 3; M = 3;
    int g1[3][3] = {{3,7,9},{2,8,6},{0,5,1}};
    for (int y = 0; y < N; y++)
        for (int x = 0; x < M; x++) grid[y][x] = g1[y][x];

    int res1 = maxPathSum();
    printGrid();
    cout << "  Maksimum toplam: " << res1 << endl;
    printPath();

    // Test 2: Negatif değerler
    cout << "\n=== Izgara Yolu — 2x4 (negatif dahil) ===" << endl;
    N = 2; M = 4;
    int g2[2][4] = {{1,-3,4,2},{2,5,-1,3}};
    for (int y = 0; y < N; y++)
        for (int x = 0; x < M; x++) grid[y][x] = g2[y][x];

    int res2 = maxPathSum();
    printGrid();
    cout << "  Maksimum toplam: " << res2 << endl;
    printPath();

    return 0;
}
