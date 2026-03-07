# Data Structures & Algorithms (DSA) – Complete Reference

Bu dosya **tüm temel veri yapıları ve algoritmalar** için C++ ve Python implementasyonlarını, açıklamalarını ve karmaşıklık analizlerini içerir. Her veri yapısı için **ekleme (insert), silme (delete), arama (search), yazdırma (print)** işlemleri verilmiştir. Graf bölümünde hem **Adjacency List** hem **Adjacency Matrix** implementasyonları ile **BFS** ve **DFS** algoritmaları sunulmuştur.

---

## 0. Types, Limits and Overflow (Ekstra)

Bu bölüm DSA çözerken özellikle **C++** için veri tipleri, overflow ve zaman karmaşıklığı limitleri hakkında hızlı bir referanstır.

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

```cpp
long long a = 1e9, b = 1e9;
cout << a * b; // Doğru, long long yeterli
```

### 0.2 Python

- `int` pratikte sınırsızdır (overflow yok), ama C++'a göre daha yavaştır.

### 0.3 N Büyüklüğüne Göre Kabul Edilen Karmaşıklık

| n üst sınırı | Güvenli zaman karmaşıklığı        |
|--------------|-----------------------------------|
| ≤ 10^4       | O(n^2)                           |
| ≤ 10^5       | O(n log n)                       |
| ≤ 10^6       | O(n)                             |
| ≤ 10^7       | O(n) (sınırda, çok optimize olmalı) |

---

## 1. Arrays (Diziler)

Contiguous block of memory storing fixed-size sequence of same-typed elements.

| Operation                 | Time        | Space |
|---------------------------|-------------|-------|
| Access by index           | O(1)        | O(n)  |
| Update by index           | O(1)        | O(n)  |
| Insert/delete at end      | O(1) amort. | O(n)  |
| Insert/delete at middle   | O(n)        | O(n)  |
| Search (unsorted)         | O(n)        | O(1)  |

**Nerede kullanılır?** Rastgele erişimin çok önemli olduğu ve dizinin boyutunun nispeten sabit kaldığı durumlarda.
**Nerede kullanma?** Ortaya sık sık eleman ekleyip sildiğin durumlarda; her seferinde kaydırma maliyeti O(n) olur.

### 1.1 STL vector Kullanımı (C++)

```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    vector<int> a = {1, 2, 3};
    a.push_back(4);          // O(1) amortized - sona ekleme
    a.pop_back();             // O(1) - sondan silme
    int x = a[2];            // O(1) - random access
    auto it = find(a.begin(), a.end(), 3); // O(n) - linear search
    if (it != a.end()) cout << "Bulundu: " << *it << "\n";

    // Yazdırma
    for (int val : a) cout << val << " ";
    cout << "\n";
    return 0;
}
```

### 1.2 Python list

```python
arr = [1, 2, 3]
arr.append(4)      # amortized O(1) - sona ekleme
arr.pop()           # O(1) - sondan silme
print(arr[2])       # O(1) - random access
print(3 in arr)     # O(n) - linear search
print(arr)          # yazdırma
```

### 1.3 Array-Based List (Custom Class – C++)

Insert, delete, search, print işlemleri ile birlikte:

```cpp
#include <bits/stdc++.h>
using namespace std;

class ArrayList {
private:
    int *data;
    int capacity;
    int length;

    void resize(int newCap) {
        int *newData = new int[newCap];
        for (int i = 0; i < length; ++i) newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = newCap;
    }

public:
    ArrayList(int cap = 4) : capacity(cap), length(0), data(new int[cap]) {}
    ~ArrayList() { delete[] data; }

    int size() const { return length; }
    bool empty() const { return length == 0; }

    // ACCESS
    int operator[](int index) const {
        if (index < 0 || index >= length) throw out_of_range("Index");
        return data[index];
    }

    // INSERT - sona
    void push_back(int value) {
        if (length == capacity) resize(capacity * 2);
        data[length++] = value;
    }

    // INSERT - belirli indekse
    void insert(int index, int value) {
        if (index < 0 || index > length) throw out_of_range("Index");
        if (length == capacity) resize(capacity * 2);
        for (int i = length; i > index; --i) data[i] = data[i - 1];
        data[index] = value;
        ++length;
    }

    // DELETE - belirli indeksten
    void erase(int index) {
        if (index < 0 || index >= length) throw out_of_range("Index");
        for (int i = index; i < length - 1; ++i) data[i] = data[i + 1];
        --length;
    }

    // SEARCH - değere göre arama
    int search(int value) const {
        for (int i = 0; i < length; ++i)
            if (data[i] == value) return i;
        return -1;
    }

    // PRINT
    void print() const {
        for (int i = 0; i < length; ++i) cout << data[i] << " ";
        cout << "\n";
    }
};
```

### 1.4 C++ `vector` Fonksiyon Özeti

```cpp
vector<int> v;
v.push_back(x);   // sona eleman ekler - O(1) amort.
v.pop_back();      // son elemanı siler - O(1)
v.size();          // eleman sayısını döndürür
v.empty();         // boş mu kontrol eder
v.clear();         // tüm elemanları siler
v[i];              // i. indekse O(1) erişim
v.at(i);           // güvenli erişim (sınır kontrolü yapar)
```

---

## 2. Linked Lists (Bağlı Listeler)

### 2.1 Singly Linked List (Tek Yönlü Bağlı Liste)

Her `Node` sadece `next` pointer'ına sahiptir.

| Operation                 | Time | Space |
|---------------------------|------|-------|
| Access k-th               | O(n) | O(n)  |
| Insert/delete at head     | O(1) | O(1)  |
| Insert/delete after node  | O(1) | O(1)  |
| Search                    | O(n) | O(1)  |

**Nerede kullanılır?** Listenin başında/ortasında çok sık ekleme-silme yaptığın, ama rastgele indeks erişimine ihtiyaç duymadığın durumlarda.
**Nerede kullanma?** Elemanlara sık sık `k`'ıncı indeks üzerinden erişmen gerekiyorsa; her erişim O(n) olur.

**C++ – Insert, Delete, Search, Print**

```cpp
struct SinglyNode {
    int value;
    SinglyNode *next;
    SinglyNode(int v) : value(v), next(nullptr) {}
};

class SinglyLinkedList {
private:
    SinglyNode *head;

public:
    SinglyLinkedList() : head(nullptr) {}

    ~SinglyLinkedList() {
        SinglyNode *cur = head;
        while (cur) {
            SinglyNode *tmp = cur->next;
            delete cur;
            cur = tmp;
        }
    }

    // INSERT - başa
    void push_front(int v) {
        SinglyNode *node = new SinglyNode(v);
        node->next = head;
        head = node;
    }

    // INSERT - sona
    void push_back(int v) {
        SinglyNode *node = new SinglyNode(v);
        if (!head) { head = node; return; }
        SinglyNode *cur = head;
        while (cur->next) cur = cur->next;
        cur->next = node;
    }

    // DELETE - değere göre ilk eşleşeni sil
    bool remove(int v) {
        if (!head) return false;
        if (head->value == v) {
            SinglyNode *tmp = head;
            head = head->next;
            delete tmp;
            return true;
        }
        SinglyNode *cur = head;
        while (cur->next && cur->next->value != v) cur = cur->next;
        if (!cur->next) return false;
        SinglyNode *tmp = cur->next;
        cur->next = cur->next->next;
        delete tmp;
        return true;
    }

    // SEARCH - değere göre arama
    bool search(int v) const {
        SinglyNode *cur = head;
        while (cur) {
            if (cur->value == v) return true;
            cur = cur->next;
        }
        return false;
    }

    // PRINT
    void print() const {
        SinglyNode *cur = head;
        while (cur) {
            cout << cur->value << " -> ";
            cur = cur->next;
        }
        cout << "NULL\n";
    }
};
```

**Python – Insert, Delete, Search, Print**

```python
class Node:
    def __init__(self, v: int):
        self.v = v
        self.next = None

class SinglyLinkedList:
    def __init__(self) -> None:
        self.head = None

    def push_front(self, v: int) -> None:
        n = Node(v)
        n.next = self.head
        self.head = n

    def push_back(self, v: int) -> None:
        n = Node(v)
        if not self.head:
            self.head = n
            return
        cur = self.head
        while cur.next:
            cur = cur.next
        cur.next = n

    def remove(self, v: int) -> bool:
        if not self.head:
            return False
        if self.head.v == v:
            self.head = self.head.next
            return True
        cur = self.head
        while cur.next and cur.next.v != v:
            cur = cur.next
        if not cur.next:
            return False
        cur.next = cur.next.next
        return True

    def search(self, v: int) -> bool:
        cur = self.head
        while cur:
            if cur.v == v:
                return True
            cur = cur.next
        return False

    def print_list(self) -> None:
        cur = self.head
        while cur:
            print(cur.v, end=" -> ")
            cur = cur.next
        print("NULL")
```

### 2.2 Doubly Linked List (Çift Yönlü Bağlı Liste)

Her `Node` hem `prev` hem `next` tutar. Geriye doğru traversal mümkündür.

**C++ – Insert, Delete, Search, Print**

