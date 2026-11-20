// Q4 - Red-Black Tree (process scheduler) + rotation comparison with AVL
#include <bits/stdc++.h>
using namespace std;
struct R { int k; bool red = 1; R *c[2] = {0, 0}, *p = 0; };
R *root, *mn; long long rbRot;
void rot(R *x, int d) {                       // d=0: left rotation, d=1: right rotation
    R *y = x->c[!d]; x->c[!d] = y->c[d]; if (y->c[d]) y->c[d]->p = x;
    y->p = x->p;
    if (!x->p) root = y; else x->p->c[x == x->p->c[1]] = y;
    y->c[d] = x; x->p = y; rbRot++;
}
void insert(int k) {
    R *z = new R{k}, *y = 0, *x = root;
    while (x) { y = x; x = x->c[k > x->k]; }
    z->p = y; if (!y) root = z; else y->c[k > y->k] = z;
    if (!mn || k < mn->k) mn = z;                           // cached leftmost = next process
    while (z->p && z->p->red) {
        R *g = z->p->p; int d = (z->p == g->c[1]); R *u = g->c[!d];
        if (u && u->red) { z->p->red = u->red = 0; g->red = 1; z = g; }       // recolour only
        else {
            if (z == z->p->c[!d]) { z = z->p; rot(z, d); }
            z->p->red = 0; g->red = 1; rot(g, !d);
        }
    }
    root->red = 0;
}
int ht(R *n) { return n ? 1 + max(ht(n->c[0]), ht(n->c[1])) : 0; }
int chk(R *n) {                                // black-height, or -1 if a property fails
    if (!n) return 1;
    if (n->red && ((n->c[0] && n->c[0]->red) || (n->c[1] && n->c[1]->red))) return -1;
    int a = chk(n->c[0]), b = chk(n->c[1]);
    return (a < 0 || a != b) ? -1 : a + !n->red;
}
void show(R *n) { if (n) { show(n->c[0]); printf("%d(%c) ", n->k, n->red ? 'R' : 'B'); show(n->c[1]); } }
void reset() { root = mn = 0; rbRot = 0; }
struct A { int k, h; A *l, *r; }; long long aRot;       // compact AVL, only to count rotations
int ah(A *n) { return n ? n->h : 0; }
void af(A *n) { n->h = 1 + max(ah(n->l), ah(n->r)); }
A *aR(A *y) { A *x = y->l; y->l = x->r; x->r = y; af(y); af(x); aRot++; return x; }
A *aL(A *x) { A *y = x->r; x->r = y->l; y->l = x; af(x); af(y); aRot++; return y; }
A *ains(A *n, int k) {
    if (!n) return new A{k, 1, 0, 0};
    if (k < n->k) n->l = ains(n->l, k); else n->r = ains(n->r, k);
    af(n); int b = ah(n->l) - ah(n->r);
    if (b > 1 && k < n->l->k) return aR(n);
    if (b < -1 && k > n->r->k) return aL(n);
    if (b > 1) { n->l = aL(n->l); return aR(n); }
    if (b < -1) { n->r = aR(n->r); return aL(n); }
    return n;
}
int main() {
    for (int k : {10, 20, 30, 15, 25, 5}) insert(k);
    cout << "Inorder with colours after inserting {10,20,30,15,25,5}:\n  "; show(root);
    printf("\nRoot = %d(%c)\n", root->k, root->red ? 'R' : 'B');
    reset();
    const int N = 10000; vector<int> ks(N); iota(ks.begin(), ks.end(), 1);
    cout << "\n=== Red-Black Tree Investigation (N = 10,000) ===\n";
    for (int k : ks) insert(k);
    printf("Sorted Insertions -> Height: %d | Rotations: %lld\n", ht(root), rbRot); reset();
    srand(5); for (int i = N - 1; i > 0; i--) swap(ks[i], ks[rand() % (i + 1)]);
    for (int k : ks) insert(k);
    long long rr = rbRot;
    printf("Random Insertions -> Height: %d | Rotations: %lld\nSmallest Process (Leftmost Node): %d\n", ht(root), rr, mn->k);
    int bh = chk(root);
    if (!root->red && bh > 0) printf("All 5 Red-Black Properties Verified: PASSED (Black-Height = %d)\n", bh - 1);
    else puts("Property check: FAILED");
    A *a = 0; for (int k : ks) a = ains(a, k);
    printf("\n=== Rotation Comparison (random order, N = 10,000) ===\nRed-Black rotations: %lld | AVL rotations: %lld | Red-Black does %.0f%% fewer\n", rr, aRot, 100.0 * (aRot - rr) / aRot);
}
