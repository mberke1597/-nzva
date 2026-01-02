# Data Structures & Algorithms (DSA) – Interview Roadmap

This is a concise but complete DSA curriculum in one Markdown file, with C++ and Python examples and interview-style focus.

---

## 0. Types, Limits and Overflow (Ekstra)

Bu bölüm DSA çözerken özellikle **C++** için veri tipleri, overflow ve zaman karmaşıklığı limitleri hakkında hızlı bir referanstır.

**Nerede kullanılır?** Problemdeki `n`, değer aralıkları ve olası overflow risklerine göre doğru veri tipini ve algoritma karmaşıklığını seçerken.
**Nerede kullanma?** Bu kısım doğrudan bir veri yapısı/algoritma değil; ama gerçek sorularda tip seçimini **asla ihmal etme**.

### 0.1 C++ Temel Sayısal Tipler

| Tip            | Bit | Yaklaşık Aralık           |
|----------------|-----|---------------------------|
| `int`          | 32  | -2^31 .. 2^31-1 ≈ ±2.1e9 |
| `long long`    | 64  | ≈ -9e18 .. 9e18          |
| `unsigned int` | 32  | 0 .. ≈ 4.2e9            |
| `float`        | 32  | kayan nokta (yaklaşık)   |
| `double`       | 64  | daha hassas kayan nokta  |

**Overflow örneği (yanlış / doğru):**

```cpp
int a = 1e9, b = 1e9;
cout << a * b; // OVERFLOW, yanlış
```

Bu kodda `int` tipinin sınırını aştığımız için çarpım sonucu overflow olur ve yanlış değer üretir.

```cpp
long long a = 1e9, b = 1e9;
cout << a * b; // Doğru, long long yeterli
```

Burada ise aynı işlemi daha geniş aralıklı `long long` ile yaptığımız için overflow olmadan doğru sonuç alınır.

### 0.2 Python

- `int` pratikte sınırsızdır (overflow yok), ama C++'a göre daha yavaştır.

### 0.3 N Büyüklüğüne Göre Kabul Edilen Karmaşıklık

| n üst sınırı | Güvenli zaman karmaşıklığı        |
|--------------|-----------------------------------|
| ≤ 10^4       | O(n^2)                           |
| ≤ 10^5       | O(n log n)                       |
| ≤ 10^6       | O(n)                             |
| ≤ 10^7       | O(n) (sınırda, çok optimize olmalı) |

## 1. Arrays

**Definition**  
Contiguous block of memory storing fixed-size sequence of same-typed elements.

**Complexities**

| Operation                 | Time | Space |
|---------------------------|------|-------|
| Access by index           | O(1) | O(n)  |
| Update by index           | O(1) | O(n)  |
| Insert/delete at end*     | O(1) amort. | O(n) |
| Insert/delete at middle   | O(n) | O(n)  |
| Search (unsorted)         | O(n) | O(1)  |

**Nerede kullanılır?** Rastgele erişimin çok önemli olduğu ve dizinin boyutunun nispeten sabit kaldığı durumlarda.
**Nerede kullanma?** Ortaya sık sık eleman ekleyip sildiğin durumlarda; her seferinde kaydırma maliyeti O(n) olur.

**C++**
```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    vector<int> a = {1, 2, 3};
    a.push_back(4);          // O(1) amortized
    int x = a[2];            // random access
    auto it = find(a.begin(), a.end(), 3); // linear search
    cout << x << "\n";
    return 0;
}
```

Bu örnek `vector` ile dinamik dizi oluşturup sonuna eleman eklemeyi, indeks ile erişimi ve `find` fonksiyonu ile lineer aramayı gösterir.

**Python**
```python
arr = [1, 2, 3]
arr.append(4)      # amortized O(1)
print(arr[2])      # random access
print(3 in arr)    # linear search
```

Bu Python kodu list'e eleman eklemeyi, indeksle erişimi ve `in` operatörüyle lineer aramayı örnekler.

### 1.1 C++ `vector` – Ekstra Fonksiyon Özeti

```cpp
vector<int> v;
v.push_back(x);
v.pop_back();
v.size();
v.empty();
v.clear();
v[i];
```

Fonksiyon açıklamaları (Türkçe):

- `vector<int> v;`  
    `int` tipinde dinamik bir dizi (vector) oluşturur. Başlangıçta boştur.