```cpp
struct DoublyNode {
    int value;
    DoublyNode *prev;
    DoublyNode *next;
    DoublyNode(int v) : value(v), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
private:
    DoublyNode *head;
    DoublyNode *tail;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    ~DoublyLinkedList() {
        DoublyNode *cur = head;
        while (cur) {
            DoublyNode *tmp = cur->next;
            delete cur;
            cur = tmp;
        }
    }

    // INSERT - başa
    void push_front(int v) {
        DoublyNode *node = new DoublyNode(v);
        node->next = head;
        if (head) head->prev = node;
        head = node;
        if (!tail) tail = node;
    }

    // INSERT - sona
    void push_back(int v) {
        DoublyNode *node = new DoublyNode(v);
        node->prev = tail;
        if (tail) tail->next = node;
        tail = node;
        if (!head) head = node;
    }

    // DELETE - baştan
    void pop_front() {
        if (!head) return;
        DoublyNode *tmp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete tmp;
    }

    // DELETE - sondan
    void pop_back() {
        if (!tail) return;
        DoublyNode *tmp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete tmp;
    }

    // DELETE - değere göre
    bool remove(int v) {
        DoublyNode *cur = head;
        while (cur) {
            if (cur->value == v) {
                if (cur->prev) cur->prev->next = cur->next;
                else head = cur->next;
                if (cur->next) cur->next->prev = cur->prev;
                else tail = cur->prev;
                delete cur;
                return true;
            }
            cur = cur->next;
        }
        return false;
    }

    // SEARCH
    bool search(int v) const {
        DoublyNode *cur = head;
        while (cur) {
            if (cur->value == v) return true;
            cur = cur->next;
        }
        return false;
    }

    // PRINT - ileri yönde
    void print_forward() const {
        DoublyNode *cur = head;
        while (cur) {
            cout << cur->value << " <-> ";
            cur = cur->next;
        }
        cout << "NULL\n";
    }

    // PRINT - geri yönde
    void print_backward() const {
        DoublyNode *cur = tail;
        while (cur) {
            cout << cur->value << " <-> ";
            cur = cur->prev;
        }
        cout << "NULL\n";
    }
};
```

STL ile hazır bağlı listeler: `std::list<int>` (doubly), `std::forward_list<int>` (singly).

```cpp
list<int> dl;
dl.push_front(2);
dl.push_back(3);
```

---

## 3. Stack – LIFO (Yığın)

**Nerede kullanılır?** İfade değerlendirme, geri alma (undo), DFS, parantez dengeleme, çağrı yığını benzeri LIFO durumlarında.
**Nerede kullanma?** Rastgele eleman erişimi veya kuyruğa benzer FIFO davranışı gerektiğinde.

| Operation | Time | Space |
|-----------|------|-------|
| push/pop  | O(1) | O(n)  |
| top/peek  | O(1) | O(1)  |

### 3.1 Array-Based Stack (C++)

```cpp
class ArrayStack {
private:
    vector<int> data;

public:
    // INSERT
    void push(int x) { data.push_back(x); }

    // DELETE
    void pop() {
        if (!data.empty()) data.pop_back();
    }

    // SEARCH / ACCESS (sadece tepe)
    int top() const {
        if (data.empty()) throw runtime_error("Empty stack");
        return data.back();
    }

    bool empty() const { return data.empty(); }
    int size() const { return (int)data.size(); }

    // PRINT
    void print() const {
        cout << "Stack (top -> bottom): ";
        for (int i = (int)data.size() - 1; i >= 0; --i)
            cout << data[i] << " ";
        cout << "\n";
    }
};
```

### 3.2 Linked-List Based Stack (C++)

```cpp
class LinkedStack {
private:
    SinglyNode *head;

public:
    LinkedStack() : head(nullptr) {}

    ~LinkedStack() {
        while (head) {
            SinglyNode *tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    void push(int x) {
        SinglyNode *node = new SinglyNode(x);
        node->next = head;
        head = node;
    }

    void pop() {
        if (!head) return;
        SinglyNode *tmp = head;
        head = head->next;
        delete tmp;
    }

    int top() const {
        if (!head) throw runtime_error("Empty stack");
        return head->value;
    }

    bool empty() const { return head == nullptr; }
};
```

### 3.3 STL `stack`

```cpp
stack<int> st;
st.push(1);    // tepeye eleman ekler
st.push(2);
int x = st.top(); // tepedeki elemanı okur (2)
st.pop();         // tepedeki elemanı siler
st.empty();       // boş mu kontrol
```

### 3.4 Python Stack

```python
st: list[int] = []
st.append(1)   # push
st.append(2)
x = st[-1]     # top/peek
x = st.pop()   # pop (siler ve döndürür)
```

---

## 4. Queue / Deque – FIFO (Kuyruk)

**Nerede kullanılır?** BFS, görev sıralama, üretici-tüketici kuyrukları, akış/buffer yapılarında.
**Nerede kullanma?** Ortadan rastgele silme/ekleme veya stack tarzı sadece sondan çalışma gerektiğinde.

| Structure | Operations (amort.) | Typical impl |
|-----------|---------------------|--------------|
| Queue     | enqueue/dequeue O(1)| array/list   |
| Deque     | push/pop both ends O(1) | circular buf |

### 4.1 Circular Array Queue (C++)

```cpp
class CircularQueue {
private:
    vector<int> data;
    int frontIdx, rearIdx, count;

public:
    CircularQueue(int cap) : data(cap), frontIdx(0), rearIdx(0), count(0) {}

    bool empty() const { return count == 0; }
    bool full() const { return count == (int)data.size(); }

    // INSERT
    void enqueue(int x) {
        if (full()) throw runtime_error("Full queue");
        data[rearIdx] = x;
        rearIdx = (rearIdx + 1) % data.size();
        ++count;
    }

    // DELETE
    void dequeue() {
        if (empty()) throw runtime_error("Empty queue");
        frontIdx = (frontIdx + 1) % data.size();
        --count;
    }

    // ACCESS
    int front() const {
        if (empty()) throw runtime_error("Empty queue");
        return data[frontIdx];
    }

    // PRINT
    void print() const {
        if (empty()) { cout << "Empty\n"; return; }
        cout << "Queue: ";
        for (int i = 0; i < count; ++i)
            cout << data[(frontIdx + i) % data.size()] << " ";
        cout << "\n";
    }
};
```

### 4.2 Linked-List Queue (C++)

```cpp
class LinkedQueue {
private:
    SinglyNode *head; // front
    SinglyNode *tail; // rear

public:
    LinkedQueue() : head(nullptr), tail(nullptr) {}

    ~LinkedQueue() {
        while (head) {
            SinglyNode *tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    bool empty() const { return head == nullptr; }

    // INSERT
    void enqueue(int x) {
        SinglyNode *node = new SinglyNode(x);
        if (!tail) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    // DELETE
    void dequeue() {
        if (!head) return;
        SinglyNode *tmp = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete tmp;
    }

    // ACCESS
    int front() const {
        if (!head) throw runtime_error("Empty queue");
        return head->value;
    }

    // PRINT
    void print() const {
        SinglyNode *cur = head;
        cout << "Queue: ";
        while (cur) {
            cout << cur->value << " ";
            cur = cur->next;
        }
        cout << "\n";
    }
};
```

### 4.3 STL `queue` ve `deque`

```cpp
// FIFO Queue
queue<int> q;
q.push(1); q.push(2);      // enqueue
int f = q.front();          // ön elemanı okur
q.pop();                    // ön elemanı siler

// Double-Ended Queue
deque<int> dq;
dq.push_front(1);   // başa ekle
dq.push_back(2);    // sona ekle
dq.pop_front();      // baştan sil
dq.pop_back();       // sondan sil
dq.front();          // ilk elemanı oku
dq.back();           // son elemanı oku
```

### 4.4 Python Queue

```python
from collections import deque
q = deque()
q.append(1)          # enqueue (sona ekle)
q.append(2)
f = q[0]             # front
q.popleft()          # dequeue (baştan sil)
```

---

## 5. Hash Table (Map / Set)

Average O(1) insert/find/erase; worst O(n).

**Nerede kullanılır?** Frekans sayımı, üyelik kontrolü, cache tutma, key→value eşleştirme gibi O(1) ortalama erişim gereken yerlerde.
**Nerede kullanma?** Elemanların **sırasına** ihtiyaç varsa; ağaç tabanlı yapılar daha uygun.

### 5.1 Hash Table – Separate Chaining (C++ Custom)

```cpp
class HashTable {
private:
    vector<list<pair<int, string>>> table;

    int hashFunc(int key) const {
        return ((key % (int)table.size()) + (int)table.size()) % (int)table.size();
    }

public:
    HashTable(int size = 10) : table(size) {}

    // INSERT / UPDATE
    void insert(int key, const string &value) {
        int idx = hashFunc(key);
        for (auto &p : table[idx]) {
            if (p.first == key) {
                p.second = value; // update
                return;
            }
        }
        table[idx].push_back({key, value});
    }

    // DELETE
    bool remove(int key) {
        int idx = hashFunc(key);
        for (auto it = table[idx].begin(); it != table[idx].end(); ++it) {
            if (it->first == key) {
                table[idx].erase(it);
                return true;
            }
        }
        return false;
    }

    // SEARCH
    string* search(int key) {
        int idx = hashFunc(key);
        for (auto &p : table[idx]) {
            if (p.first == key) return &p.second;
        }
        return nullptr;
    }

    // PRINT
    void print() const {
        for (int i = 0; i < (int)table.size(); ++i) {
            cout << "[" << i << "]: ";
            for (const auto &p : table[i])
                cout << "(" << p.first << "," << p.second << ") ";
            cout << "\n";
        }
    }
};
```

### 5.2 STL `unordered_map` ve `unordered_set`

```cpp
// unordered_map
unordered_map<string,int> freq;
for (string s : {"a","b","a"}) freq[s]++;   // insert/update
cout << freq["a"];                           // search → 2
freq.erase("b");                             // delete
cout << freq.count("b");                     // 0 (yok)

// unordered_set
unordered_set<int> s;
s.insert(5);       // insert
s.count(5);        // search → 1 (var)
s.erase(5);        // delete

// İterasyon
for (auto &[key, val] : freq) cout << key << ":" << val << " ";
```

