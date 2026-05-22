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

**Nerede kullanılır?** Her zaman en küçük/en büyük elemanı hızlıca çekmen gereken yerlerde (Dijkstra, k‑küçük eleman, akış/scheduling problemleri).
**Nerede kullanma?** Tüm elemanlara sık sık rastgele erişmen gerekiyorsa; heap sadece tepe elemanı için optimize edilmiştir.

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

**Nerede kullanılır?** Otomatik tamamlama, sözlükte prefix arama, çok sayıda string üzerinde ortak prefix tabanlı işlemler yaparken.
**Nerede kullanma?** Az sayıda kısa string varsa veya sadece tam eşitlik kontrolü gerekiyorsa; normal `set`/`dict` çoğu zaman yeterlidir.

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

**Nerede kullanılır?** Yol bulma, bileşen sayma, bağlılık/bağımlılık ilişkileri (sosyal ağ, yol ağı, dependency graph) modellemek istediğinde.
**Nerede kullanma?** Veri doğası gereği lineer veya tamamen tablosal ise ve aralarında kenar ilişkisi yoksa graf modeline zorlamaya gerek yok.

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

**Nerede kullanılır?** Bağlı bileşenler, arkadaş grupları, ada sayma, Kruskal MST gibi sadece "aynı kümede mi?" sorusunu sorduğun problemlerde.
**Nerede kullanma?** Gerçek yol/rota bulman gerekiyorsa; DSU sadece bağlantı bilgisini tutar, yolun kendisini vermez.

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

**Nerede kullanılır?** Tüm veri yapılarının içinde eleman ararken; lineer arama küçük/rasgele verilerde, ikili arama büyük ve **sıralı** dizilerde.
**Nerede kullanma?** Binary search'ü veri sıralı değilken veya rastgele erişim olmayan yapılarda (saf linked list) kullanma.

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

**Nerede kullanılır?** Veriyi sıralaman gereken neredeyse her yerde; küçük veya neredeyse sıralı dizilerde insertion sort, büyük genel vakalarda merge/quick sort.
**Nerede kullanma?** Sıralı olmayan ama sadece tek seferlik min/max gerekiyorsa bazen heap veya tek geçişli algoritmalar daha uygundur.

### 12.1 Insertion Sort (small / nearly-sorted)

**Nerede kullanılır?** Küçük `n` veya neredeyse sıralı listelerde; implementasyonu basit olduğundan bazen hibrit algoritmalarda alt seviye olarak.
**Nerede kullanma?** Büyük ve rastgele dağılmış verilerde; O(n²) maliyet çok pahalı olur.

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

**Nerede kullanılır?** Stabil sıralama gerektiğinde veya linked list gibi random access olmayan yapılarda.
**Nerede kullanma?** Ek O(n) bellek ayıramıyorsan; in-place değildir.

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

**Nerede kullanılır?** Ortalama durumda çok hızlı olduğundan pratikte genel amaçlı sıralayıcı olarak.
**Nerede kullanma?** En kötü O(n²) durumunun problem olabileceği, pivot seçiminin zor olduğu durumlarda.

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

**Nerede kullanılır?** Ağırlıklı yollarda en kısa mesafe (Dijkstra) veya bağımlılık grafiğinde sıralama (topological sort) yaparken.
**Nerede kullanma?** Negatif ağırlıklı kenarlar varsa klasik Dijkstra kullanma; Bellman-Ford veya farklı yöntem gerekir.

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

**Nerede kullanılır?** Farklı problemleri aynı kalıp altında çözmek için; two pointers/sliding window dizi/substringlerde, DP yineleyen alt problemlerde.
**Nerede kullanma?** Zorla her soruyu tek bir pattern'e uydurmaya çalışma; önce problemi iyi modelle.

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
---

## 16. Greedy Algorithms (Chapter 6 - Kitap Çözümleri & Detaylı Açıklamalar)

Açgözlü (Greedy) algoritmalar, her adımda o an için en iyi görünen (yerel olarak optimal) seçimi yaparak bütünde optimal bir çözüme ulaşmayı hedefler. Yapılan kararlardan asla geri dönülmez; çözüm doğrudan inşa edilir. Bu yüzden oldukça hızlı çalışırlar. Ancak en büyük zorluk, seçilen açgözlü stratejinin her zaman küresel olarak en iyi çözümü (globally optimal) verdiğini kanıtlamaktır.

### 16.1 Coin Problem (Bozuk Para Değişimi - Greedy Sınırları)

