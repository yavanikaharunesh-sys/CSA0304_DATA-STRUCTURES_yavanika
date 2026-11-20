// Q3 - B-Tree (min degree t; each node holds t-1 .. 2t-1 keys = one disk block)
#include <bits/stdc++.h>
using namespace std;
struct B { bool leaf; vector<int> k; vector<B *> c; };
struct BTree {
    int t; B *root;
    BTree(int t) : t(t), root(new B{true}) {}
    void split(B *p, int i) {                                   // median moves up
        B *y = p->c[i], *z = new B{y->leaf};
        int m = y->k[t - 1];
        z->k.assign(y->k.begin() + t, y->k.end()); y->k.resize(t - 1);
        if (!y->leaf) { z->c.assign(y->c.begin() + t, y->c.end()); y->c.resize(t); }
        p->k.insert(p->k.begin() + i, m); p->c.insert(p->c.begin() + i + 1, z);
    }
    void add(B *x, int v) {
        int i = upper_bound(x->k.begin(), x->k.end(), v) - x->k.begin();
        if (x->leaf) { x->k.insert(x->k.begin() + i, v); return; }
        if ((int)x->c[i]->k.size() == 2 * t - 1) { split(x, i); if (v > x->k[i]) i++; }
        add(x->c[i], v);
    }
    void insert(int v) {
        if ((int)root->k.size() == 2 * t - 1) { B *s = new B{false}; s->c.push_back(root); root = s; split(s, 0); }
        add(root, v);
    }
    bool find(int v, int &reads) {
        reads = 0;
        for (B *x = root; x; ) {
            reads++; int i = lower_bound(x->k.begin(), x->k.end(), v) - x->k.begin();
            if (i < (int)x->k.size() && x->k[i] == v) return 1;
            x = x->leaf ? 0 : x->c[i];
        }
        return 0;
    }
    int height() { int h = 1; for (B *x = root; !x->leaf; x = x->c[0]) h++; return h; }
    void print() {
        vector<B *> cur{root};
        for (int lv = 0; !cur.empty(); lv++) {
            cout << "  Level " << lv << ": "; vector<B *> nx;
            for (B *n : cur) {
                cout << "["; for (size_t i = 0; i < n->k.size(); i++) cout << n->k[i] << (i + 1 < n->k.size() ? " " : ""); cout << "] ";
                nx.insert(nx.end(), n->c.begin(), n->c.end());
            }
            cout << "\n"; cur = nx;
        }
    }
};
int main() {
    BTree s(2);                                                  // order 4
    for (int k : {10, 20, 5, 6, 12, 30, 7, 17}) s.insert(k);
    cout << "=== B-Tree (Order 4) Final Structure ===\n"; s.print();
    cout << "\n=== B-Tree (Order 4) Search Verification ===\n";
    int r;
    for (int k : {5, 30, 15}) { bool f = s.find(k, r); printf("Search %-2d -> Found: %-3s | Disk Reads (Nodes visited): %d\n", k, f ? "YES" : "NO", r); }

    const int N = 5000000;
    cout << "\n=== 5,000,000 Records Disk Simulation (10 ms / read) ===\n";
    BTree big(50);                                               // order 100
    for (int i = 1; i <= N; i++) big.insert(i);
    int hb = big.height(), ha = ceil(log2(N + 1.0));
    vector<int> key(N), L(N, -1), R(N, -1), p(N), dep(N, 1); iota(p.begin(), p.end(), 0);
    srand(11);
    for (int i = N - 1; i > 0; i--) swap(p[i], p[((long long)rand() * 32768 + rand()) % (i + 1)]);
    int hr = 1;
    for (int i = 0; i < N; i++) {                                // random BST, track depth
        key[i] = p[i]; if (!i) continue;
        int cur = 0, d = 2;
        for (;; d++) { int &nx = key[i] < key[cur] ? L[cur] : R[cur]; if (nx < 0) { nx = i; break; } cur = nx; }
        hr = max(hr, d);
    }
    printf("B-Tree (Order 100) -> Height: %d | Disk Reads: %d | Search Time: %d ms\n", hb, hb, hb * 10);
    printf("AVL Tree           -> Height: %d | Disk Reads: %d | Search Time: %d ms\n", ha, ha, ha * 10);
    printf("Random BST         -> Height: %d | Disk Reads: %d | Search Time: %d ms\n", hr, hr, hr * 10);
}