### 5.3 Python dict / set / Counter

```python
from collections import Counter

# dict
freq = {}
for s in ["a", "b", "a"]:
    freq[s] = freq.get(s, 0) + 1
print(freq["a"])           # 2

# Counter
freq = Counter(["a", "b", "a"])
print(freq["a"])           # 2

# set
s = set()
s.add(5)                   # insert
print(5 in s)              # search → True
s.discard(5)               # delete
```

---

## 6. Trees & BST (Ağaçlar)

### 6.1 Binary Tree Basics

```cpp
struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
```

**Traversals (Dolaşma Yöntemleri):**

```cpp
// Inorder: Sol → Kök → Sağ (BST'de sıralı çıktı verir)
void inorder(TreeNode* root) {
    if (!root) return;
    inorder(root->left);
    cout << root->val << " ";
    inorder(root->right);
}

// Preorder: Kök → Sol → Sağ
void preorder(TreeNode* root) {
    if (!root) return;
    cout << root->val << " ";
    preorder(root->left);
    preorder(root->right);
}

// Postorder: Sol → Sağ → Kök
void postorder(TreeNode* root) {
    if (!root) return;
    postorder(root->left);
    postorder(root->right);
    cout << root->val << " ";
}

// Level-order (BFS): Seviye seviye
void levelOrder(TreeNode* root) {
    if (!root) return;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* cur = q.front(); q.pop();
        cout << cur->val << " ";
        if (cur->left) q.push(cur->left);
        if (cur->right) q.push(cur->right);
    }
    cout << "\n";
}
```

**Python traversals:**

```python
class TreeNode:
    def __init__(self, v: int):
        self.v = v
        self.l = None
        self.r = None

def inorder(root: TreeNode) -> None:
    if not root: return
    inorder(root.l)
    print(root.v, end=" ")
    inorder(root.r)

def preorder(root: TreeNode) -> None:
    if not root: return
    print(root.v, end=" ")
    preorder(root.l)
    preorder(root.r)

def postorder(root: TreeNode) -> None:
    if not root: return
    postorder(root.l)
    postorder(root.r)
    print(root.v, end=" ")
```

### 6.2 Binary Search Tree (BST) – Tam Implementasyon

Property: left < node < right.

| Operation | Avg      | Worst (skewed) |
|-----------|----------|----------------|
| search    | O(log n) | O(n)           |
| insert    | O(log n) | O(n)           |
| delete    | O(log n) | O(n)           |

**Nerede kullanılır?** Sıralı veriler üzerinde arama, ekleme ve silme yaparken hem düzen hem de logaritmik zaman karmaşıklığı istediğinde.
**Nerede kullanma?** Sadece üyelik kontrolleri yapıyorsan ve sıraya ihtiyacın yoksa; hash tabloları genelde daha basit ve hızlıdır.

**C++ – Insert, Delete, Search, Print (tam class)**

```cpp
class BinarySearchTree {
private:
    TreeNode *root;

    TreeNode* insert(TreeNode *node, int key) {
        if (!node) return new TreeNode(key);
        if (key < node->val) node->left = insert(node->left, key);
        else if (key > node->val) node->right = insert(node->right, key);
        return node;
    }

    bool search(TreeNode *node, int key) const {
        if (!node) return false;
        if (node->val == key) return true;
        if (key < node->val) return search(node->left, key);
        return search(node->right, key);
    }

    TreeNode* findMin(TreeNode *node) {
        while (node && node->left) node = node->left;
        return node;
    }

    TreeNode* remove(TreeNode *node, int key) {
        if (!node) return nullptr;
        if (key < node->val) {
            node->left = remove(node->left, key);
        } else if (key > node->val) {
            node->right = remove(node->right, key);
        } else {
            // Düğüm bulundu – 3 durum
            if (!node->left) {
                TreeNode *r = node->right;
                delete node;
                return r;
            } else if (!node->right) {
                TreeNode *l = node->left;
                delete node;
                return l;
            } else {
                // İki çocuklu: inorder successor ile değiştir
                TreeNode *succ = findMin(node->right);
                node->val = succ->val;
                node->right = remove(node->right, succ->val);
            }
        }
        return node;
    }

    void inorderPrint(TreeNode *node) const {
        if (!node) return;
        inorderPrint(node->left);
        cout << node->val << " ";
        inorderPrint(node->right);
    }

    void clear(TreeNode *node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() { clear(root); }

    void insert(int key) { root = insert(root, key); }
    bool search(int key) const { return search(root, key); }
    void remove(int key) { root = remove(root, key); }

    void print() const {
        inorderPrint(root);
        cout << "\n";
    }
};
```

**Python BST:**

```python
class BSTNode:
    def __init__(self, key: int):
        self.key = key
        self.left = None
        self.right = None

class BST:
    def __init__(self):
        self.root = None

    def insert(self, key: int) -> None:
        self.root = self._insert(self.root, key)

    def _insert(self, node, key):
        if not node: return BSTNode(key)
        if key < node.key: node.left = self._insert(node.left, key)
        elif key > node.key: node.right = self._insert(node.right, key)
        return node

    def search(self, key: int) -> bool:
        return self._search(self.root, key)

    def _search(self, node, key):
        if not node: return False
        if node.key == key: return True
        if key < node.key: return self._search(node.left, key)
        return self._search(node.right, key)

    def remove(self, key: int) -> None:
        self.root = self._remove(self.root, key)

    def _remove(self, node, key):
        if not node: return None
        if key < node.key:
            node.left = self._remove(node.left, key)
        elif key > node.key:
            node.right = self._remove(node.right, key)
        else:
            if not node.left: return node.right
            if not node.right: return node.left
            succ = node.right
            while succ.left: succ = succ.left
            node.key = succ.key
            node.right = self._remove(node.right, succ.key)
        return node

    def print_inorder(self) -> None:
        self._inorder(self.root)
        print()

    def _inorder(self, node):
        if not node: return
        self._inorder(node.left)
        print(node.key, end=" ")
        self._inorder(node.right)
```

---

## 7. Heaps & Priority Queue (Yığınlar)

Binary heap: complete tree with heap property (min-heap veya max-heap).

| Operation       | Time     |
|----------------|----------|
| push / pop top | O(log n) |
| peek top       | O(1)     |
| build heap     | O(n)     |

### 7.1 Manual MinHeap Implementation (C++)

```cpp
class MinHeap {
private:
    vector<int> data;

    void siftUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (data[parent] > data[i]) {
                swap(data[parent], data[i]);
                i = parent;
            } else break;
        }
    }

    void siftDown(int i) {
        int n = (int)data.size();
        while (2 * i + 1 < n) {
            int child = 2 * i + 1;
            if (child + 1 < n && data[child + 1] < data[child]) ++child;
            if (data[i] > data[child]) {
                swap(data[i], data[child]);
                i = child;
            } else break;
        }
    }

public:
    // INSERT
    void push(int x) {
        data.push_back(x);
        siftUp((int)data.size() - 1);
    }

    // DELETE (min elemanı sil)
    void pop() {
        if (data.empty()) return;
        data[0] = data.back();
        data.pop_back();
        if (!data.empty()) siftDown(0);
    }

    // ACCESS (min elemanı oku)
    int top() const {
        if (data.empty()) throw runtime_error("Empty heap");
        return data[0];
    }

    bool empty() const { return data.empty(); }
    int size() const { return (int)data.size(); }

    // PRINT
    void print() const {
        for (int x : data) cout << x << " ";
        cout << "\n";
    }
};
```

### 7.2 STL `priority_queue`

```cpp
// Max-heap (varsayılan)
priority_queue<int> maxpq;
maxpq.push(5); maxpq.push(3); maxpq.push(7);
cout << maxpq.top(); // 7
maxpq.pop();

// Min-heap
priority_queue<int, vector<int>, greater<int>> minpq;
for (int x : {5, 3, 7}) minpq.push(x);
while (!minpq.empty()) {
    cout << minpq.top() << " "; // 3 5 7
    minpq.pop();
}
```

### 7.3 Python `heapq`

```python
import heapq
h = []
for x in [5, 3, 7]:
    heapq.heappush(h, x)     # insert
while h:
    print(heapq.heappop(h), end=" ")  # delete min: 3 5 7
```

---

## 8. Trie (Prefix Tree)

Efficient prefix queries on strings.

### 8.1 C++ Trie – Insert, Search, Delete, Print