- `v.push_back(x);`  
    Vektörün **sonuna** bir eleman ekler.  
    Parametre: `x` → eklenecek eleman (ör. `int`).  
    Amortize zaman: O(1).

- `v.pop_back();`  
    Son elemanı siler.  
    Parametre yok. Boş vektörde çağırırsanız tanımsız davranış (UB) olur.

- `v.size();`  
    Şu an vektörde kaç eleman olduğunu döndürür.  
    Dönüş tipi: `size_t`.

- `v.empty();`  
    Vektör boşsa `true`, değilse `false` döner.  
    `v.size() == 0` kontrolüne göre daha okunaklıdır.

- `v.clear();`  
    Tüm elemanları siler, boyutu 0 yapar. Kapasite genelde korunur.

- `v[i];`  
    `i` indeksindeki elemana **O(1)** zamanda erişir.  
    Parametre: `i` → 0‑based indeks. Geçersiz indeks UB'dir; güvenli erişim için `v.at(i)` kullanılabilir.

---

## 2. Linked Lists

### 2.1 Singly Linked List

**Definition**  
Nodes with (value, next-pointer); no random access.

**Complexities**

| Operation                 | Time | Space |
|---------------------------|------|-------|
| Access k-th               | O(n) | O(n)  |
| Insert/delete at head     | O(1) | O(1)  |
| Insert/delete after node* | O(1) | O(1)  |

**Nerede kullanılır?** Listenin başında/ortasında çok sık ekleme-silme yaptığın, ama rastgele indeks erişimine ihtiyaç duymadığın durumlarda.
**Nerede kullanma?** Elemanlara sık sık `k`’ıncı indeks üzerinden erişmen gerekiyorsa; her erişim O(n) olur.

**C++**
```cpp
struct Node {
    int val;
    Node* next;
    Node(int v): val(v), next(nullptr) {}
};

struct List {
    Node* head = nullptr;

    void push_front(int v) {
        Node* n = new Node(v);
        n->next = head;
        head = n;
    }

    void remove(int v) {
        if (!head) return;
        if (head->val == v) {
            Node* t = head; head = head->next; delete t; return;
        }
        Node* cur = head;
        while (cur->next && cur->next->val != v) cur = cur->next;
        if (!cur->next) return;
        Node* t = cur->next; cur->next = t->next; delete t;
    }
};
```

Ekstra fonksiyon açıklamaları (C++ `List`):

- `void push_front(int v);`  
    Listenin **başına** yeni bir düğüm ekler.  
    Parametre: `v` → yeni düğümün değeri.  
    Zaman: O(1).

- `void remove(int v);`  
    Listedeki **ilk** `v` değerine sahip düğümü siler.  
    Parametre: `v` → aranıp silinecek değer.  
    Boş listede veya listede `v` yoksa hiçbir şey yapmaz.  
    Zaman: en kötü O(n) (tüm listeyi gezebilir).

**Python**
```python
class Node:
    def __init__(self, v: int):
        self.v = v
        self.next = None

class List:
    def __init__(self) -> None:
        self.head = None

    def push_front(self, v: int) -> None:
        n = Node(v)
        n.next = self.head
        self.head = n

    def remove(self, v: int) -> None:
        if not self.head:
            return
        if self.head.v == v:
            self.head = self.head.next
            return
        cur = self.head
        while cur.next and cur.next.v != v:
            cur = cur.next
        if cur.next:
            cur.next = cur.next.next
```

Ekstra fonksiyon açıklamaları (Python `List`):

- `push_front(self, v: int) -> None`  
    Listenin başına yeni bir düğüm ekler.  
    Parametre: `v` → eklenecek değer.

- `remove(self, v: int) -> None`  
    Listedeki ilk `v` değerini bulup siler.  
    Parametre: `v` → silinmek istenen değer.  
    Eğer değer yoksa fonksiyon sessizce çıkar (hata atmaz).


### 2.2 Doubly Linked List

Adds prev-pointer; O(1) delete given node.

**C++ STL**
```cpp
list<int> dl;
dl.push_front(2);
dl.push_back(3);
```

Bu kod C++ STL `list<int>` (çift yönlü bağlı liste) kullanarak başa ve sona eleman eklemenin nasıl yapıldığını gösterir.

---

## 3. Stack (LIFO)

**Nerede kullanılır?** İfade değerlendirme, geri alma (undo), DFS, parantez dengeleme, çağrı yığını benzeri LIFO durumlarında.
**Nerede kullanma?** Rastgele eleman erişimi veya kuyruğa benzer FIFO davranışı gerektiğinde.