**Problem Tanımı:**  
Elimizde $coins = \{c_1, c_2, ..., c_k\}$ değerlerinde sınırsız sayıda bozuk para bulunmaktadır. Hedefimiz, toplam değeri $n$ olan bir para miktarını **minimum** sayıda bozuk para kullanarak oluşturmaktır.

**Açgözlü Strateji:**  
Her adımda, kalan toplam miktarı aşmayacak şekilde değeri en büyük olan bozuk parayı seçmek.

**Ne zaman çalışır? (Örnek: Euro/Dolar Seti):**  
Euro madeni paraları $\{1, 2, 5, 10, 20, 50, 100, 200\}$ cent için greedy yaklaşım her zaman optimaldir.  
*Kanıt Mantığı:* Daha küçük paraların optimal bir çözümdeki kombinasyonları, bir büyük paranın değerini geçemez. Örneğin, optimal bir çözümde 2 ve 20 centlik madeni paralar en fazla 2 kez yer alabilir (çünkü 3 tane 2 cent yerine 5+1 cent, 3 tane 20 cent yerine 50+10 cent koyabiliriz). Bu kurallar zinciriyle, her $x$ madeni parası için, $x$ değerinden daha küçük paralarla yapılabilecek maksimum optimal toplamın her zaman $x-1$ olduğu gösterilir. Dolayısıyla, her zaman en büyük madeni parayı seçmek matematiksel olarak en az sayıda parayı garanti eder.

**Ne zaman çalışmaz? (Genel Durum - Karşıt Örnek):**  
Genel para setlerinde greedy algoritma yanlış sonuçlar verebilir.  
*Örnek:* $coins = \{1, 3, 4\}$ ve hedef toplam $n = 6$ olsun.
- **Greedy yaklaşım:** Önce en büyük para olan $4$'ü seçer, geriye $2$ kalır. Sonra $1$ ve tekrar $1$ seçer. Sonuç: $4 + 1 + 1$ (3 adet para).
- **Optimal çözüm:** İki adet $3$'lük para seçmektir: $3 + 3 = 6$ (2 adet para).

**C++ Çözümü:**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Greedy bozuk para değişimi algoritması
void greedyCoinChange(int n, vector<int> coins) {
    // Paraları büyükten küçüğe doğru sıralıyoruz
    sort(coins.rbegin(), coins.rend());
    
    cout << "Hedef Miktar: " << n << "\nSecilen Paralar: ";
    int coin_count = 0;
    
    for (int coin : coins) {
        while (n >= coin) {
            n -= coin;
            cout << coin << " ";
            coin_count++;
        }
    }
    cout << "\nToplam madeni para sayisi: " << coin_count << "\n";
    if (n > 0) {
        cout << "Kalan ve olusturulamayan miktar: " << n << "\n";
    }
}
```

---

### 16.2 Scheduling (Etkinlik/Aralık Zamanlama)

**Problem Tanımı:**  
Başlangıç ve bitiş zamanları $[s_i, e_i]$ olarak verilen $n$ adet etkinlik bulunmaktadır. Hiçbir etkinlik birbiriyle çakışmayacak (kesişmeyecek) şekilde, programa dahil edilebilecek **maksimum** etkinlik sayısını bulmamız istenmektedir.

**Olası Stratejiler ve Analizleri:**
1. **En kısa etkinlikleri önce seçmek:** Yanlış.  
   *Karşıt Örnek:* $[1, 5]$ ve $[6, 10]$ şeklinde iki uzun etkinlik olsun. Bir de ortalarında sıkışmış $[4, 7]$ şeklinde çok kısa bir etkinlik olsun. Kısa olanı seçersek sadece 1 etkinlik yapabiliriz. Ancak uzun olan ikisini seçersek 2 etkinlik yapabiliriz.
2. **En erken başlayan etkinlikleri önce seçmek:** Yanlış.  
   *Karşıt Örnek:* $[1, 100]$ gibi çok erken başlayan ama bütün günü kaplayan bir etkinlik ile $[2, 3]$ ve $[4, 5]$ gibi kısa etkinlikler olsun. İlkini seçersek diğerlerini engelleyeceği için sadece 1 etkinlik yapabiliriz.
3. **Bitiş zamanı en erken olan etkinlikleri önce seçmek:** **DOĞRU!**  
   *Neden çalışır?* Her adımda bitiş zamanı en erken olan etkinliği seçmek, arkasından seçilebilecek diğer etkinlikler için gelecekte **mümkün olan en geniş zaman aralığını** bırakır.

**C++ Çözümü:**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Event {
    int start;
    int end;
};

// Etkinlikleri bitiş zamanına göre artan sırada sıralayan karşılaştırıcı
bool compareEvents(const Event& a, const Event& b) {
    return a.end < b.end;
}

int maxEvents(vector<Event>& events) {
    if (events.empty()) return 0;
    
    // Bitiş zamanına göre sırala
    sort(events.begin(), events.end(), compareEvents);
    
    int count = 1;
    int last_end_time = events[0].end; // İlk seçilen etkinliğin bitişi
    
    for (size_t i = 1; i < events.size(); i++) {
        // Eğer yeni etkinliğin başlangıcı, son seçilenin bitişine eşit veya sonraysa seç
        if (events[i].start >= last_end_time) {
            count++;
            last_end_time = events[i].end;
        }
    }
    return count;
}
```