```cpp
struct TrieNode {
    TrieNode* children[26];
    bool isEnd;
    TrieNode() : isEnd(false) {
        memset(children, 0, sizeof(children));
    }
};

class Trie {
private:
    TrieNode* root;

    void clear(TrieNode* node) {
        if (!node) return;
        for (int i = 0; i < 26; ++i) clear(node->children[i]);
        delete node;
    }

    bool deleteHelper(TrieNode* node, const string& word, int depth) {
        if (!node) return false;
        if (depth == (int)word.size()) {
            if (!node->isEnd) return false;
            node->isEnd = false;
            for (int i = 0; i < 26; ++i)
                if (node->children[i]) return false;
            return true; // bu düğüm silinebilir
        }
        int idx = word[depth] - 'a';
        if (!node->children[idx]) return false;
        bool shouldDelete = deleteHelper(node->children[idx], word, depth + 1);
        if (shouldDelete) {
            delete node->children[idx];
            node->children[idx] = nullptr;
            if (!node->isEnd) {
                for (int i = 0; i < 26; ++i)
                    if (node->children[i]) return false;
                return true;
            }
        }
        return false;
    }

    void printHelper(TrieNode* node, string& prefix) const {
        if (node->isEnd) cout << prefix << "\n";
        for (int i = 0; i < 26; ++i) {
            if (node->children[i]) {
                prefix.push_back('a' + i);
                printHelper(node->children[i], prefix);
                prefix.pop_back();
            }
        }
    }

public:
    Trie() : root(new TrieNode()) {}
    ~Trie() { clear(root); }

    // INSERT
    void insert(const string& word) {
        TrieNode* cur = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!cur->children[idx])
                cur->children[idx] = new TrieNode();
            cur = cur->children[idx];
        }
        cur->isEnd = true;
    }

    // SEARCH (exact word)
    bool search(const string& word) const {
        TrieNode* cur = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!cur->children[idx]) return false;
            cur = cur->children[idx];
        }
        return cur->isEnd;
    }

    // SEARCH (prefix)
    bool startsWith(const string& prefix) const {
        TrieNode* cur = root;
        for (char ch : prefix) {
            int idx = ch - 'a';
            if (!cur->children[idx]) return false;
            cur = cur->children[idx];
        }
        return true;
    }

    // DELETE
    void erase(const string& word) {
        deleteHelper(root, word, 0);
    }

    // PRINT all words
    void printAll() const {
        string prefix;
        printHelper(root, prefix);
    }
};
```

### 8.2 Python Trie

```python
class TrieNode:
    def __init__(self) -> None:
        self.children: dict[str, "TrieNode"] = {}
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
            if ch not in node.children: return False
            node = node.children[ch]
        return node.end

    def starts_with(self, prefix: str) -> bool:
        node = self.root
        for ch in prefix:
            if ch not in node.children: return False
            node = node.children[ch]
        return True

    def delete_word(self, word: str) -> None:
        def _delete(node, word, depth):
            if depth == len(word):
                if not node.end: return False
                node.end = False
                return len(node.children) == 0
            ch = word[depth]
            if ch not in node.children: return False
            should_delete = _delete(node.children[ch], word, depth + 1)
            if should_delete:
                del node.children[ch]
                return not node.end and len(node.children) == 0
            return False
        _delete(self.root, word, 0)

    def print_all(self) -> None:
        def collect(node, prefix, words):
            if node.end: words.append(prefix)
            for ch, child in node.children.items():
                collect(child, prefix + ch, words)
        words = []
        collect(self.root, "", words)
        for w in words: print(w)
```

---

## 9. Graphs – Representations & Traversals (Graflar)

Bu bölüm grafların iki temel gösterim biçimini (**Adjacency List** ve **Adjacency Matrix**) ve her ikisi üzerinde **BFS** ve **DFS** algoritmalarını içerir.

### 9.1 Adjacency List (Komşuluk Listesi)

Sparse (seyrek) graflar için idealdir. Bellek: O(V + E).

**C++ – Adjacency List Graph (Insert Edge, Delete Edge, Search Edge, BFS, DFS, Print)**

```cpp
#include <bits/stdc++.h>
using namespace std;

class GraphAdjList {
private:
    int V;
    vector<vector<int>> adj;

    void DFS_helper(int u, vector<bool> &visited) const {
        visited[u] = true;
        cout << u << " ";
        for (int v : adj[u]) {
            if (!visited[v]) DFS_helper(v, visited);
        }
    }

public:
    GraphAdjList(int n) : V(n), adj(n) {}

    // INSERT EDGE (yönsüz)
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // INSERT EDGE (yönlü – directed)
    void addDirectedEdge(int u, int v) {
        adj[u].push_back(v);
    }

    // DELETE EDGE (yönsüz)
    void removeEdge(int u, int v) {
        adj[u].erase(remove(adj[u].begin(), adj[u].end(), v), adj[u].end());
        adj[v].erase(remove(adj[v].begin(), adj[v].end(), u), adj[v].end());
    }

    // SEARCH EDGE
    bool hasEdge(int u, int v) const {
        for (int x : adj[u])
            if (x == v) return true;
        return false;
    }

    // PRINT (Adjacency List)
    void print() const {
        for (int i = 0; i < V; ++i) {
            cout << i << ": ";
            for (int v : adj[i]) cout << v << " ";
            cout << "\n";
        }
    }

    // ===== BFS (Adjacency List) =====
    void BFS(int start) const {
        vector<bool> visited(V, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);

        cout << "BFS (Adj List) from " << start << ": ";
        while (!q.empty()) {
            int u = q.front(); q.pop();
            cout << u << " ";
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        cout << "\n";
    }

    // BFS – mesafe hesaplama
    vector<int> BFS_distances(int start) const {
        vector<int> dist(V, -1);
        queue<int> q;
        dist[start] = 0;
        q.push(start);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        return dist;
    }

    // ===== DFS (Adjacency List) – Recursive =====
    void DFS(int start) const {
        vector<bool> visited(V, false);
        cout << "DFS (Adj List) from " << start << ": ";
        DFS_helper(start, visited);
        cout << "\n";
    }

    // ===== DFS (Adjacency List) – Iterative (Stack ile) =====
    void DFS_iterative(int start) const {
        vector<bool> visited(V, false);
        stack<int> st;
        st.push(start);

        cout << "DFS Iterative (Adj List) from " << start << ": ";
        while (!st.empty()) {
            int u = st.top(); st.pop();
            if (visited[u]) continue;
            visited[u] = true;
            cout << u << " ";
            for (int i = (int)adj[u].size() - 1; i >= 0; --i) {
                if (!visited[adj[u][i]])
                    st.push(adj[u][i]);
            }
        }
        cout << "\n";
    }

    int vertexCount() const { return V; }
    const vector<int>& neighbors(int u) const { return adj[u]; }
};
```

**Python – Adjacency List Graph**

```python
from collections import deque

class GraphAdjList:
    def __init__(self, n: int):
        self.V = n
        self.adj = [[] for _ in range(n)]

    # INSERT EDGE
    def add_edge(self, u: int, v: int) -> None:
        self.adj[u].append(v)
        self.adj[v].append(u)

    # DELETE EDGE
    def remove_edge(self, u: int, v: int) -> None:
        self.adj[u] = [x for x in self.adj[u] if x != v]
        self.adj[v] = [x for x in self.adj[v] if x != u]

    # SEARCH EDGE
    def has_edge(self, u: int, v: int) -> bool:
        return v in self.adj[u]

    # PRINT
    def print_graph(self) -> None:
        for i in range(self.V):
            print(f"{i}: {self.adj[i]}")

    # BFS
    def bfs(self, start: int) -> list[int]:
        visited = [False] * self.V
        order = []
        q = deque([start])
        visited[start] = True
        while q:
            u = q.popleft()
            order.append(u)
            for v in self.adj[u]:
                if not visited[v]:
                    visited[v] = True
                    q.append(v)
        return order

    # DFS (recursive)
    def dfs(self, start: int) -> list[int]:
        visited = [False] * self.V
        order = []
        self._dfs(start, visited, order)
        return order

    def _dfs(self, u: int, visited: list, order: list) -> None:
        visited[u] = True
        order.append(u)
        for v in self.adj[u]:
            if not visited[v]:
                self._dfs(v, visited, order)

    # DFS (iterative)
    def dfs_iterative(self, start: int) -> list[int]:
        visited = [False] * self.V
        order = []
        stack = [start]
        while stack:
            u = stack.pop()
            if visited[u]: continue
            visited[u] = True
            order.append(u)
            for v in reversed(self.adj[u]):
                if not visited[v]:
                    stack.append(v)
        return order
```

---

### 9.2 Adjacency Matrix (Komşuluk Matrisi)

Dense (yoğun) graflar için veya kenar sorgularının O(1) olması gerektiğinde uygundur. Bellek: O(V²).

**C++ – Adjacency Matrix Graph (Insert Edge, Delete Edge, Search Edge, BFS, DFS, Print)**

```cpp
class GraphAdjMatrix {
private:
    int V;
    vector<vector<int>> mat; // mat[i][j] = 1 ise kenar var

    void DFS_helper(int u, vector<bool> &visited) const {
        visited[u] = true;
        cout << u << " ";
        for (int v = 0; v < V; ++v) {
            if (mat[u][v] == 1 && !visited[v])
                DFS_helper(v, visited);
        }
    }

public:
    GraphAdjMatrix(int n) : V(n), mat(n, vector<int>(n, 0)) {}

    // INSERT EDGE (yönsüz)
    void addEdge(int u, int v) {
        mat[u][v] = 1;
        mat[v][u] = 1;
    }

    // INSERT EDGE (yönlü)
    void addDirectedEdge(int u, int v) {
        mat[u][v] = 1;
    }

    // DELETE EDGE (yönsüz)
    void removeEdge(int u, int v) {
        mat[u][v] = 0;
        mat[v][u] = 0;
    }

    // SEARCH EDGE – O(1)
    bool hasEdge(int u, int v) const {
        return mat[u][v] == 1;
    }

    // PRINT (Matrix)
    void print() const {
        cout << "  ";
        for (int i = 0; i < V; ++i) cout << i << " ";
        cout << "\n";
        for (int i = 0; i < V; ++i) {
            cout << i << " ";
            for (int j = 0; j < V; ++j) cout << mat[i][j] << " ";
            cout << "\n";
        }
    }

    // ===== BFS (Adjacency Matrix) =====
    void BFS(int start) const {
        vector<bool> visited(V, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);

        cout << "BFS (Adj Matrix) from " << start << ": ";
        while (!q.empty()) {
            int u = q.front(); q.pop();
            cout << u << " ";
            for (int v = 0; v < V; ++v) {
                if (mat[u][v] == 1 && !visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        cout << "\n";
    }

    // BFS – mesafe hesaplama
    vector<int> BFS_distances(int start) const {
        vector<int> dist(V, -1);
        queue<int> q;
        dist[start] = 0;
        q.push(start);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v = 0; v < V; ++v) {
                if (mat[u][v] == 1 && dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        return dist;
    }

    // ===== DFS (Adjacency Matrix) – Recursive =====
    void DFS(int start) const {
        vector<bool> visited(V, false);
        cout << "DFS (Adj Matrix) from " << start << ": ";
        DFS_helper(start, visited);
        cout << "\n";
    }

    // ===== DFS (Adjacency Matrix) – Iterative (Stack ile) =====
    void DFS_iterative(int start) const {
        vector<bool> visited(V, false);
        stack<int> st;
        st.push(start);

        cout << "DFS Iterative (Adj Matrix) from " << start << ": ";
        while (!st.empty()) {
            int u = st.top(); st.pop();
            if (visited[u]) continue;
            visited[u] = true;
            cout << u << " ";
            for (int v = V - 1; v >= 0; --v) {
                if (mat[u][v] == 1 && !visited[v])
                    st.push(v);
            }
        }
        cout << "\n";
    }

    int vertexCount() const { return V; }
};
```