| Operation | Time | Space |
|-----------|------|-------|
| push/pop  | O(1) | O(n)  |

**C++**
```cpp
stack<int> st;
st.push(1);
st.push(2);
int x = st.top(); st.pop();
```

Bu örnek bir `stack<int>` üzerinde eleman itme (`push`), en üstteki elemanı okuma (`top`) ve silme (`pop`) işlemlerini sergiler.

### 3.1 Ekstra – C++ `stack` Fonksiyon Açıklamaları

```cpp
stack<int> st;
st.push(x);
st.pop();
st.top();
st.empty();
```

- `stack<int> st;`  
    `int` tutan bir LIFO yığını oluşturur. Altındaki container varsayılan olarak `deque<int>`'tir.

- `st.push(x);`  
    Yığının **tepesine** yeni eleman ekler.  
    Parametre: `x` → eklenecek değer.

- `st.pop();`  
    Tepedeki elemanı **siler** (geri döndürmez).  
    Parametre yoktur. Boş yığında çağrılırsa tanımsız davranış; önce `st.empty()` ile kontrol et.

- `st.top();`  
    Tepedeki elemanı referans olarak **okur**, silmez.  
    Parametre yok, dönüş tipi bu örnekte `int&`.

- `st.empty();`  
    Yığında eleman yoksa `true`, varsa `false` döner.

**Python**
```python
st: list[int] = []
st.append(1)
st.append(2)
x = st.pop()
```

Python tarafında normal listeyi yığın gibi kullanıp `append` ile tepeye ekleme ve `pop` ile sondan silme işlemi yapılır.

---

## 4. Queue / Deque (FIFO)

**Nerede kullanılır?** BFS, görev sıralama, üretici-tüketici kuyrukları, akış/buffer yapılarında.
**Nerede kullanma?** Ortadan rastgele silme/ekleme veya stack tarzı sadece sondan çalışma gerektiğinde.

| Structure | Operations (amort.) | Typical impl |
|-----------|---------------------|--------------|
| Queue     | enqueue/dequeue O(1)| array/list   |
| Deque     | push/pop both ends O(1) | circular buf |

**C++**
```cpp
queue<int> q;
q.push(1); q.push(2);
int f = q.front(); q.pop();

deque<int> dq;
dq.push_front(1);
dq.push_back(2);
```

Bu kod C++ `queue` ile FIFO kuyruğa eleman ekleyip baştan çekmeyi, `deque` ile de hem başa hem sona ekleme işlemlerini gösterir.

**Python**
```python
from collections import deque
q = deque()
q.append(1); q.append(2)
f = q[0]; q.popleft()
```

Python `deque` örneğinde kuyruğa eleman eklenir, baştaki elemana indeksle erişilir ve `popleft` ile baştan silinir.

### 4.1 Ekstra – C++ `queue` ve `deque` Fonksiyonları

```cpp
queue<int> q;
q.push(x);
q.pop();
q.front();

deque<int> dq;
dq.push_front(x);
dq.push_back(x);
dq.pop_front();
dq.pop_back();
```

`queue<int> q;`  
FIFO kuyruk. Alt container varsayılan `deque<int>`'tir.

- `q.push(x);` → Kuyruğun **sonuna** eleman ekler. Parametre: `x` (eklencek değer).
- `q.pop();`  → Kuyruğun **başındaki** elemanı siler. Boşsa tanımsız davranış; önce `q.empty()` kontrol et.
- `q.front();` → Sıradaki (ilk) elemanı referans olarak döndürür, silmez.

`deque<int> dq;`  
İki uçtan da O(1) ekleme/silme yapılabilen yapı.

- `dq.push_front(x);` → Başa eleman ekler. Parametre: `x`.
- `dq.push_back(x);`  → Sona eleman ekler.
- `dq.pop_front();`   → Baştaki elemanı siler.
- `dq.pop_back();`    → Sondaki elemanı siler.
- `dq.front()` / `dq.back()` → Sırasıyla ilk ve son elemanı okur.

---

## 5. Hash Table (Map / Set)

Average O(1) insert/find/erase; worst O(n).

