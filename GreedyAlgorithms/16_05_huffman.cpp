// ============================================================
//  16.5 — Huffman Kodlaması (Huffman Coding)
//  Kaynak: Competitive Programmer's Handbook, Bölüm 6
//
//  ✅ struct/class YOK → global dizi tabanlı ağaç temsili
//  ✅ Min-heap: indeks bazlı priority_queue
//  ✅ Kod üretimi DFS ile global diziler üzerinde yapılır
//
//  NEDEN HUFFMAN?
//  Sık karakterlere kısa, nadir karakterlere uzun bit kodu ata.
//  → Toplam bit sayısını minimize et (optimal prefix-free kod).
//
//  GREEDY STRATEJİSİ:
//  Her adımda en düşük frekanslı 2 düğümü birleştir.
//  Bu yerel optimal seçim küresel optimal kodlamayı üretir.
// ============================================================

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

// ─────────────────────────────────────────────
//  GLOBAL TANIMLAMALAR (struct yok!)
//  Huffman ağacı → global dizi tabanlı temsil
// ─────────────────────────────────────────────
const int MAXNODE = 520; // Maks karakter tipi * 2

int  nodeFreq[MAXNODE];       // Her düğümün frekansı (ağırlığı)
char nodeChar[MAXNODE];       // Yaprak düğümlerin karakteri ('\0' = iç düğüm)
int  leftChild[MAXNODE];      // Sol çocuk indeksi (-1 = yok)
int  rightChild[MAXNODE];     // Sağ çocuk indeksi (-1 = yok)
int  nodeCount;               // Toplam düğüm sayısı

// Huffman kodları: her karakter için bit dizisi string olarak
string huffCode[256];         // huffCode[(int)ch] = "0110..." gibi

// ─────────────────────────────────────────────
//  DÜĞÜM OLUŞTURMA
// ─────────────────────────────────────────────
int newLeaf(char ch, int freq) {
    int id = nodeCount++;
    nodeChar[id]   = ch;
    nodeFreq[id]   = freq;
    leftChild[id]  = -1;
    rightChild[id] = -1;
    return id;
}

int newInternal(int left, int right) {
    int id = nodeCount++;
    nodeChar[id]   = '\0'; // İç düğüm: karakter yok
    nodeFreq[id]   = nodeFreq[left] + nodeFreq[right];
    leftChild[id]  = left;
    rightChild[id] = right;
    return id;
}

// ─────────────────────────────────────────────
//  HUFFMAN AĞACI İNŞASI
//
//  Min-heap kullanarak en düşük frekanslı 2 düğümü birleştir.
//  Karmaşıklık: O(n log n)
// ─────────────────────────────────────────────
int buildHuffmanTree(const unordered_map<char, int>& freqMap) {
    nodeCount = 0;

    // Min-heap: {frekans, düğüm_id}
    // Frekans eşitse id'ye göre sırala (deterministik sonuç için)
    using pii = pair<int, int>;
    priority_queue<pii, vector<pii>, greater<pii>> minHeap;

    // Tüm karakterler için yaprak düğüm oluştur
    for (auto& [ch, freq] : freqMap) {
        int id = newLeaf(ch, freq);
        minHeap.push({freq, id});
    }

    // Tek karakter durumu
    if (minHeap.size() == 1) {
        int id = newLeaf('\0', 0); // Boş iç düğüm
        int leafId = minHeap.top().second; minHeap.pop();
        int root = newInternal(leafId, id);
        return root;
    }

    // İki en küçüğü birleştirerek yukarı doğru ağaç oluştur
    while (minHeap.size() > 1) {
        auto [f1, id1] = minHeap.top(); minHeap.pop();
        auto [f2, id2] = minHeap.top(); minHeap.pop();

        int parent = newInternal(id1, id2);
        minHeap.push({nodeFreq[parent], parent});
    }

    return minHeap.top().second; // Kök düğüm
}

// ─────────────────────────────────────────────
//  KOD ÜRETİMİ — DFS (global diziler üzerinde)
//
//  Kökten yapraklara giderken:
//  Sol dal  → "0" ekle
//  Sağ dal  → "1" ekle
//  Yaprak   → huffCode[charIndex] = o ana kadar biriken kod
// ─────────────────────────────────────────────
void generateCodes(int node, string currentCode) {
    if (node == -1) return;

    // Yaprak düğüm mü?
    if (leftChild[node] == -1 && rightChild[node] == -1 && nodeChar[node] != '\0') {
        huffCode[(unsigned char)nodeChar[node]] = currentCode;
        return;
    }

    generateCodes(leftChild[node],  currentCode + "0");
    generateCodes(rightChild[node], currentCode + "1");
}