**Python – Adjacency Matrix Graph**

```python
from collections import deque

class GraphAdjMatrix:
    def __init__(self, n: int):
        self.V = n
        self.mat = [[0] * n for _ in range(n)]

    # INSERT EDGE
    def add_edge(self, u: int, v: int) -> None:
        self.mat[u][v] = 1
        self.mat[v][u] = 1

    # DELETE EDGE
    def remove_edge(self, u: int, v: int) -> None:
        self.mat[u][v] = 0
        self.mat[v][u] = 0

    # SEARCH EDGE – O(1)
    def has_edge(self, u: int, v: int) -> bool:
        return self.mat[u][v] == 1

    # PRINT
    def print_graph(self) -> None:
        print("  ", " ".join(str(i) for i in range(self.V)))
        for i in range(self.V):
            print(i, " ".join(str(x) for x in self.mat[i]))

    # BFS
    def bfs(self, start: int) -> list[int]:
        visited = [False] * self.V
        order = []
        q = deque([start])
        visited[start] = True
        while q:
            u = q.popleft()
            order.append(u)
            for v in range(self.V):
                if self.mat[u][v] == 1 and not visited[v]:
                    visited[v] = True
                    q.append(v)
        return order

    # DFS (recursive)
    def dfs(self, start: int) -> list[int]:
        visited = [False] * self.V
        order = []
        self._dfs(start, visited, order)
        return order

    def _dfs(self, u: int, visited: list, order: list) -> None:
        visited[u] = True
        order.append(u)
        for v in range(self.V):
            if self.mat[u][v] == 1 and not visited[v]:
                self._dfs(v, visited, order)

    # DFS (iterative)
    def dfs_iterative(self, start: int) -> list[int]:
        visited = [False] * self.V
        order = []
        stack = [start]
        while stack:
            u = stack.pop()
            if visited[u]: continue
            visited[u] = True
            order.append(u)
            for v in range(self.V - 1, -1, -1):
                if self.mat[u][v] == 1 and not visited[v]:
                    stack.append(v)
        return order
```

### 9.3 Adjacency List vs Adjacency Matrix Karşılaştırma

| Özellik              | Adjacency List   | Adjacency Matrix |
|----------------------|------------------|------------------|
| Bellek               | O(V + E)         | O(V²)           |
| Kenar ekleme         | O(1)             | O(1)            |
| Kenar silme          | O(E)             | O(1)            |
| Kenar sorgulama      | O(degree)        | O(1)            |
| Komşuları listeleme  | O(degree)        | O(V)            |
| Seyrek graf için      | ✅ İdeal         | ❌ Bellek israfı |
| Yoğun graf için       | ▪ Olur            | ✅ İdeal        |

---

### 9.4 Ağırlıklı Graf (Weighted Graph) – Adjacency List

```cpp
class WeightedGraphAdjList {
private:
    int V;
    vector<vector<pair<int,int>>> adj; // (komşu, ağırlık)

public:
    WeightedGraphAdjList(int n) : V(n), adj(n) {}

    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    void print() const {
        for (int i = 0; i < V; ++i) {
            cout << i << ": ";
            for (auto [v, w] : adj[i])
                cout << "(" << v << ", w=" << w << ") ";
            cout << "\n";
        }
    }

    // Dijkstra – tek kaynaklı en kısa yol
    vector<int> dijkstra(int src) const {
        const int INF = 1e9;
        vector<int> dist(V, INF);
        dist[src] = 0;
        using P = pair<int,int>;
        priority_queue<P, vector<P>, greater<P>> pq;
        pq.push({0, src});

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            for (auto [v, w] : adj[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    }
};
```

### 9.5 Ağırlıklı Graf – Adjacency Matrix

```cpp
class WeightedGraphAdjMatrix {
private:
    int V;
    vector<vector<int>> mat; // mat[i][j] = weight, 0 = no edge

    void DFS_helper(int u, vector<bool> &visited) const {
        visited[u] = true;
        cout << u << " ";
        for (int v = 0; v < V; ++v) {
            if (mat[u][v] != 0 && !visited[v])
                DFS_helper(v, visited);
        }
    }

public:
    WeightedGraphAdjMatrix(int n) : V(n), mat(n, vector<int>(n, 0)) {}

    void addEdge(int u, int v, int w) {
        mat[u][v] = w;
        mat[v][u] = w;
    }

    void removeEdge(int u, int v) {
        mat[u][v] = 0;
        mat[v][u] = 0;
    }

    bool hasEdge(int u, int v) const { return mat[u][v] != 0; }

    void print() const {
        cout << "    ";
        for (int i = 0; i < V; ++i) cout << setw(4) << i;
        cout << "\n";
        for (int i = 0; i < V; ++i) {
            cout << setw(4) << i;
            for (int j = 0; j < V; ++j) cout << setw(4) << mat[i][j];
            cout << "\n";
        }
    }

    // Dijkstra – Adjacency Matrix (O(V²))
    vector<int> dijkstra(int src) const {
        const int INF = 1e9;
        vector<int> dist(V, INF);
        vector<bool> visited(V, false);
        dist[src] = 0;

        for (int i = 0; i < V; ++i) {
            int u = -1;
            for (int j = 0; j < V; ++j)
                if (!visited[j] && (u == -1 || dist[j] < dist[u])) u = j;
            if (dist[u] == INF) break;
            visited[u] = true;
            for (int v = 0; v < V; ++v) {
                if (mat[u][v] != 0 && dist[u] + mat[u][v] < dist[v])
                    dist[v] = dist[u] + mat[u][v];
            }
        }
        return dist;
    }

    // BFS
    void BFS(int start) const {
        vector<bool> visited(V, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);
        cout << "BFS (Weighted Adj Matrix) from " << start << ": ";
        while (!q.empty()) {
            int u = q.front(); q.pop();
            cout << u << " ";
            for (int v = 0; v < V; ++v) {
                if (mat[u][v] != 0 && !visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        cout << "\n";
    }

    // DFS
    void DFS(int start) const {
        vector<bool> visited(V, false);
        cout << "DFS (Weighted Adj Matrix) from " << start << ": ";
        DFS_helper(start, visited);
        cout << "\n";
    }
};
```

**Python – Ağırlıklı Graf (Dijkstra) – Adjacency List**

```python
import heapq

def dijkstra(n: int, src: int, adj: list[list[tuple[int,int]]]) -> list[int]:
    INF = 10**18
    dist = [INF] * n
    dist[src] = 0
    pq = [(0, src)]
    while pq:
        d, u = heapq.heappop(pq)
        if d > dist[u]: continue
        for v, w in adj[u]:
            nd = d + w
            if nd < dist[v]:
                dist[v] = nd
                heapq.heappush(pq, (nd, v))
    return dist
```

---

## 10. Union-Find (Disjoint Set Union – DSU)

Tracks connectivity between elements with near O(1) `find`/`union`.

**C++**

```cpp
struct DSU {
    vector<int> parent, rank_;
    DSU(int n) : parent(n), rank_(n, 0) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]); // path compression
    }

    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (rank_[a] < rank_[b]) swap(a, b);
        parent[b] = a;
        if (rank_[a] == rank_[b]) rank_[a]++;
    }

    bool connected(int a, int b) { return find(a) == find(b); }
};
```

**Python**

```python
class DSU:
    def __init__(self, n: int):
        self.parent = list(range(n))
        self.rank = [0] * n

    def find(self, x: int) -> int:
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])
        return self.parent[x]

    def unite(self, a: int, b: int) -> None:
        a, b = self.find(a), self.find(b)
        if a == b: return
        if self.rank[a] < self.rank[b]: a, b = b, a
        self.parent[b] = a
        if self.rank[a] == self.rank[b]: self.rank[a] += 1

    def connected(self, a: int, b: int) -> bool:
        return self.find(a) == self.find(b)
```

Use: connectivity queries, Kruskal MST, dynamic components.

---

## 11. Searching Algorithms (Arama Algoritmaları)

### 11.1 Linear Search – O(n)

```cpp
int linearSearch(const vector<int> &arr, int target) {
    for (int i = 0; i < (int)arr.size(); ++i)
        if (arr[i] == target) return i;
    return -1;
}
```

```python
def linear_search(a: list[int], x: int) -> int:
    for i, v in enumerate(a):
        if v == x: return i
    return -1
```

