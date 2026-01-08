# DSA Çalışma Notları ve Uygulama Kodları (C++)

Bu dosya aşağıdaki konular için **teori + tam C++ implementasyonları** içerir:

3. Array-based ve linked list'ler (I)
4. Array-based ve linked list'ler (II)
5. Recursion (Özyineleme)
6. Stacks (Yığınlar)
7. Queues (Kuyruklar)
9. Searching algorithms (Linear, Binary Search)
10. Hashing algorithms
11. Sorting algorithms (Bubble, Selection, Insertion, QuickSort, MergeSort, HeapSort)
12. Binary Search Trees (BST)
13. B-Trees
14. Graphs

Kodlar **`class` ve `Node` yapıları** kullanır, ayrıca uygun yerlerde **STL (built-in kütüphane)** kullanılır (`vector`, `list`, `stack`, `queue`, `unordered_map`, `priority_queue`, `algorithm` vs.).

---

## 3–4. Array-Based ve Linked List (I & II)

### 3.1. Array-Based List (Dinamik Dizi Listesi)

Özellikler:
- Sırayla indekslenmiş elemanlar
- Amortize `O(1)` ekleme (sona push)
- Ortaya ekleme/silme: `O(n)`

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
    ArrayList(int cap = 4) {
        capacity = cap;
        length = 0;
        data = new int[capacity];
    }

    ~ArrayList() { delete[] data; }

    int size() const { return length; }
    bool empty() const { return length == 0; }

    int operator[](int index) const {
        if (index < 0 || index >= length) throw out_of_range("Index");
        return data[index];
    }

    void push_back(int value) {
        if (length == capacity) resize(capacity * 2);
        data[length++] = value;
    }

    void insert(int index, int value) {
        if (index < 0 || index > length) throw out_of_range("Index");
        if (length == capacity) resize(capacity * 2);
        for (int i = length; i > index; --i) {
            data[i] = data[i - 1];
        }
        data[index] = value;
        ++length;
    }

    void erase(int index) {
        if (index < 0 || index >= length) throw out_of_range("Index");
        for (int i = index; i < length - 1; ++i) {
            data[i] = data[i + 1];
        }
        --length;
    }

    void print() const {
        for (int i = 0; i < length; ++i) cout << data[i] << " ";
        cout << "\n";
    }
};
```

### 3.2. Singly Linked List (Tek Yönlü Bağlı Liste)

- Her `Node` sadece `next` pointer'ına sahiptir.
- Ortaya ekleme/silme `O(1)` (pointer biliniyorsa).

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

    void push_front(int v) {
        SinglyNode *node = new SinglyNode(v);
        node->next = head;
        head = node;
    }

    void push_back(int v) {
        SinglyNode *node = new SinglyNode(v);
        if (!head) {
            head = node;
            return;
        }
        SinglyNode *cur = head;
        while (cur->next) cur = cur->next;
        cur->next = node;
    }

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

    bool search(int v) const {
        SinglyNode *cur = head;
        while (cur) {
            if (cur->value == v) return true;
            cur = cur->next;
        }
        return false;
    }

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

### 4. Doubly Linked List (Çift Yönlü Bağlı Liste)

- Her `Node` hem `prev` hem `next` tutar.
- Geriye doğru traversal mümkündür.

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

    void push_front(int v) {
        DoublyNode *node = new DoublyNode(v);
        node->next = head;
        if (head) head->prev = node;
        head = node;
        if (!tail) tail = node;
    }

    void push_back(int v) {
        DoublyNode *node = new DoublyNode(v);
        node->prev = tail;
        if (tail) tail->next = node;
        tail = node;
        if (!head) head = node;
    }

    void pop_front() {
        if (!head) return;
        DoublyNode *tmp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete tmp;
    }

    void pop_back() {
        if (!tail) return;
        DoublyNode *tmp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete tmp;
    }

    void print_forward() const {
        DoublyNode *cur = head;
        while (cur) {
            cout << cur->value << " <-> ";
            cur = cur->next;
        }
        cout << "NULL\n";
    }

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

STL ile hazır bağlı listeler için: `std::list<int>`, `std::forward_list<int>` kullanılabilir.

---

## 5. Recursion (Özyineleme)

### 5.1. Faktöriyel

```cpp
long long factorial(int n) {
    if (n <= 1) return 1;              // base case
    return 1LL * n * factorial(n - 1); // recursive case
}
```

### 5.2. Fibonacci (Top-Down + Memoization)

```cpp
long long fib(int n, vector<long long> &memo) {
    if (n <= 1) return n;
    if (memo[n] != -1) return memo[n];
    memo[n] = fib(n - 1, memo) + fib(n - 2, memo);
    return memo[n];
}
```

### 5.3. Linked List'i Recursive Print

```cpp
void printListRecursive(SinglyNode *node) {
    if (!node) {
        cout << "NULL\n";
        return;
    }
    cout << node->value << " -> ";
    printListRecursive(node->next);
}
```

---

## 6. Stacks (Yığınlar)

### 6.1. Array-Based Stack

```cpp
class ArrayStack {
private:
    vector<int> data;

public:
    void push(int x) { data.push_back(x); }