**Nerede kullanılır?** Frekans sayımı, üyelik (set/dict kontrolü), cache tutma, key→value eşleştirme gibi O(1) ortalama erişim gereken yerlerde.
**Nerede kullanma?** Elemanların **sırasına** veya sıralı olmasına ihtiyaç varsa (ör. aralık sorguları, `k`’inci küçük eleman); ağaç tabanlı yapılar veya sıralı konteynerler daha uygun.

**C++**
```cpp
unordered_map<string,int> freq;
for (string s: {"a","b","a"}) freq[s]++;
cout << freq["a"]; // 2
```

Bu örnek bir `unordered_map` ile string frekans sayımı yapar; her görülen kelimenin sayaç değerini bir arttırır.

**Python**
```python
from collections import Counter
freq = Counter(["a", "b", "a"])
print(freq["a"])  # 2
```

Burada Python `Counter` ile aynı frekans sayımının daha kısa şekilde nasıl yapılacağı gösterilir.

### 5.1 Ekstra – C++ `unordered_map` ve `unordered_set` Fonksiyonları

```cpp
unordered_map<string,int> mp;
mp[key] = value;      // insert/update
mp.insert({key,val});
mp.find(key);
mp.count(key);
mp.erase(key);
mp.size();
mp.clear();

unordered_set<int> s;
s.insert(x);
s.count(x);
s.erase(x);
```

**`unordered_map<Key, T> mp;`**  
Hash tabanlı key→value map. Ortalama O(1) erişim.

- `mp[key] = value;`  
    Eğer `key` yoksa **yeni** bir entry oluşturur ve value atar.  
    Parametreler: `key` (anahtar), `value` (değer).  
    Dikkat: `mp[key]` erişimi de yoksa default-constructed değer oluşturur.

- `mp.insert({key, val});`  
    Key zaten varsa ekleme yapmaz (insert başarısız olur).  
    Parametre: `pair<Key,T>` veya `make_pair(key,val)`.

- `mp.find(key);`  
    İteratör döner. Eğer `key` yoksa `mp.end()` döner.  
    Parametre: aranacak `key`.