### 11.2 Binary Search – O(log n) – Sıralı Dizide

```cpp
int binarySearch(const vector<int> &arr, int target) {
    int left = 0, right = (int)arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}
```

```python
def binary_search(a: list[int], x: int) -> int:
    lo, hi = 0, len(a) - 1
    while lo <= hi:
        mid = lo + (hi - lo) // 2
        if a[mid] == x: return mid
        elif a[mid] < x: lo = mid + 1
        else: hi = mid - 1
    return -1
```

STL: `std::binary_search`, `std::lower_bound`, `std::upper_bound`.

---

## 12. Sorting Algorithms (Sıralama Algoritmaları)

### 12.1 Bubble Sort – O(n²)

```cpp
void bubbleSort(vector<int> &arr) {
    int n = (int)arr.size();
    bool swapped;
    for (int i = 0; i < n - 1; ++i) {
        swapped = false;
        for (int j = 0; j < n - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}
```

### 12.2 Selection Sort – O(n²)

```cpp
void selectionSort(vector<int> &arr) {
    int n = (int)arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < n; ++j)
            if (arr[j] < arr[minIdx]) minIdx = j;
        if (minIdx != i) swap(arr[i], arr[minIdx]);
    }
}
```

### 12.3 Insertion Sort – O(n²) (küçük / neredeyse sıralı diziler için iyi)

```cpp
void insertionSort(vector<int> &arr) {
    int n = (int)arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}
```

```python
def insertion_sort(a: list[int]) -> None:
    for i in range(1, len(a)):
        key = a[i]
        j = i - 1
        while j >= 0 and a[j] > key:
            a[j + 1] = a[j]
            j -= 1
        a[j + 1] = key
```

### 12.4 Merge Sort – O(n log n) (stable)

```cpp
void merge(vector<int> &arr, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<int> &arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}
```

### 12.5 Quick Sort – O(n log n) average, O(n²) worst

```cpp
int partition(vector<int> &arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<int> &arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
```

```python
def quick_sort(a: list[int]) -> list[int]:
    if len(a) <= 1: return a
    pivot = a[len(a) // 2]
    left  = [x for x in a if x < pivot]
    mid   = [x for x in a if x == pivot]
    right = [x for x in a if x > pivot]
    return quick_sort(left) + mid + quick_sort(right)
```

### 12.6 Heap Sort – O(n log n)

```cpp
void heapify(vector<int> &arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int> &arr) {
    int n = (int)arr.size();
    for (int i = n / 2 - 1; i >= 0; --i) heapify(arr, n, i);
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}
```

### 12.7 Sıralama Karşılaştırma Tablosu

| Algoritma      | Best      | Average   | Worst     | Stable | In-place |
|----------------|-----------|-----------|-----------|--------|----------|
| Bubble Sort    | O(n)      | O(n²)    | O(n²)    | ✅      | ✅        |
| Selection Sort | O(n²)    | O(n²)    | O(n²)    | ❌      | ✅        |
| Insertion Sort | O(n)      | O(n²)    | O(n²)    | ✅      | ✅        |
| Merge Sort     | O(n lg n) | O(n lg n)| O(n lg n)| ✅      | ❌        |
| Quick Sort     | O(n lg n) | O(n lg n)| O(n²)    | ❌      | ✅        |
| Heap Sort      | O(n lg n) | O(n lg n)| O(n lg n)| ❌      | ✅        |

STL: `std::sort(arr.begin(), arr.end());` — pratikte O(n log n) introsort.

---

## 13. Graph Algorithms (İleri Düzey)

### 13.1 Topological Sort (DAG) – Kahn's Algorithm

```cpp
vector<int> topologicalSort(int n, const vector<vector<int>>& g) {
    vector<int> indeg(n, 0);
    for (int u = 0; u < n; ++u)
        for (int v : g[u]) indeg[v]++;

    queue<int> q;
    for (int i = 0; i < n; ++i)
        if (indeg[i] == 0) q.push(i);

    vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : g[u])
            if (--indeg[v] == 0) q.push(v);
    }
    if ((int)order.size() != n) return {}; // cycle detected
    return order;
}
```

### 13.2 Cycle Detection (Döngü Tespiti) – Directed Graph

```cpp
bool hasCycleDFS(int u, const vector<vector<int>>& g, vector<int>& color) {
    color[u] = 1; // visiting (gray)
    for (int v : g[u]) {
        if (color[v] == 1) return true;       // back edge = cycle
        if (color[v] == 0 && hasCycleDFS(v, g, color)) return true;
    }
    color[u] = 2; // done (black)
    return false;
}

bool hasCycle(int n, const vector<vector<int>>& g) {
    vector<int> color(n, 0); // 0=white, 1=gray, 2=black
    for (int i = 0; i < n; ++i)
        if (color[i] == 0 && hasCycleDFS(i, g, color)) return true;
    return false;
}
```

### 13.3 Connected Components (Bağlı Bileşenler)

```cpp
int countComponents(int n, const vector<vector<int>>& g) {
    vector<bool> visited(n, false);
    int count = 0;
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            ++count;
            queue<int> q;
            q.push(i);
            visited[i] = true;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : g[u]) {
                    if (!visited[v]) {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        }
    }
    return count;
}
```

---

## 14. B-Trees

B-tree çok büyük veri yapılarında (disk tabanlı) kullanılır. `t` minimum derece olmak üzere her düğüm max `2t-1` anahtar tutar.

```cpp
class BTreeNode {
public:
    bool leaf;
    int t;
    vector<int> keys;
    vector<BTreeNode*> children;

    BTreeNode(int _t, bool _leaf) : leaf(_leaf), t(_t) {
        keys.reserve(2 * t - 1);
        children.reserve(2 * t);
    }

    ~BTreeNode() {
        if (!leaf)
            for (auto c : children) delete c;
    }

    // PRINT (traverse)
    void traverse() {
        int i;
        for (i = 0; i < (int)keys.size(); ++i) {
            if (!leaf && i < (int)children.size()) children[i]->traverse();
            cout << " " << keys[i];
        }
        if (!leaf && i < (int)children.size()) children[i]->traverse();
    }

    // SEARCH
    BTreeNode* search(int k) {
        int i = 0;
        while (i < (int)keys.size() && k > keys[i]) ++i;
        if (i < (int)keys.size() && keys[i] == k) return this;
        if (leaf) return nullptr;
        return children[i]->search(k);
    }

    void splitChild(int i, BTreeNode *y) {
        BTreeNode *z = new BTreeNode(y->t, y->leaf);
        for (int j = 0; j < t - 1; ++j)
            z->keys.push_back(y->keys[j + t]);
        if (!y->leaf)
            for (int j = 0; j < t; ++j)
                z->children.push_back(y->children[j + t]);
        y->keys.resize(t - 1);
        if (!y->leaf) y->children.resize(t);
        children.insert(children.begin() + i + 1, z);
        keys.insert(keys.begin() + i, y->keys[t - 1]);
    }

    // INSERT (non-full node)
    void insertNonFull(int k) {
        int i = (int)keys.size() - 1;
        if (leaf) {
            keys.push_back(0);
            while (i >= 0 && keys[i] > k) { keys[i + 1] = keys[i]; --i; }
            keys[i + 1] = k;
        } else {
            while (i >= 0 && keys[i] > k) --i;
            ++i;
            if ((int)children[i]->keys.size() == 2 * t - 1) {
                splitChild(i, children[i]);
                if (keys[i] < k) ++i;
            }
            children[i]->insertNonFull(k);
        }
    }
};

class BTree {
private:
    BTreeNode *root;
    int t;

public:
    BTree(int _t) : root(nullptr), t(_t) {}
    ~BTree() { delete root; }

    void traverse() {
        if (root) root->traverse();
        cout << "\n";
    }

    BTreeNode* search(int k) {
        return root ? root->search(k) : nullptr;
    }

    void insert(int k) {
        if (!root) {
            root = new BTreeNode(t, true);
            root->keys.push_back(k);
            return;
        }
        if ((int)root->keys.size() == 2 * t - 1) {
            BTreeNode *s = new BTreeNode(t, false);
            s->children.push_back(root);
            s->splitChild(0, root);
            int i = (s->keys[0] < k) ? 1 : 0;
            s->children[i]->insertNonFull(k);
            root = s;
        } else {
            root->insertNonFull(k);
        }
    }
};
```

---

## 15. Recursion (Özyineleme)

### 15.1 Faktöriyel

```cpp
long long factorial(int n) {
    if (n <= 1) return 1;
    return 1LL * n * factorial(n - 1);
}
```

### 15.2 Fibonacci (Memoization – Top-Down)

```cpp
long long fib(int n, vector<long long> &memo) {
    if (n <= 1) return n;
    if (memo[n] != -1) return memo[n];
    memo[n] = fib(n - 1, memo) + fib(n - 2, memo);
    return memo[n];
}
```

### 15.3 Linked List Recursive Print

```cpp
void printListRecursive(SinglyNode *node) {
    if (!node) { cout << "NULL\n"; return; }
    cout << node->value << " -> ";
    printListRecursive(node->next);
}
```

---

## 16. Dynamic Programming (Dinamik Programlama)

### 16.1 Fibonacci – Tabulation (Bottom-Up) – O(n)

```cpp
long long fib(int n) {
    if (n <= 1) return n;
    long long a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        long long c = a + b;
        a = b;
        b = c;
    }
    return b;
}
```

### 16.2 Fibonacci – Matrix Exponentiation – O(log n)

