// Q5 - Splay Tree (Zig, Zig-Zig, Zig-Zag)
#include <bits/stdc++.h>
using namespace std;
struct S { int k; S *c[2] = {0, 0}, *p = 0; };
S *root;
void up(S *x) {                                   // rotate x above its parent
    S *p = x->p, *g = p->p; int d = (x == p->c[1]);
    p->c[d] = x->c[!d]; if (x->c[!d]) x->c[!d]->p = p;
    x->c[!d] = p; p->p = x; x->p = g;
    if (g) g->c[g->c[1] == p] = x;
}
void splay(S *x) {
    while (x->p) {
        S *p = x->p, *g = p->p;
        if (!g) up(x);                                              // Zig
        else if ((g->c[0] == p) == (p->c[0] == x)) { up(p); up(x); } // Zig-Zig
        else { up(x); up(x); }                                      // Zig-Zag
    }
    root = x;
}
void insert(int k) {
    S *z = new S{k}, *y = 0, *x = root;
    while (x) { y = x; x = x->c[k > x->k]; }
    z->p = y; if (!y) root = z; else y->c[k > y->k] = z;
    splay(z);
}
void access(int k, int &v) {
    S *x = root, *last = 0; v = 0;
    while (x) { v++; last = x; if (k == x->k) break; x = x->c[k > x->k]; }
    if (last) splay(last);
}
void fresh() { root = 0; for (int i = 1; i <= 1000; i++) insert(i); }      // sorted inserts -> long chain
int main() {
    const int N = 1000, A = 10000, hot = N / 5; srand(3);
    vector<int> sk, un;
    for (int i = 0; i < A; i++) {
        sk.push_back(rand() % 100 < 80 ? 1 + rand() % hot : hot + 1 + rand() % (N - hot));
        un.push_back(1 + rand() % N);
    }
    int v; fresh();
    cout << "First 10 roots during skewed accesses: ";
    for (int i = 0; i < 10; i++) { access(sk[i], v); cout << root->k << (i < 9 ? " " : "\n"); }
    cout << "\n=== Splay Tree Workload Simulation (1,000 items, 10,000 accesses) ===\n";
    for (int w = 0; w < 2; w++) {
        fresh(); long long tot = 0; int worst = 0;
        for (int k : w ? un : sk) { access(k, v); tot += v; worst = max(worst, v); }
        if (!w) printf("Skewed Workload (80/20 rule) -> Avg Nodes Visited: %.2f  | Worst Single Access: %d\n", (double)tot / A, worst);
        else    printf("Uniform Workload             -> Avg Nodes Visited: %.2f\n", (double)tot / A);
    }
}