// ─────────────────────────────────────────────
//  SIKIŞTIRMA — Metni kodla
// ─────────────────────────────────────────────
string encode(const string& text) {
    string result = "";
    for (char ch : text) {
        result += huffCode[(unsigned char)ch];
    }
    return result;
}

// ─────────────────────────────────────────────
//  ÇÖZME — Kodlanmış biti decode et
// ─────────────────────────────────────────────
string decode(const string& bits, int root) {
    string result = "";
    int current = root;

    for (char bit : bits) {
        if (bit == '0') current = leftChild[current];
        else            current = rightChild[current];

        // Yaprak düğüme ulaştık mı?
        if (leftChild[current] == -1 && rightChild[current] == -1) {
            result += nodeChar[current];
            current = root; // Kökten tekrar başla
        }
    }
    return result;
}

// ─────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ── Test 1: Kitap örneği ──
    // Metin: "AABACDACA"
    // Frekanslar: A=5, B=1, C=2, D=1
    // Beklenen: A → kısa kod, B/D → uzun kod
    string text = "AABACDACA";
    cout << "=== Huffman Kodlama: \"" << text << "\" ===" << endl;

    // Frekans sayımı
    unordered_map<char, int> freqMap;
    for (char ch : text) freqMap[ch]++;

    cout << "\n  Karakter Frekansları:" << endl;
    // Sıralı yazdır
    vector<pair<char,int>> freqList(freqMap.begin(), freqMap.end());
    sort(freqList.begin(), freqList.end());
    for (auto [ch, freq] : freqList) {
        cout << "    '" << ch << "' → " << freq << " kez" << endl;
    }

    // Huffman ağacı inşa et
    int root = buildHuffmanTree(freqMap);

    // Kodları üret
    fill(begin(huffCode), end(huffCode), "");
    generateCodes(root, "");

    cout << "\n  Huffman Kodları:" << endl;
    int originalBits = 0;
    int compressedBits = 0;
    for (auto [ch, freq] : freqList) {
        string code = huffCode[(unsigned char)ch];
        cout << "    '" << ch << "' [freq=" << freq
             << "] → " << code
             << "  (uzunluk=" << code.size() << " bit)" << endl;
        originalBits   += freq * 8;          // ASCII: 8 bit/karakter
        compressedBits += freq * code.size();
    }

    // Kodla ve çöz
    string encoded = encode(text);
    string decoded = decode(encoded, root);

    cout << "\n  Orijinal:  \"" << text << "\" (" << originalBits << " bit ASCII)" << endl;
    cout << "  Kodlanmış: " << encoded << " (" << compressedBits << " bit)" << endl;
    cout << "  Sıkıştırma oranı: "
         << (100.0 * (originalBits - compressedBits) / originalBits) << "%" << endl;
    cout << "  Çözülmüş: \"" << decoded << "\"" << endl;
    cout << "  Doğruluk: " << (text == decoded ? "✅ EŞİT" : "❌ FARKLI") << endl;

    // ── Test 2: Tek karakter ──
    cout << "\n=== Test 2: Tek Karakter ===\n  Metin: \"AAAA\"" << endl;
    string text2 = "AAAA";
    unordered_map<char, int> freqMap2;
    for (char ch : text2) freqMap2[ch]++;
    int root2 = buildHuffmanTree(freqMap2);
    fill(begin(huffCode), end(huffCode), "");
    generateCodes(root2, "");
    cout << "  'A' kodu: " << huffCode['A'] << endl;

    // ── Test 3: Eşit frekans ──
    cout << "\n=== Test 3: Eşit Frekans ===\n  Metin: \"ABCD\"" << endl;
    string text3 = "ABCD";
    unordered_map<char, int> freqMap3;
    for (char ch : text3) freqMap3[ch]++;
    int root3 = buildHuffmanTree(freqMap3);
    fill(begin(huffCode), end(huffCode), "");
    generateCodes(root3, "");
    cout << "  Tüm kodlar 2 bit uzunluğunda olmalı:" << endl;
    for (char ch : string("ABCD")) {
        cout << "    '" << ch << "' → " << huffCode[(unsigned char)ch] << endl;
    }

    return 0;
}

/*
 * KARMAŞIKLIK:
 *   Zaman: O(n log n) — n = benzersiz karakter sayısı
 *   Uzay:  O(n) düğüm
 *
 * NEDEN GREEDY OPTİMAL?
 *   En düşük frekanslı iki karakter, optimal ağaçta daima
 *   kardeş iki yaprak olarak en derin seviyede yer alır.
 *   (Kanıt: exchange argument + induction)
 *
 * PREFIX-FREE ŞARTI:
 *   Hiçbir karakterin kodu diğerinin öneki olamaz.
 *   Binary ağaç yapısı bunu otomatik garanti eder:
 *   Karakterler yalnızca YAPRAK düğümlerde bulunur.
 */