```python
import numpy as np

def fibonacci_matrix(n):
    if n == 0: return 0
    matrix = np.array([[1, 1], [1, 0]])
    result = np.linalg.matrix_power(matrix, n - 1)
    return result[0][0]
```

### 16.3 Coin Change – Combination (Sıra önemli değil)

Aynı madeni parayı birden fazla kez kullanabilirsin; sıra önemli **değildir** (combination).

```cpp
#include <bits/stdc++.h>
using ll = long long;
const ll MOD = 1000000007;
using namespace std;

ll coinCombination() {
    ll n, k;
    cin >> n >> k;
    vector<ll> arr(n + 1);
    vector<vector<ll>> dp(n + 1, vector<ll>(k + 1, 0));
    for (ll i = 1; i <= n; i++) cin >> arr[i];

    for (ll i = 0; i <= n; i++) dp[i][0] = 1;

    for (ll i = 1; i <= n; i++) {
        for (ll s = 1; s <= k; s++) {
            dp[i][s] = dp[i - 1][s]; // bu parayı kullanmadan
            if (s >= arr[i])
                dp[i][s] = (dp[i][s] + dp[i][s - arr[i]]) % MOD;
        }
    }
    return dp[n][k] % MOD;
}
```

### 16.4 Coin Change – Permutation (Sıra önemli)

```cpp
#include <bits/stdc++.h>
using ll = long long;
const ll MOD = 1000000007;
using namespace std;

ll coinPermutation() {
    ll n, k;
    cin >> n >> k;
    vector<ll> arr(n + 1);
    vector<ll> dp(k + 1, 0);
    for (ll i = 1; i <= n; i++) cin >> arr[i];

    dp[0] = 1;

    for (ll s = 1; s <= k; s++) {
        for (ll i = 1; i <= n; i++) {
            if (s >= arr[i]) {
                dp[s] += dp[s - arr[i]];
                if (dp[s] >= MOD) dp[s] -= MOD;
            }
        }
    }
    return dp[k] % MOD;
}
```

---

## 17. Problem-Solving Patterns (Çözüm Kalıpları)

### 17.1 Two Pointers (sıralı dizi – çift toplam)

```python
def two_sum_sorted(a: list[int], target: int) -> tuple[int,int] | None:
    i, j = 0, len(a) - 1
    while i < j:
        s = a[i] + a[j]
        if s == target: return i, j
        if s < target: i += 1
        else: j -= 1
    return None
```

### 17.2 Sliding Window (sabit boyutlu pencere)

```cpp
int maxSumK(const vector<int>& a, int k) {
    if ((int)a.size() < k) return 0;
    int win = 0;
    for (int i = 0; i < k; ++i) win += a[i];
    int best = win;
    for (int i = k; i < (int)a.size(); ++i) {
        win += a[i] - a[i - k];
        best = max(best, win);
    }
    return best;
}
```

### 17.3 Prefix Sum (aralık toplamı)

```python
a = [1, 2, 3, 4]
p = [0]
for x in a:
    p.append(p[-1] + x)
# sum on [l,r] = p[r+1] - p[l]
```

### 17.4 Backtracking (tüm alt kümeler)

```python
def subsets(nums: list[int]) -> list[list[int]]:
    res: list[list[int]] = []
    def dfs(i: int, cur: list[int]) -> None:
        if i == len(nums):
            res.append(cur.copy())
            return
        dfs(i + 1, cur)          # nums[i]'yı alma
        cur.append(nums[i])
        dfs(i + 1, cur)          # nums[i]'yı al
        cur.pop()
    dfs(0, [])
    return res
```

---

## 18. Örnek `main` – Her Şeyi Test Et

```cpp
#include <bits/stdc++.h>
using namespace std;

// ... yukarıdaki tüm struct/class tanımlarını buraya koy ...

int main() {
    // ===== Array =====
    ArrayList al;
    al.push_back(3); al.push_back(1); al.push_back(4);
    al.insert(1, 2);
    cout << "ArrayList: "; al.print();
    cout << "Search 4: index=" << al.search(4) << "\n";
    al.erase(0);
    cout << "After erase(0): "; al.print();

    // ===== Singly Linked List =====
    SinglyLinkedList sll;
    sll.push_back(10); sll.push_back(20); sll.push_front(5);
    cout << "SLL: "; sll.print();
    cout << "Search 20: " << sll.search(20) << "\n";
    sll.remove(20);
    cout << "After remove(20): "; sll.print();

    // ===== Doubly Linked List =====
    DoublyLinkedList dll;
    dll.push_back(1); dll.push_back(2); dll.push_back(3);
    cout << "DLL forward: "; dll.print_forward();
    cout << "DLL backward: "; dll.print_backward();
    dll.remove(2);
    cout << "After remove(2): "; dll.print_forward();

    // ===== Stack =====
    ArrayStack st;
    st.push(1); st.push(2); st.push(3);
    cout << "Stack top = " << st.top() << "\n";
    st.print();

    // ===== Queue =====
    CircularQueue cq(5);
    cq.enqueue(10); cq.enqueue(20); cq.enqueue(30);
    cq.print();
    cq.dequeue();
    cout << "After dequeue, front = " << cq.front() << "\n";

    // ===== Hash Table =====
    HashTable ht;
    ht.insert(10, "Ali"); ht.insert(20, "Veli"); ht.insert(30, "Can");
    ht.print();
    string* found = ht.search(20);
    if (found) cout << "Search 20: " << *found << "\n";
    ht.remove(20);

    // ===== BST =====
    BinarySearchTree bst;
    bst.insert(50); bst.insert(30); bst.insert(70);
    bst.insert(20); bst.insert(40);
    cout << "BST inorder: "; bst.print();
    cout << "Search 30: " << bst.search(30) << "\n";
    bst.remove(30);
    cout << "After remove(30): "; bst.print();

    // ===== Sorting =====
    vector<int> arr = {5, 2, 9, 1, 5, 6};
    quickSort(arr, 0, (int)arr.size() - 1);
    cout << "QuickSort: ";
    for (int x : arr) cout << x << " ";
    cout << "\n";

    // ===== Graph – Adjacency List =====
    GraphAdjList gList(6);
    gList.addEdge(0, 1); gList.addEdge(0, 2);
    gList.addEdge(1, 3); gList.addEdge(2, 4);
    gList.addEdge(3, 5); gList.addEdge(4, 5);
    cout << "\n=== Adjacency List ===\n"; gList.print();
    gList.BFS(0);
    gList.DFS(0);
    gList.DFS_iterative(0);

    // ===== Graph – Adjacency Matrix =====
    GraphAdjMatrix gMat(6);
    gMat.addEdge(0, 1); gMat.addEdge(0, 2);
    gMat.addEdge(1, 3); gMat.addEdge(2, 4);
    gMat.addEdge(3, 5); gMat.addEdge(4, 5);
    cout << "\n=== Adjacency Matrix ===\n"; gMat.print();
    gMat.BFS(0);
    gMat.DFS(0);
    gMat.DFS_iterative(0);

    // ===== Weighted Graph – Dijkstra =====
    WeightedGraphAdjList wg(5);
    wg.addEdge(0, 1, 4); wg.addEdge(0, 2, 1);
    wg.addEdge(2, 1, 2); wg.addEdge(1, 3, 1);
    wg.addEdge(2, 3, 5); wg.addEdge(3, 4, 3);
    vector<int> dist = wg.dijkstra(0);
    cout << "\nDijkstra from 0: ";
    for (int d : dist) cout << d << " ";
    cout << "\n";

    // ===== B-Tree =====
    BTree bt(2);
    bt.insert(10); bt.insert(20); bt.insert(5);
    bt.insert(6); bt.insert(12); bt.insert(30);
    cout << "B-Tree: "; bt.traverse();

    return 0;
}
```

---

## 19. How to Practice

- Her yapıyı/algoritmayı C++ ve Python'da ezberden yeniden yaz.
- Her biri için:
  - Tanım ve kullanım alanlarını açıkla.
  - Zaman/alan karmaşıklıklarını belirt.
  - Küçük bir örnek üzerinde yürüt.
- Kalıba göre problem çöz:
  - **Arrays/strings** → two pointers, sliding window, prefix sums.
  - **Trees/graphs** → DFS/BFS, recursion.
  - **DP** → overlapping subproblems & optimal substructure tanı.
  - **Graphs** → adjacency list vs matrix seç, BFS/DFS/Dijkstra uygula.

---

## 20. Fast Exponentiation (Hızlı Üs Alma – Binary Exponentiation)

Bir tabanı büyük bir üsse çıkarmak O(n) yerine **O(log n)**'de yapılır. Competitive programming'de modüler aritmetikle birlikte çok sık kullanılır.

### 20.1 Temel Fikir

$a^n$ hesaplamak için:

- $n$ çift ise: $a^n = (a^{n/2})^2$
- $n$ tek ise: $a^n = a \cdot a^{n-1}$

Bu sayede her adımda üs yarıya iner → **O(log n)**.

### 20.2 C++ – Iteratif (Önerilen)

```cpp
#include <bits/stdc++.h>
using namespace std;

// mod olmadan
long long binpow(long long a, long long n) {
    long long result = 1;
    while (n > 0) {
        if (n & 1) result *= a;   // n tek ise çarp
        a *= a;                    // tabanı karele
        n >>= 1;                   // n'i yarıla
    }
    return result;
}

// mod ile (competitive programming'de en sık kullanılan)
long long binpow(long long a, long long n, long long mod) {
    long long result = 1;
    a %= mod;
    while (n > 0) {
        if (n & 1) result = result * a % mod;
        a = a * a % mod;
        n >>= 1;
    }
    return result;
}
```

### 20.3 C++ – Recursive