- `mp.count(key);`  
    0 veya 1 döner (unordered_map aynı key'i sadece bir kez tutar).  
    Parametre: `key`.

- `mp.erase(key);`  
    Verilen key'i siler (varsa).  
    Parametre: `key`.

- `mp.size();`  
    Kaç (key,value) çifti olduğunu döndürür.

- `mp.clear();`  
    Tüm entry'leri siler.

**`unordered_set<T> s;`**  
Sadece anahtar tutan hash tabanlı küme.

- `s.insert(x);` → Eleman ekler. Varsa tekrar eklenmez.
- `s.count(x);`  → 0 (yok) veya 1 (var) döner.
- `s.erase(x);`  → Elemanı siler (varsa).

**İteratör ile gezinme (`unordered_map` / `unordered_set`)**

```cpp
for (auto &p : mp) {
        // p.first: key, p.second: value
}

for (auto it = mp.begin(); it != mp.end(); ++it) {
        // (*it).first, (*it).second
}

for (int x : s) {
        // kümedeki her x elemanı
}
```

- `mp.begin()` / `mp.end()`  
    Hash tablosunun başına ve **sona-sonrası** konuma işaret eden iterator döndürür.  
    `begin()`'den `end()`'e kadar dolaşarak tüm `(key,value)` çiftlerini gezebilirsin.  
    Dikkat: Sıralı değildir; insertion order **garanti edilmez**.

- Range-based `for (auto &p : mp)`  
    Daha kısa sözdizimi.  
    `p.first` → key, `p.second` → value.  
    Referans (`&`) kullanırsan doğrudan güncelleme yapabilirsin: `p.second++` gibi.

- `mp.erase(it);`  
    Verilen iterator'ın gösterdiği öğeyi siler.  
    Parametre: `it` → `unordered_map<Key,T>::iterator`.  
    Tipik pattern: `auto it = mp.find(key); if (it != mp.end()) mp.erase(it);`.

- `s.begin()` / `s.end()` ve `for (int x : s)`  
    Küme elemanlarını sırayla gezmek için kullanılır.  
    Her `x` kümede bulunan bir anahtardır (sıra yine rastgele; sıralı değildir).

---

## 6. Trees & BST

### 6.1 Binary Tree Basics

**Node**
```cpp
struct TreeNode {
    int v; TreeNode* l; TreeNode* r;
    TreeNode(int x): v(x), l(nullptr), r(nullptr) {}
};
```

Bu yapı ikili ağaç düğümünü temsil eder; her düğümün bir değeri (`v`) ve sol/sağ çocuk pointer'ları (`l`, `r`) vardır.

Traversals: preorder (root-left-right), inorder (left-root-right), postorder (left-right-root).

**C++ inorder**
```cpp
void inorder(TreeNode* root) {
    if (!root) return;
    inorder(root->l);
    cout << root->v << " ";
    inorder(root->r);
}
```

Bu fonksiyon ikili ağaç üzerinde **inorder** (sol-kök-sağ) dolaşma yapar ve düğüm değerlerini sırayla ekrana yazar.

**Python**
```python
class TreeNode:
    def __init__(self, v: int):
        self.v = v; self.l = None; self.r = None

def inorder(root: TreeNode) -> None:
    if not root: return
    inorder(root.l)
    print(root.v, end=" ")
    inorder(root.r)
```

Python sürümü de aynı inorder dolaşmayı rekürsif olarak gerçekleştirip düğüm değerlerini ekrana basar.

### 6.2 Binary Search Tree (BST)

Property: left < node < right.

| Operation | Avg | Worst (skewed) |
|----------|-----|-----------------|
| search   | O(log n) | O(n) |
| insert   | O(log n) | O(n) |

**Nerede kullanılır?** Sıralı veriler üzerinde arama, ekleme ve silme yaparken hem **düzen** (order) hem de logaritmik zaman karmaşıklığı istediğinde (örn. aralık sorguları, en küçük/büyük bulma).
**Nerede kullanma?** Sadece üyelik kontrolleri yapıyorsan ve sıraya ihtiyacın yoksa; hash tabloları genelde daha basit ve hızlıdır.

**C++**
```cpp
TreeNode* insert(TreeNode* root, int x) {
    if (!root) return new TreeNode(x);
    if (x < root->v) root->l = insert(root->l, x);
    else if (x > root->v) root->r = insert(root->r, x);
    return root;
}

bool search(TreeNode* root, int x) {
    if (!root) return false;
    if (root->v == x) return true;
    if (x < root->v) return search(root->l, x);
    return search(root->r, x);
}
```

Ekstra fonksiyon açıklamaları (BST – C++):

- `TreeNode* insert(TreeNode* root, int x);`  
    Değeri `x` olan düğümü BST'ye ekler ve **yeni kökü** döndürür.  
    Parametreler: `root` → mevcut kök pointer'ı (boş olabilir), `x` → eklenecek değer.  
    `x < root->v` ise sol alt ağaçta, `x > root->v` ise sağ alt ağaçta rekürsif ekleme yapar.

- `bool search(TreeNode* root, int x);`  
    BST içinde `x` değerinin olup olmadığını döndürür.  
    Parametreler: `root` → kök pointer'ı, `x` → aranan değer.  
    Bulursa `true`, bulamazsa `false` döner.

**Python**
```python
def insert(root: TreeNode | None, x: int) -> TreeNode:
    if not root:
        return TreeNode(x)
    if x < root.v:
        root.l = insert(root.l, x)
    elif x > root.v:
        root.r = insert(root.r, x)
    return root
```

Bu Python fonksiyonu verilen `x` değerini BST'ye uygun konuma ekler ve güncellenmiş kök düğümü geri döndürür.

---

## 7. Heaps & Priority Queue

Binary heap: complete tree with heap property (min-heap or max-heap).

| Operation       | Time |
|----------------|------|
| push / pop top | O(log n) |
| peek top       | O(1) |

**C++** (min-heap)
```cpp
priority_queue<int, vector<int>, greater<int>> pq;
for (int x: {5,3,7}) pq.push(x);
while (!pq.empty()) { cout << pq.top() << " "; pq.pop(); }
```

Bu kod `priority_queue` ile min-heap oluşturup elemanları küçükten büyüğe doğru `top`/`pop` kombinasyonuyla çıkarır.

**Python**
```python
import heapq
h = []
for x in [5, 3, 7]:
    heapq.heappush(h, x)
while h:
    print(heapq.heappop(h), end=" ")
```

Python `heapq` kullanılarak benzer şekilde min-heap oluşturulur ve elemanlar sıralı biçimde `heappop` ile alınır.

Use: Dijkstra, scheduling k-smallest, merges.

### 7.1 Ekstra – C++ `priority_queue` Kullanımı

```cpp
priority_queue<int> maxpq; // max-heap (varsayılan)
priority_queue<int, vector<int>, greater<int>> minpq; // min-heap
```

- `priority_queue<int> maxpq;`  
    Varsayılan: en **büyük** eleman tepede (max-heap).  
    Şablon parametreleri: `T` (tutulan tip), `Container` (default `vector<T>`), `Compare` (default `less<T>`).

- `priority_queue<int, vector<int>, greater<int>> minpq;`  
    En **küçük** eleman tepede (min-heap).  
    Burada üçüncü şablon parametresi olan karşılaştırıcıyı `greater<int>` yaparak min-heap elde ediyoruz.

Ortak fonksiyonlar:

- `pq.push(x);` → Yeni elemanı ekler (O(log n)). Parametre: `x`.
- `pq.pop();`  → Tepedeki elemanı siler (O(log n)). Parametre yok.
- `pq.top();`  → Tepedeki elemanı okur (O(1)). Parametre yok.
- `pq.empty();` / `pq.size();` → Boşluk ve eleman sayısı bilgisini verir.

---

## 8. Trie (Prefix Tree)

Efficient prefix queries on strings.

**Python**
```python
class TrieNode:
    def __init__(self) -> None:
        self.children: dict[str, TrieNode] = {}
        self.end = False

class Trie:
    def __init__(self) -> None:
        self.root = TrieNode()

    def insert(self, word: str) -> None:
        node = self.root
        for ch in word:
            node = node.children.setdefault(ch, TrieNode())
        node.end = True

    def search(self, word: str) -> bool:
        node = self.root
        for ch in word:
            if ch not in node.children:
                return False
            node = node.children[ch]
        return node.end
```

`TrieNode` her harf için bir `children` sözlüğü ve kelimenin bitişini gösteren `end` bayrağı tutan temel düğüm sınıfıdır; `Trie` sınıfı ise kelime ekleme ve aramayı gerçekleştirir.

Ekstra fonksiyon açıklamaları (Python `Trie`):

- `insert(self, word: str) -> None`  
    Verilen `word` kelimesini trie'a ekler.  
    Parametre: `word` → eklenmek istenen string.  
    Her karakter için gerekli child düğümleri yaratılır, son düğümün `end` bayrağı `True` yapılır.

- `search(self, word: str) -> bool`  
    Trie içinde tam olarak `word` kelimesinin olup olmadığını kontrol eder.  
    Parametre: `word` → aranacak string.  
    Tüm karakterler sırayla bulunduysa ve son düğümde `end == True` ise `True` döner.

---

## 9. Graphs & Traversals

### 9.1 Representation

Adjacency list (preferred for sparse graphs).

**C++**
```cpp
int n = 5;
vector<vector<int>> g(n);
auto add = [&](int u, int v) {
    g[u].push_back(v);
    g[v].push_back(u);
};
```

Bu C++ örneği `vector<vector<int>>` ile basit, yönsüz bir grafiği adjacency list şeklinde kurar ve `add` lambda'sı ile kenar ekler.

**Python**
```python
n = 5
adj = [[] for _ in range(n)]

def add(u: int, v: int) -> None:
    adj[u].append(v)
    adj[v].append(u)
```

Python sürümü de aynı adjacency list yapısını liste içinde listelerle kurar ve `add` fonksiyonu ile iki yönlü kenar ekler.

### 9.2 BFS

Finds shortest path (edges count) in unweighted graphs.

**Python**
```python
from collections import deque

def bfs(start: int, adj: list[list[int]]) -> list[int]:
    n = len(adj)
    dist = [-1] * n
    dq = deque([start])
    dist[start] = 0
    while dq:
        u = dq.popleft()
        for v in adj[u]:
            if dist[v] == -1:
                dist[v] = dist[u] + 1
                dq.append(v)
    return dist
```

Bu BFS fonksiyonu tek kaynaklı en kısa mesafe (kenar sayısı cinsinden) hesaplar ve her düğüm için mesafeleri `dist` listesiyle döndürür.

### 9.3 DFS

Useful for components, cycle detection, topological sort (on DAG).

**C++**
```cpp
void dfs(int u, const vector<vector<int>>& g, vector<int>& vis) {
    vis[u] = 1;
    for (int v: g[u]) if (!vis[v]) dfs(v, g, vis);
}
```

Bu DFS fonksiyonu verilen başlangıç düğümünden ulaşılabilen tüm düğümleri rekürsif olarak ziyaret eder ve `vis` dizisinde işaretler.

---

## 10. Union-Find (Disjoint Set Union – DSU)

Tracks connectivity between elements with near O(1) `find`/`union`.

**C++**
```cpp
struct DSU {
    vector<int> p, r;
    DSU(int n): p(n), r(n,0) { iota(p.begin(), p.end(), 0); }
    int find(int x){ return p[x]==x?x:p[x]=find(p[x]); }
    void unite(int a,int b){
        a=find(a); b=find(b);
        if(a==b) return;
        if(r[a]<r[b]) swap(a,b);
        p[b]=a;
        if(r[a]==r[b]) r[a]++;
    }
};
```

Ekstra fonksiyon açıklamaları (C++ `DSU`):

- `DSU(int n);`  
    0'dan `n-1`'e kadar her elemanı **kendi kümesinde** olacak şekilde başlatır.  
    Parametre: `n` → eleman sayısı.

- `int find(int x);`  
    `x` elemanının ait olduğu kümeyi temsil eden **kök** elemanı döndürür.  
    Parametre: `x` → sorgulanan eleman.  
    Path-compression kullandığı için amortize zaman neredeyse O(1).

- `void unite(int a, int b);`  
    `a` ve `b` elemanlarının kümelerini **birleştirir**.  
    Parametreler: `a`, `b` → birleştirilmek istenen elemanlar.  
    Rank/height'ı küçük olan kökü büyük olana bağlayarak dengeyi korur.

Use: connectivity queries, Kruskal MST, dynamic components.

---

## 11. Searching Algorithms

### 11.1 Linear Search

Any array/list; O(n).

**Python**
```python
def linear_search(a: list[int], x: int) -> int:
    for i, v in enumerate(a):
        if v == x:
            return i
    return -1
```

Bu fonksiyon dizide soldan sağa gezerek `x` değerini arar; bulursa indeksini, bulamazsa `-1` döndürür.

### 11.2 Binary Search (sorted array)

O(log n), requires sorted & random-access.

**C++**
```cpp
int bs(const vector<int>& a, int x) {
    int lo = 0, hi = (int)a.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] == x) return mid;
        if (a[mid] < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}
```

Bu klasik ikili arama fonksiyonu sıralı `vector` içinde `x` elemanını logaritmik zamanda arar; bulursa indeksini aksi halde `-1` döndürür.

---

## 12. Sorting Algorithms (Interview-Level)

### 12.1 Insertion Sort (small / nearly-sorted)

**Python**
```python
def insertion_sort(a: list[int]) -> None:
    for i in range(1, len(a)):
        key = a[i]
        j = i - 1
        while j >= 0 and a[j] > key:
            a[j+1] = a[j]
            j -= 1
        a[j+1] = key
```

Bu fonksiyon eklemeli sıralama algoritmasını uygular; diziyi soldan sağa gezer ve her elemanı soldaki sıralı kısım içine doğru yere kaydırır.

### 12.2 Merge Sort (stable, O(n log n))

**C++**
```cpp
void merge_vec(vector<int>& a, int l, int m, int r) {
    vector<int> L(a.begin()+l, a.begin()+m+1), R(a.begin()+m+1, a.begin()+r+1);
    int i=0,j=0,k=l;
    while(i<(int)L.size() && j<(int)R.size())
        a[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while(i<(int)L.size()) a[k++] = L[i++];
    while(j<(int)R.size()) a[k++] = R[j++];
}

void merge_sort(vector<int>& a, int l, int r) {
    if(l>=r) return;
    int m = (l+r)/2;
    merge_sort(a,l,m);
    merge_sort(a,m+1,r);
    merge_vec(a,l,m,r);
}
```

`merge_sort` fonksiyonu diziyi böl ve fethet mantığıyla ikiye ayırıp rekürsif sıralar; `merge_vec` ise iki sıralı alt diziyi tek sıralı dizi halinde birleştirir.

### 12.3 Quick Sort (average O(n log n))

**Python (functional)**
```python
def quick_sort(a: list[int]) -> list[int]:
    if len(a) <= 1:
        return a
    pivot = a[len(a)//2]
    left  = [x for x in a if x < pivot]
    mid   = [x for x in a if x == pivot]
    right = [x for x in a if x > pivot]
    return quick_sort(left) + mid + quick_sort(right)
```

Bu fonksiyon pivot seçip diziyi `pivot`'dan küçük, eşit ve büyük parçalara ayırarak rekürsif hızlı sıralama uygular.

---

## 13. Graph Algorithms (Weighted)

### 13.1 Dijkstra (non-negative weights)

**Python**
```python
import heapq
from typing import List, Tuple

def dijkstra(n: int, src: int, adj: List[List[Tuple[int,int]]]) -> List[int]:
    INF = 10**18
    dist = [INF]*n
    dist[src] = 0
    pq: list[Tuple[int,int]] = [(0, src)]
    while pq:
        d,u = heapq.heappop(pq)
        if d > dist[u]:
            continue
        for v,w in adj[u]:
            nd = d + w
            if nd < dist[v]:
                dist[v] = nd
                heapq.heappush(pq, (nd, v))
    return dist
```

Bu Dijkstra implementasyonu kaynak düğümden tüm düğümlere olan en kısa yolları (toplam ağırlık) min-heap (`heapq`) kullanarak hesaplar.

### 13.2 Topological Sort (DAG)

**C++ – Kahn’s algorithm**
```cpp
vector<int> topo(int n, const vector<vector<int>>& g) {
    vector<int> indeg(n); for(int u=0;u<n;++u) for(int v:g[u]) indeg[v]++;
    queue<int> q; for(int i=0;i<n;++i) if(!indeg[i]) q.push(i);
    vector<int> order;
    while(!q.empty()){
        int u=q.front();q.pop(); order.push_back(u);
        for(int v:g[u]) if(--indeg[v]==0) q.push(v);
    }
    if((int)order.size()!=n) return {}; // cycle
    return order;
}
```

Bu fonksiyon Kahn algoritması ile DAG üzerinde topolojik sıralama yapar; eğer tüm düğümler sıraya giremiyorsa grafikte döngü olduğunu anlar.

---

## 14. Problem-Solving Patterns

### 14.1 Two Pointers (sorted pair sum)

**Python**
```python
def two_sum_sorted(a: list[int], target: int) -> tuple[int,int] | None:
    i, j = 0, len(a)-1
    while i < j:
        s = a[i] + a[j]
        if s == target: return i, j
        if s < target: i += 1
        else: j -= 1
    return None
```

Bu iki uçlu gösterge (two pointers) fonksiyonu sıralı dizide toplamı `target` olan bir çift indeks bulmaya çalışır.

### 14.2 Sliding Window (fixed size)

**C++**
```cpp
int max_sum_k(const vector<int>& a, int k) {
    if ((int)a.size() < k) return 0;
    int win = 0;
    for (int i=0;i<k;++i) win += a[i];
    int best = win;
    for (int i=k;i<(int)a.size();++i) {
        win += a[i] - a[i-k];
        best = max(best, win);
    }
    return best;
}
```

Bu kayan pencere (sliding window) fonksiyonu uzunluğu `k` olan tüm alt diziler arasından maksimum toplamı bulur.

### 14.3 Prefix Sum (range sum)

**Python**
```python
a = [1,2,3,4]
p = [0]
for x in a:
    p.append(p[-1] + x)
# sum on [l,r] = p[r+1] - p[l]
```

### 14.4 Backtracking (subsets)

**Python**
```python
def subsets(nums: list[int]) -> list[list[int]]:
    res: list[list[int]] = []
    def dfs(i: int, cur: list[int]) -> None:
        if i == len(nums):
            res.append(cur.copy()); return
        dfs(i+1, cur)
        cur.append(nums[i])
        dfs(i+1, cur)
        cur.pop()
    dfs(0, [])
    return res
```

### 14.5 Dynamic Programming – Fibonacci (tabulation)

**C++**
```cpp
long long fib(int n){
    if(n<=1) return n;
    long long a=0,b=1;
    for(int i=2;i<=n;++i){ long long c=a+b; a=b; b=c; }
    return b;
}
```

---

## 15. How to Practice

- Re-implement each structure/algorithm in C++ and Python from memory.
- For each, be ready to:
  - Explain definition & use-cases.
  - State time/space complexities.
  - Walk through a small example on a whiteboard.
- Solve lots of problems by pattern:
  - Arrays/strings → two pointers, sliding window, prefix sums.
  - Trees/graphs → DFS/BFS, recursion.
  - DP → recognize overlapping subproblems & optimal substructure.

This file is intentionally dense but compact: treat it as a checklist and reference while you drill problems on LeetCode / HackerRank / etc.