    void pop() {
        if (!data.empty()) data.pop_back();
    }

    int top() const {
        if (data.empty()) throw runtime_error("Empty stack");
        return data.back();
    }

    bool empty() const { return data.empty(); }
    int size() const { return (int)data.size(); }
};
```

### 6.2. Linked-List Based Stack

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

### 6.3. STL `stack`

```cpp
stack<int> st;
st.push(10);
st.push(20);
int x = st.top(); // 20
st.pop();
```

---

## 7. Queues (Kuyruklar)

### 7.1. Circular Array Queue

```cpp
class CircularQueue {
private:
    vector<int> data;
    int frontIdx;
    int rearIdx;
    int count;

public:
    CircularQueue(int cap) : data(cap), frontIdx(0), rearIdx(0), count(0) {}

    bool empty() const { return count == 0; }
    bool full() const { return count == (int)data.size(); }

    void enqueue(int x) {
        if (full()) throw runtime_error("Full queue");
        data[rearIdx] = x;
        rearIdx = (rearIdx + 1) % data.size();
        ++count;
    }

    void dequeue() {
        if (empty()) throw runtime_error("Empty queue");
        frontIdx = (frontIdx + 1) % data.size();
        --count;
    }

    int front() const {
        if (empty()) throw runtime_error("Empty queue");
        return data[frontIdx];
    }
};
```

### 7.2. Linked-List Queue

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

    void enqueue(int x) {
        SinglyNode *node = new SinglyNode(x);
        if (!tail) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    void dequeue() {
        if (!head) return;
        SinglyNode *tmp = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete tmp;
    }

    int front() const {
        if (!head) throw runtime_error("Empty queue");
        return head->value;
    }
};
```

### 7.3. STL `queue`

```cpp
queue<int> q;
q.push(5);
q.push(10);
int f = q.front(); // 5
q.pop();
```

---

## 9. Searching Algorithms

### 9.1. Linear Search (Doğrusal Arama)

```cpp
int linearSearch(const vector<int> &arr, int target) {
    for (int i = 0; i < (int)arr.size(); ++i) {
        if (arr[i] == target) return i; // bulundu
    }
    return -1; // yok
}
```

Zaman karmaşıklığı: `O(n)`.

### 9.2. Binary Search (İkili Arama) – Sıralı Dizide

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

Ayrıca STL `std::binary_search`, `std::lower_bound`, `std::upper_bound` fonksiyonları kullanılabilir.

---

## 10. Hashing Algorithms (Hash Tablolar)

### 10.1. Basit Hash Tablosu (Separate Chaining)

```cpp
class HashTable {
private:
    vector<list<pair<int, string>>> table; // key -> string value

    int hashFunc(int key) const {
        return key % (int)table.size();
    }

public:
    HashTable(int size = 10) : table(size) {}

    void insert(int key, const string &value) {
        int idx = hashFunc(key);
        for (auto &p : table[idx]) {
            if (p.first == key) { // update
                p.second = value;
                return;
            }
        }
        table[idx].push_back({key, value});
    }

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

    string *search(int key) {
        int idx = hashFunc(key);
        for (auto &p : table[idx]) {
            if (p.first == key) return &p.second;
        }
        return nullptr;
    }
};
```

### 10.2. STL `unordered_map`

```cpp
unordered_map<int, string> mp;
mp[10] = "Ali";
mp[20] = "Veli";
if (mp.count(10)) cout << mp[10] << "\n";
```

---

## 11. Sorting Algorithms

Aşağıda temel sıralama algoritmalarının hepsi C++ ile verilmiştir.