---

### 16.3 Tasks and Deadlines (Görevler ve Son Teslim Tarihleri)

**Problem Tanımı:**  
Her birinin bir süresi (duration) ve son teslim tarihi (deadline) olan $n$ görevimiz var. Her görev tamamlandığında $d - x$ puan kazanırız ($d$ görevin deadline'ı, $x$ ise görevin bittiği zaman anıdır). Tüm görevleri tamamlayarak elde edebileceğimiz **maksimum** toplam skoru bulmamız istenmektedir.

**Açgözlü Strateji:**  
Tarihlerden (deadline) bağımsız olarak, görevleri sadece **sürelerine göre artan (küçükten büyüğe) sırada** gerçekleştirmek her zaman optimaldir.

**Neden çalışır? (Exchange Argument):**  
Ardışık iki görev düşünelim: Süresi $a$ olan $X$ görevi ve süresi $b$ olan $Y$ görevi. Farz edelim ki $a > b$ olsun ve biz önce $X$'i, sonra $Y$'yi yapıyoruz.
- Eğer bu ikisinin sırasını değiştirip önce $Y$ sonra $X$'i yaparsak:
  - $Y$ görevi $a$ birim zaman daha erken biter (bize $+a$ puan kazandırır).
  - $X$ görevi ise $b$ birim zaman daha geç biter (bize $-b$ puan kaybettirir).
  - Net kazanç değişimi: $a - b$ olur. $a > b$ olduğu için bu değer kesinlikle pozitiftir ($a - b > 0$).
  - Dolayısıyla, her zaman daha kısa olan görevi öne almak skoru artırır.

**C++ Çözümü:**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Task {
    int duration;
    int deadline;
};

// Görevleri sürelerine göre artan sırada sıralayan karşılaştırıcı
bool compareTasks(const Task& a, const Task& b) {
    return a.duration < b.duration;
}

long long maxScore(vector<Task>& tasks) {
    sort(tasks.begin(), tasks.end(), compareTasks);
    
    long long current_time = 0;
    long long total_score = 0;
    
    for (const auto& task : tasks) {
        current_time += task.duration;
        total_score += (task.deadline - current_time);
    }
    return total_score;
}
```

---

### 16.4 Minimizing Sums (Toplamları Minimize Etme)

**Problem Tanımı:**  
Bize $n$ adet $a_1, a_2, ..., a_n$ sayıları verilmiştir. Öyle bir $x$ değeri bulmalıyız ki aşağıdaki toplam en küçük (minimum) olsun:
$$\sum_{i=1}^{n} |a_i - x|^c$$
Özellikle $c = 1$ ve $c = 2$ durumları mülakatlarda ve yarışmalarda sıkça sorulur.

#### Durum $c = 1$: Mutlak Farkların Toplamı ($\sum |a_i - x|$)
- **Optimal Seçim:** $x$, verilen sayıların **medyanı** (sıralandıktan sonra ortadaki sayı) olmalıdır.
- **Açıklama:** Eğer $x$ medyan değerinden daha küçük seçilirse, $x$'i artırmak toplam mesafeyi azaltır; medyan değerinden büyük seçilirse azaltmak mesafeyi düşürür. Eleman sayısı çift ise, ortadaki iki medyan sayı ve bu iki sayının arasındaki tüm reel sayılar optimal çözümdür.

#### Durum $c = 2$: Karelerin Farkı Toplamı ($\sum (a_i - x)^2$)
- **Optimal Seçim:** $x$, verilen sayıların **aritmetik ortalaması** olmalıdır.
- **Açıklama:** Bu fonksiyonu açtığımızda $n x^2 - 2 x \sum a_i + \sum a_i^2$ elde ederiz. Bu ifade yukarı doğru açılan bir paraboldür. Türevini alıp sıfıra eşitlediğimizde $2n x - 2 \sum a_i = 0 \implies x = \frac{\sum a_i}{n}$ (ortalama) sonucuna ulaşırız.

**C++ Çözümü:**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

// c = 1 için optimal x değerini (Medyan) döner
double getOptimalX_C1(vector<int> a) {
    sort(a.begin(), a.end());
    int n = a.size();
    if (n % 2 == 1) {
        return a[n / 2];
    } else {
        // Çift sayıda eleman varsa ortadaki iki elemanın ortalamasını dönebiliriz
        return (a[n / 2 - 1] + a[n / 2]) / 2.0;
    }
}

// c = 2 için optimal x değerini (Aritmetik Ortalama) döner
double getOptimalX_C2(const vector<int>& a) {
    if (a.empty()) return 0;
    double sum = accumulate(a.begin(), a.end(), 0.0);
    return sum / a.size();
}
```

---

### 16.5 Huffman Coding (Huffman Kodlaması)

**Problem Tanımı:**  
Bir metni (string) sıkıştırmak istiyoruz. Sabit uzunlukta kodlama yerine (her karaktere eşit bit uzunluğu, örn. ASCII'de 8 bit), değişken uzunluklu kodlama (sık geçen karakterlere kısa bit dizisi, nadir geçenlere uzun bit dizisi) kullanarak bit boyutunu minimize etmek istiyoruz. Kodlamanın geçerli olabilmesi için hiçbir karakterin kodu bir diğerinin **öneki (prefix)** olmamalıdır (bu sayede geri dönüştürme benzersiz yapılabilir).

**Algoritma Adımları (Greedy Yaklaşım):**
1. Metindeki her benzersiz karakter için frekans sayılır ve birer yaprak düğüm oluşturulur.
2. Bu düğümler frekanslarına (ağırlıklarına) göre bir min-heap'e (öncelik kuyruğu) atılır.
3. Kuyrukta tek bir düğüm kalana kadar şu işlem tekrarlanır:
   - En küçük ağırlığa sahip iki düğüm çekilir.
   - Bu iki düğümü sol ve sağ çocuk yapan yeni bir iç düğüm (internal node) oluşturulur. Bu iç düğümün ağırlığı, iki çocuğun ağırlıkları toplamıdır.
   - Yeni düğüm tekrar min-heap'e eklenir.
4. Ağaç tamamlandığında, kökten yapraklara giden yollar takip edilerek kodlar çıkarılır: Sol dal `0`, sağ dal `1` bitini temsil eder.

**C++ Çözümü:**
```cpp
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode *left, *right;
    
    HuffmanNode(char c, int f) {
        ch = c;
        freq = f;
        left = right = nullptr;
    }
};

// Öncelik kuyruğunda küçük frekanslı elemanın üstte olması için karşılaştırıcı
struct Compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->freq > r->freq;
    }
};

// Huffman ağacını derinlemesine gezerek (DFS) her karaktere atanan bit dizisini bulma
void buildCodes(HuffmanNode* root, string code, unordered_map<char, string>& huffmanCodes) {
    if (!root) return;
    
    // Yaprak düğüm ise karakteri ve oluşturulan kodu haritaya ekle
    if (!root->left && !root->right) {
        huffmanCodes[root->ch] = code;
    }
    
    buildCodes(root->left, code + "0", huffmanCodes);
    buildCodes(root->right, code + "1", huffmanCodes);
}

// Huffman ağacını kuran ana fonksiyon
unordered_map<char, string> getHuffmanCodes(string text) {
    unordered_map<char, int> freq;
    for (char ch : text) {
        freq[ch]++;
    }
    
    // Min-heap oluşturuluyor
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;
    for (auto pair : freq) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }
    
    // Ağacı oluştur
    while (pq.size() > 1) {
        HuffmanNode *left = pq.top(); pq.pop();
        HuffmanNode *right = pq.top(); pq.pop();
        
        // Ara düğümün karakterine özel bir sembol atıyoruz ('$')
        HuffmanNode *parent = new HuffmanNode('$', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }
    
    unordered_map<char, string> huffmanCodes;
    if (!pq.empty()) {
        buildCodes(pq.top(), "", huffmanCodes);
    }
    return huffmanCodes;
}
```

---
---

## 17. Dynamic Programming (Chapter 7 - Kitap Çözümleri & Detaylı Açıklamalar)

Dinamik Programlama (DP), kaba kuvvet (brute force) algoritmalarının doğruluğunu, açgözlü (greedy) algoritmaların hızıyla birleştiren güçlü bir tekniktir. Bir problemin DP ile çözülebilmesi için **overlapping subproblems** (üst üste binen/yineleyen alt problemler) ve **optimal substructure** (optimal alt yapı - alt problemlerin optimal çözümlerinin tüm problemin optimal çözümünü oluşturması) özelliklerine sahip olması gerekir.

DP iki temel amaçla kullanılır:
1. **En iyi çözümü bulmak (Optimization):** Minimum veya maksimum değeri aramak.
2. **Çözüm sayısını saymak (Counting):** Olası tüm geçerli yolların/durumların sayısını hesaplamak.

---

### 17.1 Coin Problem (Bozuk Para - Genel Durum & Dinamik Programlama)

**Problem Tanımı:**  
$coins = \{c_1, c_2, ..., c_k\}$ bozuk para seti ve hedef $n$ değeri veriliyor. Hedef toplamı oluşturmak için gereken **minimum** para sayısını bulmamız ve seçilen paraları listelememiz isteniyor.

**Rekürsif Formülasyon:**  
$solve(x)$ değeri, $x$ toplamını oluşturmak için gereken minimum para sayısı olsun. İlk seçeceğimiz paranın $c \in coins$ olduğunu varsayarsak, geriye kalan $x - c$ miktarını en az parayla oluşturmamız gerekir:
$$solve(x) = \min_{c \in coins} (solve(x - c) + 1)$$
**Taban Durumlar:**
- $solve(0) = 0$ (0 birim parayı 0 adet madeni para ile yapabiliriz).
- $solve(x) = \infty$ ($x < 0$ ise, negatif bir toplamı oluşturmak imkansızdır).

**Çözüm İnşası (Reconstruction):**  
Optimal toplamı elde ederken hangi paraları seçtiğimizi bulmak için `first[x]` dizisinde, $x$ toplamını veren optimal geçişteki ilk parayı ($c$) saklarız.

**C++ Çözümü (Tabulation - Bottom-up):**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 1e9;

// Minimum madeni para sayısını bulur ve kullanılan paraları ekrana yazdırır
void solveCoinChangeMin(int n, const vector<int>& coins) {
    // value[x] = x miktarını oluşturmak için gereken minimum para sayısı
    vector<int> value(n + 1, INF);
    // first[x] = x miktarını veren optimal seçimdeki ilk para değeri
    vector<int> first(n + 1, 0);
    
    value[0] = 0; // Taban durum
    
    for (int x = 1; x <= n; x++) {
        for (int c : coins) {
            if (x - c >= 0 && value[x - c] + 1 < value[x]) {
                value[x] = value[x - c] + 1;
                first[x] = c; // Optimal geçişi kaydet
            }
        }
    }
    
    if (value[n] == INF) {
        cout << "Hedef miktar " << n << " olusturulamaz.\n";
        return;
    }
    
    cout << "Minimum para sayisi: " << value[n] << "\n";
    cout << "Secilen paralar: ";
    int temp = n;
    while (temp > 0) {
        cout << first[temp] << " ";
        temp -= first[temp];
    }
    cout << "\n";
}
```

---

### 17.2 Counting the Number of Solutions (Çözüm Sayısı Sayma)

**Problem Tanımı:**  
Elimizdeki bozuk paralarla, sırasına önem vererek (örneğin $\{1,3,4\}$ setiyle $5$ toplamı için $1+4$ ile $4+1$ farklı sayılır) hedef $n$ toplamını kaç farklı şekilde oluşturabileceğimizi bulmak.

**Rekürsif Formülasyon:**  
$solve(x)$ değeri, $x$ toplamını oluşturmanın farklı yollarının sayısı olsun.
$$solve(x) = \sum_{c \in coins} solve(x - c)$$
**Taban Durumlar:**
- $solve(0) = 1$ (Hiçbir para seçmemek 1 geçerli yoldur).
- $solve(x) = 0$ ($x < 0$ ise, negatif toplamlar için çözüm yoktur).

Büyük çıktılarda taşmayı önlemek için genelde sonuç $10^9+7$ modunda istenir.

**C++ Çözümü:**
```cpp
#include <iostream>
#include <vector>

using namespace std;

int countWays(int n, const vector<int>& coins) {
    int MOD = 1e9 + 7;
    // count[x] = x toplamını oluşturmanın yol sayısı
    vector<int> count(n + 1, 0);
    count[0] = 1; // Taban durum
    
    for (int x = 1; x <= n; x++) {
        for (int c : coins) {
            if (x - c >= 0) {
                count[x] = (count[x] + count[x - c]) % MOD;
            }
        }
    }
    return count[n];
}
```

---

### 17.3 Longest Increasing Subsequence (LIS - En Uzun Artan Altdizi)

**Problem Tanımı:**  
$n$ elemanlı bir dizide, soldan sağa doğru giden ve elemanlarının değerleri kesinlikle artan en uzun alt dizinin boyutunu bulmak.  
*Örnek:* `[6, 2, 5, 1, 7, 4, 8, 3]` dizisi için en uzun artan alt dizi `[2, 5, 7, 8]` olup uzunluğu $4$'tür.

#### Yaklaşım 1: Klasik $O(n^2)$ DP Çözümü
- **Tanım:** `length[k]`, $k$ indeksinde biten en uzun artan alt dizinin boyutu olsun.
- **Geçiş:** Kendinden önceki daha küçük elemanların bittiği LIS uzunluklarına bakıp en büyüğüne 1 ekler:
  $$length[k] = \max(1, \max_{i < k, a[i] < a[k]} (length[i] + 1))$$

#### Yaklaşım 2: Optimize $O(n \log n)$ Çözümü (Binary Search / Sabır Sıralaması)
- **Fikir:** `tails[i]` adında dinamik bir dizi tutarız. `tails[i]`, şu ana kadar uzunluğu $i+1$ olan tüm artan alt dizilerin bittiği en küçük son elemanı saklar.
- Bu `tails` dizisi her zaman sıralı kalır. Gelen her yeni eleman için `tails` dizisinde kendisinden büyük ya da eşit olan ilk elemanı ikili arama (`lower_bound`) ile buluruz:
  - Eğer böyle bir eleman varsa, o elemanı yenisiyle güncelleriz (böylece o uzunluktaki alt dizinin sonunu daha küçük ve avantajlı bir sayı yapmış oluruz).
  - Eğer yoksa, yeni sayıyı `tails` dizisinin sonuna ekleriz (yeni bir uzunluk rekoru kırıldı demektir).
- En sonunda `tails` dizisinin boyutu bize LIS uzunluğunu verir.

**C++ Çözümü (Her iki yaklaşım da dahil):**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// O(n^2) DP Yaklaşımı
int lisO_N2(const vector<int>& a) {
    int n = a.size();
    if (n == 0) return 0;
    vector<int> length(n, 1);
    int max_lis = 1;
    
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < k; i++) {
            if (a[i] < a[k]) {
                length[k] = max(length[k], length[i] + 1);
            }
        }
        max_lis = max(max_lis, length[k]);
    }
    return max_lis;
}

// O(n log n) İkili Arama Optimize Yaklaşım
int lisO_NLogN(const vector<int>& a) {
    vector<int> tails;
    for (int x : a) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) {
            tails.push_back(x); // Yeni maksimum uzunluk
        } else {
            *it = x; // Mevcut LIS kuyruğunu daha küçük bir sayı ile optimize et
        }
    }
    return tails.size();
}
```

---

### 17.4 Paths in a Grid (Izgara Üzerinde Yol)

**Problem Tanımı:**  
$n \times n$ boyutlarında sayı dolu bir ızgara veriliyor. Sol üst köşeden $(1, 1)$ başlayıp sağ alt köşeye $(n, n)$ sadece aşağı ve sağa hareket ederek gidilebilecek en büyük toplamlı yolu bulmamız isteniyor.

**Rekürsif Formülasyon:**  
$sum(y, x)$, $(1, 1)$ noktasından $(y, x)$ noktasına giden bir yolun alabileceği maksimum toplam değer olsun. Herhangi bir $(y, x)$ hücresine sadece soldan $(y, x - 1)$ ya da yukarıdan $(y - 1, x)$ gelinebilir:
$$sum(y, x) = \max(sum(y, x - 1), sum(y - 1, x)) + value[y][x]$$
**Taban Durumlar:**  
Sınırların dışına çıkmayı engellemek için $y = 0$ veya $x = 0$ ise $sum(y, x) = 0$ olarak kabul edilir.

**C++ Çözümü:**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int maxPathSum(const vector<vector<int>>& grid) {
    int n = grid.size();
    if (n == 0) return 0;
    int m = grid[0].size();
    
    // Sınır kontrollerini kolaylaştırmak için 1-based index tablosu
    vector<vector<int>> sum(n + 1, vector<int>(m + 1, 0));
    
    for (int y = 1; y <= n; y++) {
        for (int x = 1; x <= m; x++) {
            // grid 0-indexed olduğundan y-1 ve x-1 hücrelerini alıyoruz
            sum[y][x] = max(sum[y][x - 1], sum[y - 1][x]) + grid[y - 1][x - 1];
        }
    }
    return sum[n][m];
}
```

---

### 17.5 Knapsack Problems (Sırt Çantası)

Sırt çantası problemleri, belirli kapasite kısıtlamaları altında en değerli nesne alt kümesini seçme mantığına dayanan temel DP kalıplarındandır.

#### Kısım 1: Ağırlıkların Kümesi ile Olası Toplamları Bulma (Subset Sum)
Bize $[w_1, w_2, ..., w_n]$ ağırlıkları veriliyor. Bu ağırlıklardan hangilerini toplayarak hangi toplam ağırlıkları elde edebiliriz?
- **Durum Tanımı:** $possible(x, k) = \text{true}$ (ilk $k$ ağırlığı kullanarak $x$ toplamını elde edebiliyorsak).
- **Geçiş:** $k$. ağırlığı ya kullanırız ya da kullanmayız:
  $$possible(x, k) = possible(x - w_k, k - 1) \lor possible(x, k - 1)$$
- **Bellek Optimizasyonu:** $O(n \cdot W)$ tablosu yerine tek boyutlu bir $possible$ dizisi açıp güncellemeyi **sağdan sola (tersten)** yaparsak, aynı elemanı aynı adımda iki kez kullanma hatasından (sınırsız kullanım) kaçınmış oluruz.

#### Kısım 2: Klasik 0/1 Sırt Çantası Problemi (Knapsack with Values)
Kapasitesi $W$ olan bir çantaya, ağırlıkları $w_i$ ve değerleri $v_i$ olan eşyalardan maksimum toplam değere sahip olanları seçerek koymak.
- **Geçiş:** Tersten güncellenen 1D dizi ile:
  $$dp[x] = \max(dp[x], dp[x - w_i] + v_i)$$

**C++ Çözümü:**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 1. Kısım: Subset Sum (Elde edilebilen toplam ağırlıklar)
vector<bool> getPossibleSums(const vector<int>& weights, int W) {
    vector<bool> possible(W + 1, false);
    possible[0] = true; // 0 ağırlığı her zaman elde edilebilir (hiçbir şey seçmeyerek)
    
    for (int w : weights) {
        // Çakışmayı önlemek için sağdan sola doğru güncelliyoruz
        for (int x = W; x >= w; x--) {
            if (possible[x - w]) {
                possible[x] = true;
            }
        }
    }
    return possible;
}

// 2. Kısım: Klasik 0/1 Sırt Çantası
int knapsack01(int W, const vector<int>& weights, const vector<int>& values) {
    int n = weights.size();
    // dp[x] = x ağırlığı ile elde edilebilecek maksimum değer sum'ı
    vector<int> dp(W + 1, 0);
    
    for (int i = 0; i < n; i++) {
        int w = weights[i];
        int v = values[i];
        for (int x = W; x >= w; x--) {
            dp[x] = max(dp[x], dp[x - w] + v);
        }
    }
    return dp[W];
}
```

---

### 17.6 Edit Distance (Levenshtein/Düzenleme Mesafesi)

**Problem Tanımı:**  
İki string ($x$ ve $y$) veriliyor. $x$'i $y$'ye dönüştürmek için yapılması gereken **minimum** düzenleme (karakter ekleme, karakter silme, karakter değiştirme) işlem sayısını bulmak.  
*Örnek:* LOVE ile MOVIE arasındaki mesafe $2$'dir (LOVE $\rightarrow$ MOVE (değiştirme), MOVE $\rightarrow$ MOVIE (ekleme)).

**Rekürsif Formülasyon:**  
$distance(a, b)$ değeri, $x$'in $a$ boyundaki öneki ile $y$'nin $b$ boyundaki öneki arasındaki mesafe olsun.
$$distance(a, b) = \min \begin{cases} 
distance(a, b - 1) + 1 & \text{(karakter ekleme)} \\ 
distance(a - 1, b) + 1 & \text{(karakter silme)} \\ 
distance(a - 1, b - 1) + cost(a, b) & \text{(karakter eşleştirme/değiştirme)} 
\end{cases}$$
Burada $cost(a, b) = 0$ (eğer $x[a] == y[b]$ ise), aksi halde $cost(a, b) = 1$ olur.

**C++ Çözümü:**
```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int editDistance(const string& x, const string& y) {
    int n = x.length();
    int m = y.length();
    
    // dp[i][j] = x[0..i-1] ile y[0..j-1] arasındaki düzenleme mesafesi
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    
    // Taban durumlar (bir taraf boşsa, tüm karakterleri silmeli veya eklemeliyiz)
    for (int i = 0; i <= n; i++) dp[i][0] = i;
    for (int j = 0; j <= m; j++) dp[0][j] = j;
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            int cost = (x[i - 1] == y[j - 1]) ? 0 : 1;
            dp[i][j] = min({
                dp[i][j - 1] + 1,       // Ekleme
                dp[i - 1][j] + 1,       // Silme
                dp[i - 1][j - 1] + cost  // Değiştirme / Eşleşme
            });
        }
    }
    return dp[n][m];
}
```

---

### 17.7 Counting Tilings (Izgara Kaplama / Profil/Bitmask DP)

**Problem Tanımı:**  
$n \times m$ boyutlarındaki boş bir ızgarayı, dikey ya da yatay yerleştirilebilen $1 \times 2$ ve $2 \times 1$ boyutundaki dominolarla boşluk kalmayacak ve taşmayacak şekilde kaplamanın toplam kaç farklı yolu olduğunu bulmak.

**Çözüm Yöntemi (Bitmask DP):**  
Izgarayı satır satır doldururuz. Herhangi bir satırın durumu, bir önceki satırdan dikey domino taşması olup olmadığını belirten $m$ bitlik bir sayı (bitmask) ile temsil edilir.
- `0`: Dikey taşma yok, boş.
- `1`: Üst satırdan gelen dikey domino burayı kaplamış.

**Zaman Karmaşıklığı:**  
$O(n \cdot 2^m)$ zaman karmaşıklığı vardır. Burada m, ızgaranın daha kısa olan kenarı olmalıdır (değilse döndürülür).

**C++ Çözümü:**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MOD = 1e9 + 7;
int grid_n, grid_m;
vector<vector<int>> dp_tiling;

// Rekürsif maske geçişlerini üreten yardımcı fonksiyon
void generateMasks(int col, int current_mask, int next_mask, vector<int>& next_states) {
    if (col == grid_m) {
        next_states.push_back(next_mask);
        return;
    }
    
    // Eğer mevcut hücre zaten üst satırdan gelen dikey taşma ile doluysa
    if (current_mask & (1 << col)) {
        // Bu sütunu boş geçiyoruz (next_mask'te bit sıfır kalır)
        generateMasks(col + 1, current_mask, next_mask, next_states);
    } else {
        // 1. Dikey taş yerleştir (alt satıra taşma yapar, next_mask'te biti 1 yapar)
        generateMasks(col + 1, current_mask, next_mask | (1 << col), next_states);
        
        // 2. Yatay taş yerleştir (bu hücre ve yanındaki hücre boş olmalı)
        if (col + 1 < grid_m && !(current_mask & (1 << (col + 1)))) {
            // İki sütun birden kapandığı için col+2'ye atlar
            generateMasks(col + 2, current_mask, next_mask, next_states);
        }
    }
}

int countTilingWays(int n, int m) {
    // Profil bitmask boyutunu küçük tutmak için kısa kenarı m yapıyoruz
    if (n < m) swap(n, m);
    
    grid_n = n;
    grid_m = m;
    
    // dp_tiling[r][mask] = r. satırın profili 'mask' iken ilk r satırın kaplanma yollarının sayısı
    dp_tiling.assign(n + 1, vector<int>(1 << m, 0));
    dp_tiling[0][0] = 1; // Başlangıç taban durumu
    
    for (int r = 0; r < n; r++) {
        for (int mask = 0; mask < (1 << m); mask++) {
            if (dp_tiling[r][mask] == 0) continue;
            
            vector<int> next_states;
            generateMasks(0, mask, 0, next_states);
            
            for (int next_mask : next_states) {
                dp_tiling[r + 1][next_mask] = (dp_tiling[r + 1][next_mask] + dp_tiling[r][mask]) % MOD;
            }
        }
    }
    // Son satırda alt satıra dikey taşma olamayacağından profilin 0 olması gerekir
    return dp_tiling[n][0];
}
```

---