```cpp
long long binpow_rec(long long a, long long n, long long mod) {
    if (n == 0) return 1;
    long long half = binpow_rec(a, n / 2, mod);
    half = half * half % mod;
    if (n & 1) half = half * a % mod;
    return half;
}
```

### 20.4 Python

```python
# Python'da built-in pow(a, n, mod) zaten O(log n) çalışır!
result = pow(2, 1000000007 - 2, 1000000007)  # Fermat modular inverse

# Manuel versiyon
def binpow(a: int, n: int, mod: int) -> int:
    result = 1
    a %= mod
    while n > 0:
        if n & 1:
            result = result * a % mod
        a = a * a % mod
        n >>= 1
    return result
```

### 20.5 Uygulama Alanları

| Kullanım | Açıklama |
|---|---|
| Modüler ters (Modular Inverse) | $a^{-1} \equiv a^{p-2} \pmod{p}$ (Fermat) |
| Matris üs alma | Fibonacci O(log n)'de hesaplama |
| Büyük kombinasyon / faktöriyel | $nCr \pmod{p}$ |
| Geometrik seri toplamı | Bölme ve fethet ile |

---

## 21. Matris Üs Alma (Matrix Exponentiation)

Fibonacci gibi lineer reküransları **O(k³ log n)**'de çözer (k = matris boyutu).

```cpp
#include <bits/stdc++.h>
using namespace std;

typedef vector<vector<long long>> Matrix;
const long long MOD = 1e9 + 7;

Matrix multiply(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, vector<long long>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < n; ++k)
            if (A[i][k])
                for (int j = 0; j < n; ++j)
                    C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % MOD;
    return C;
}

Matrix matpow(Matrix A, long long p) {
    int n = A.size();
    Matrix result(n, vector<long long>(n, 0));
    for (int i = 0; i < n; ++i) result[i][i] = 1; // identity
    while (p > 0) {
        if (p & 1) result = multiply(result, A);
        A = multiply(A, A);
        p >>= 1;
    }
    return result;
}

// Fibonacci(n) in O(log n)
long long fib(long long n) {
    if (n <= 1) return n;
    Matrix M = {{1, 1}, {1, 0}};
    Matrix R = matpow(M, n - 1);
    return R[0][0];
}
```

---

## 22. Competitive Programming – `bits/stdc++.h` Önemli Fonksiyonlar

### 22.1 Matematik Fonksiyonları

| Fonksiyon | Açıklama | Örnek |
|---|---|---|
| `pow(x, y)` | $x^y$ (double döner, **float hatası olabilir!** Tam sayı için `binpow` kullan) | `pow(2, 10)` → `1024.0` |
| `sqrt(x)` | $\sqrt{x}$ | `sqrt(144)` → `12.0` |
| `cbrt(x)` | $\sqrt[3]{x}$ | `cbrt(27)` → `3.0` |
| `abs(x)` | Mutlak değer (int) | `abs(-5)` → `5` |
| `fabs(x)` | Mutlak değer (double) | `fabs(-3.14)` → `3.14` |
| `ceil(x)` | Yukarı yuvarlama | `ceil(2.3)` → `3` |
| `floor(x)` | Aşağı yuvarlama | `floor(2.9)` → `2` |
| `round(x)` | En yakına yuvarlama | `round(2.5)` → `3` |
| `log(x)` | $\ln(x)$ | `log(2.718)` → `~1.0` |
| `log2(x)` | $\log_2(x)$ | `log2(8)` → `3.0` |
| `log10(x)` | $\log_{10}(x)$ | `log10(1000)` → `3.0` |
| `fmod(x, y)` | Float modulo | `fmod(5.5, 2.0)` → `1.5` |
| `__gcd(a, b)` | GCD (C++14) | `__gcd(12, 8)` → `4` |
| `gcd(a, b)` | GCD (C++17 `<numeric>`) | `gcd(12, 8)` → `4` |
| `lcm(a, b)` | LCM (C++17 `<numeric>`) | `lcm(4, 6)` → `12` |

### 22.2 Bit Manipulation Fonksiyonları (GCC Builtin)

| Fonksiyon | Açıklama | Örnek |
|---|---|---|
| `__builtin_popcount(x)` | 1-bit sayısı (int) | `__builtin_popcount(7)` → `3` |
| `__builtin_popcountll(x)` | 1-bit sayısı (long long) | `__builtin_popcountll(7LL)` → `3` |
| `__builtin_clz(x)` | Baştaki 0 sayısı (count leading zeros) | `__builtin_clz(1)` → `31` |
| `__builtin_ctz(x)` | Sondaki 0 sayısı (count trailing zeros) | `__builtin_ctz(8)` → `3` |
| `__builtin_parity(x)` | 1-bit sayısının tek/çift (0=çift, 1=tek) | `__builtin_parity(7)` → `1` |
| `__lg(x)` | $\lfloor\log_2(x)\rfloor$ | `__lg(8)` → `3` |

**Bit Trick'leri:**

```cpp
x & (-x)          // en düşük set bit'i izole et (lowbit)
x & (x - 1)       // en düşük set bit'i sıfırla
(1 << k)          // 2^k
x | (1 << k)      // k. bit'i set et
x & ~(1 << k)     // k. bit'i temizle
x ^ (1 << k)      // k. bit'i toggle et
(x >> k) & 1      // k. bit'i oku
```

### 22.3 STL Algorithm Fonksiyonları

| Fonksiyon | Açıklama | Zaman |
|---|---|---|
| `sort(begin, end)` | Sıralama (IntroSort) | O(n log n) |
| `stable_sort(begin, end)` | Kararlı sıralama (MergeSort) | O(n log n) |
| `reverse(begin, end)` | Diziyi ters çevir | O(n) |
| `unique(begin, end)` | Ardışık tekrarları sil (sort sonrası kullan!) | O(n) |
| `lower_bound(begin, end, val)` | İlk ≥ val pozisyon (sıralı dizide) | O(log n) |
| `upper_bound(begin, end, val)` | İlk > val pozisyon (sıralı dizide) | O(log n) |
| `binary_search(begin, end, val)` | Var mı yok mu (bool) | O(log n) |
| `next_permutation(begin, end)` | Sonraki permütasyon | O(n) |
| `prev_permutation(begin, end)` | Önceki permütasyon | O(n) |
| `min_element(begin, end)` | Min eleman iterator | O(n) |
| `max_element(begin, end)` | Max eleman iterator | O(n) |
| `accumulate(begin, end, init)` | Toplam (`<numeric>`) | O(n) |
| `count(begin, end, val)` | Değer kaç kez geçiyor | O(n) |
| `fill(begin, end, val)` | Tüm elemanları val yap | O(n) |
| `iota(begin, end, start)` | Artan dizi doldur (start, start+1, ...) | O(n) |
| `rotate(begin, mid, end)` | Diziyi mid etrafında döndür | O(n) |
| `nth_element(begin, nth, end)` | n. eleman doğru yere gelir | O(n) avg |
| `partial_sort(begin, mid, end)` | İlk (mid-begin) eleman sıralı | O(n log k) |
| `merge(b1,e1, b2,e2, out)` | İki sıralı diziyi birleştir | O(n+m) |
| `set_union / set_intersection / set_difference` | Küme işlemleri (sıralı) | O(n+m) |

### 22.4 STL Containers – Hızlı Referans

```cpp
// ---- Sık kullanılan yapılar ----
vector<int> v;                  // dinamik dizi
deque<int> dq;                  // çift uçlu kuyruk
stack<int> st;                  // LIFO
queue<int> q;                   // FIFO
priority_queue<int> pq;         // max-heap
priority_queue<int, vector<int>, greater<int>> minpq; // min-heap

set<int> s;                     // sıralı, unique (red-black tree)
multiset<int> ms;               // sıralı, tekrarlı
map<int,int> mp;                // sıralı key-value
unordered_set<int> us;          // hash set – O(1) avg
unordered_map<int,int> um;      // hash map – O(1) avg

bitset<1000> bs;                // sabit boyutlu bit dizisi (hızlı!)
```

### 22.5 String Fonksiyonları

```cpp
string s = "hello world";
s.substr(0, 5);         // "hello"
s.find("world");        // 6  (bulunamazsa string::npos)
s.rfind("l");           // 9  (sondan ara)
s.size();               // 11
s.length();             // 11 (aynı şey)
s.empty();              // false
s += "!";               // "hello world!"
to_string(42);          // "42"
stoi("42");             // 42
stoll("123456789012");  // 123456789012LL
s.erase(5, 1);          // "helloworld!" (5. pozdan 1 char sil)
s.insert(5, " ");       // "hello world!"
reverse(s.begin(), s.end()); // tersten
sort(s.begin(), s.end());    // karakter sıralama
```

### 22.6 I/O Hızlandırma (Competitive Programming Template)

```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // cout.tie(nullptr);  // opsiyonel

    int t;
    cin >> t;
    while (t--) {
        // çözüm
    }
    return 0;
}
```

### 22.7 Sık Kullanılan Sabitler & Tipler

```cpp
const int INF = 1e9;            // int max yerine
const long long LINF = 1e18;    // long long max yerine
const int MOD = 1e9 + 7;        // en yaygın mod
const double PI = acos(-1.0);   // pi sayısı
const double EPS = 1e-9;        // float karşılaştırma

typedef long long ll;
typedef pair<int,int> pii;
typedef vector<int> vi;
typedef vector<pii> vpii;

#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
```

---

*Bu dosya kasıtlı olarak yoğun ve kompakttır: LeetCode / HackerRank / AlgoLeague'de problem çözerken kontrol listesi ve referans olarak kullan.*