### 11.1. Bubble Sort

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
        if (!swapped) break; // optimize
    }
}
```

### 11.2. Selection Sort

```cpp
void selectionSort(vector<int> &arr) {
    int n = (int)arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIdx]) minIdx = j;
        }
        if (minIdx != i) swap(arr[i], arr[minIdx]);
    }
}
```

### 11.3. Insertion Sort

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

### 11.4. Merge Sort

```cpp
void merge(vector<int> &arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
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

### 11.5. Quick Sort

```cpp
int partitionQS(vector<int> &arr, int low, int high) {
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
        int pi = partitionQS(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
```

### 11.6. Heap Sort

```cpp
void heapify(vector<int> &arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int> &arr) {
    int n = (int)arr.size();
    // Build heap
    for (int i = n / 2 - 1; i >= 0; --i) heapify(arr, n, i);
    // One by one extract
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}
```

STL ile sıralama için: `std::sort(arr.begin(), arr.end());`

---

## 12. Binary Search Trees (BST)

### 12.1. BST Node ve Sınıfı

```cpp
struct BSTNode {
    int key;
    BSTNode *left;
    BSTNode *right;
    BSTNode(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    BSTNode *root;

    BSTNode* insert(BSTNode *node, int key) {
        if (!node) return new BSTNode(key);
        if (key < node->key) node->left = insert(node->left, key);
        else if (key > node->key) node->right = insert(node->right, key);
        return node;
    }

    bool search(BSTNode *node, int key) const {
        if (!node) return false;
        if (node->key == key) return true;
        if (key < node->key) return search(node->left, key);
        return search(node->right, key);
    }

    BSTNode* findMin(BSTNode *node) {
        while (node && node->left) node = node->left;
        return node;
    }

    BSTNode* remove(BSTNode *node, int key) {
        if (!node) return nullptr;
        if (key < node->key) node->left = remove(node->left, key);
        else if (key > node->key) node->right = remove(node->right, key);
        else {
            if (!node->left) {
                BSTNode *r = node->right;
                delete node;
                return r;
            } else if (!node->right) {
                BSTNode *l = node->left;
                delete node;
                return l;
            } else {
                BSTNode *succ = findMin(node->right);
                node->key = succ->key;
                node->right = remove(node->right, succ->key);
            }
        }
        return node;
    }

    void inorder(BSTNode *node) const {
        if (!node) return;
        inorder(node->left);
        cout << node->key << " ";
        inorder(node->right);
    }

    void clear(BSTNode *node) {
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

    void printInorder() const {
        inorder(root);
        cout << "\n";
    }
};
```

---

## 13. B-Trees (Basit B-Tree Uygulaması)

B-tree çok büyük veri yapılarında (disk tabanlı) kullanılır. `t` minimum derece olmak üzere:
- Her düğüm max `2t-1` anahtar, min `t-1` anahtar tutar (root hariç).

Aşağıda **minimum derece `t`** parametreli, temel **arama ve ekleme** işlemlerini yapan sade bir B-tree gösterilmiştir.

```cpp
class BTreeNode {
public:
    bool leaf;
    int t; // min degree
    vector<int> keys;
    vector<BTreeNode*> children;

    BTreeNode(int _t, bool _leaf) : leaf(_leaf), t(_t) {
        keys.reserve(2 * t - 1);
        children.reserve(2 * t);
    }

    ~BTreeNode() {
        if (!leaf) {
            for (auto c : children) delete c;
        }
    }

    int findKey(int k) {
        int idx = 0;
        while (idx < (int)keys.size() && keys[idx] < k) ++idx;
        return idx;
    }

    void traverse() {
        int i;
        for (i = 0; i < (int)keys.size(); ++i) {
            if (!leaf && i < (int)children.size()) children[i]->traverse();
            cout << " " << keys[i];
        }
        if (!leaf && i < (int)children.size()) children[i]->traverse();
    }

    BTreeNode* search(int k) {
        int i = 0;
        while (i < (int)keys.size() && k > keys[i]) ++i;
        if (i < (int)keys.size() && keys[i] == k) return this;
        if (leaf) return nullptr;
        return children[i]->search(k);
    }

    void splitChild(int i, BTreeNode *y) {
        BTreeNode *z = new BTreeNode(y->t, y->leaf);
        // z'ye y'nin son t-1 anahtarını kopyala
        for (int j = 0; j < t - 1; ++j) {
            z->keys.push_back(y->keys[j + t]);
        }
        if (!y->leaf) {
            for (int j = 0; j < t; ++j) {
                z->children.push_back(y->children[j + t]);
            }
        }
        // y'nin boyutunu küçült
        y->keys.resize(t - 1);
        if (!y->leaf) y->children.resize(t);

        // Bu düğümün çocuk vektörüne z'yi ekle
        children.insert(children.begin() + i + 1, z);

        // Ortadaki anahtarı bu düğüme taşı
        keys.insert(keys.begin() + i, y->keys[t - 1]);
    }

    void insertNonFull(int k) {
        int i = (int)keys.size() - 1;
        if (leaf) {
            keys.push_back(0); // yer aç
            while (i >= 0 && keys[i] > k) {
                keys[i + 1] = keys[i];
                --i;
            }
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
    int t; // min degree

public:
    BTree(int _t) : root(nullptr), t(_t) {}

    ~BTree() { delete root; }

    void traverse() {
        if (root) root->traverse();
        cout << "\n";
    }

    BTreeNode* search(int k) {
        return (root ? root->search(k) : nullptr);
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
            int i = 0;
            if (s->keys[0] < k) ++i;
            s->children[i]->insertNonFull(k);
            root = s;
        } else {
            root->insertNonFull(k);
        }
    }
};
```

Bu B-tree implementasyonu **arama ve ekleme** için yeterlidir.

---

## 14. Graphs (Graf Yapıları)

### 14.1. Adjacency List Tabanlı Yönsüz Graf

```cpp
class Graph {
private:
    int V; // vertex sayısı
    vector<vector<int>> adj; // adjacency list

public:
    Graph(int n) : V(n), adj(n) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // yönsüz
    }

    void BFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
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

    void DFSUtil(int u, vector<bool> &visited) {
        visited[u] = true;
        cout << u << " ";
        for (int v : adj[u]) {
            if (!visited[v]) DFSUtil(v, visited);
        }
    }

    void DFS(int start) {
        vector<bool> visited(V, false);
        DFSUtil(start, visited);
        cout << "\n";
    }
};
```

### 14.2. Ağırlıklı Graf ve Dijkstra (Kısa Yol) – STL Kullanarak

```cpp
class WeightedGraph {
private:
    int V;
    vector<vector<pair<int,int>>> adj; // (komşu, ağırlık)

public:
    WeightedGraph(int n) : V(n), adj(n) {}

    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // yönsüz için
    }

    vector<int> dijkstra(int src) {
        const int INF = 1e9;
        vector<int> dist(V, INF);
        dist[src] = 0;
        using P = pair<int,int>; // (distance, node)
        priority_queue<P, vector<P>, greater<P>> pq;
        pq.push({0, src});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d != dist[u]) continue;
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

---

## Örnek `main` Fonksiyonu (Hepsini Test Etmek İçin Basit Şablon)

Aşağıdaki `main` içinde yukarıdaki sınıflardan bazılarını deneyebilirsiniz. Hepsini **tek dosyaya** koyarsanız, `#include <bits/stdc++.h>` en başta olsun ve `using namespace std;` kullanmayı unutmayın.

```cpp
int main() {
    // ArrayList
    ArrayList al;
    al.push_back(3);
    al.push_back(1);
    al.push_back(4);
    al.insert(1, 2);
    al.print();

    // Linked list
    SinglyLinkedList sll;
    sll.push_back(10);
    sll.push_back(20);
    sll.push_front(5);
    sll.print();

    // Recursion
    cout << "factorial(5) = " << factorial(5) << "\n";

    // Stack
    ArrayStack st;
    st.push(1); st.push(2); st.push(3);
    cout << "Stack top = " << st.top() << "\n";

    // Queue
    CircularQueue cq(5);
    cq.enqueue(10);
    cq.enqueue(20);
    cout << "Queue front = " << cq.front() << "\n";

    // Sorting
    vector<int> arr = {5, 2, 9, 1, 5, 6};
    quickSort(arr, 0, (int)arr.size() - 1);
    for (int x : arr) cout << x << " ";
    cout << "\n";

    // BST
    BinarySearchTree bst;
    bst.insert(50);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    bst.printInorder();

    // B-Tree
    BTree bt(2);
    bt.insert(10);
    bt.insert(20);
    bt.insert(5);
    bt.insert(6);
    bt.insert(12);
    bt.insert(30);
    bt.insert(7);
    bt.insert(17);
    bt.traverse();
    cout << "\n";

    // Graph
    Graph g(5);
    g.addEdge(0,1); g.addEdge(0,4); g.addEdge(1,2); g.addEdge(1,3); g.addEdge(1,4);
    cout << "BFS from 0: "; g.BFS(0);
    cout << "DFS from 0: "; g.DFS(0);

    return 0;
}
```

Bu dosyadaki tüm kodları istersen `DSAFinalPrep.cpp` içine kopyalayıp derleyebilir veya ayrı ayrı dosyalarda kullanabilirsin.
