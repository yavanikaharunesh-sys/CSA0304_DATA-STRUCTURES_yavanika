// Q2 - AVL Trees with rotation counting
#include <bits/stdc++.h>
using namespace std;
struct N { int k, h = 1; N *l = 0, *r = 0; };
long long rots = 0;
int H(N *n) { return n ? n->h : 0; }
int bf(N *n) { return H(n->l) - H(n->r); }
void fix(N *n) { n->h = 1 + max(H(n->l), H(n->r)); }
N *rotR(N *y) { N *x = y->l; y->l = x->r; x->r = y; fix(y); fix(x); rots++; return x; }   // TODO 1
N *rotL(N *x) { N *y = x->r; x->r = y->l; y->l = x; fix(x); fix(y); rots++; return y; }   // TODO 2
N *ins(N *n, int k) {
    if (!n) return new N{k};
    if (k < n->k) n->l = ins(n->l, k); else if (k > n->k) n->r = ins(n->r, k); else return n;
    fix(n); int b = bf(n);
    if (b > 1 && k < n->l->k) return rotR(n);                                  // LL
    if (b < -1 && k > n->r->k) return rotL(n);                                 // RR
    if (b > 1) { n->l = rotL(n->l); return rotR(n); }                          // LR
    if (b < -1) { n->r = rotR(n->r); return rotL(n); }                         // RL
    return n;
}
void pre(N *n) { if (n) { cout << n->k << " "; pre(n->l); pre(n->r); } }
bool ok(N *n) { return !n || (abs(bf(n)) <= 1 && ok(n->l) && ok(n->r)); }
bool has(N *n, int k) { while (n) { if (k == n->k) return 1; n = k < n->k ? n->l : n->r; } return 0; }
double ms(chrono::steady_clock::time_point t) { return chrono::duration<double, milli>(chrono::steady_clock::now() - t).count(); }

int main() {
    N *r = 0; for (int k : {10, 20, 30, 40, 50, 25}) r = ins(r, k);
    pre(r); cout << "\nheight = " << H(r) << "\n";
    const int n = 100000; vector<int> ks(n);
    iota(ks.begin(), ks.end(), 1);
    cout << "\n=== AVL Tree Investigation (N = 100,000) ===\n";
    bool good = true; int hit = 0;
    for (int pass = 0; pass < 2; pass++) {
        if (pass) { srand(7); for (int i = n - 1; i > 0; i--) swap(ks[i], ks[rand() % (i + 1)]); }
        rots = 0; r = 0; auto t0 = chrono::steady_clock::now();
        for (int k : ks) r = ins(r, k);
        double ti = ms(t0); t0 = chrono::steady_clock::now();
        for (int k : ks) hit += has(r, k);
        double ts = ms(t0);
        if (!pass) printf("Sorted Insertions -> Height: %d (Plain BST = %d)\nSorted Insertions -> Rotations: %lld | Insert Time: %.2f ms | Search Time: %.2f ms\n", H(r), n, rots, ti, ts);
        else printf("Random Insertions -> Height: %d | Rotations: %lld | Insert Time: %.2f ms | Search Time: %.2f ms\n", H(r), rots, ti, ts);
        good &= ok(r) && hit > 0;
    }
    cout << "Balance Factor Check (All nodes in [-1, 1]): " << (good ? "PASSED" : "FAILED") << "\n";
}
