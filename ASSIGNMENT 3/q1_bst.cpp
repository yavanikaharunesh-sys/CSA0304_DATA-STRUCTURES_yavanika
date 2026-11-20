// Q1 - Binary Trees and BST (accession numbers)
#include <bits/stdc++.h>
using namespace std;
struct N { int k; N *l = 0, *r = 0; };
N *ins(N *t, int k) {
    if (!t) return new N{k};
    if (k < t->k) t->l = ins(t->l, k); else if (k > t->k) t->r = ins(t->r, k);
    return t;
}
int ht(N *t) { return t ? 1 + max(ht(t->l), ht(t->r)) : -1; }
N *del(N *t, int k) {                          // 3 deletion cases
    if (!t) return 0;
    if (k < t->k) t->l = del(t->l, k);
    else if (k > t->k) t->r = del(t->r, k);
    else if (!t->l) return t->r;               // leaf / only right child
    else if (!t->r) return t->l;               // only left child
    else { N *s = t->r; while (s->l) s = s->l; // two children: inorder successor
           t->k = s->k; t->r = del(t->r, s->k); }
    return t;
}
int srch(N *t, int k, int &c) { c = 0; while (t) { c++; if (k == t->k) return 1; t = k < t->k ? t->l : t->r; } return 0; }
void in(N *t, vector<int> &v)  { if (t) { in(t->l, v); v.push_back(t->k); in(t->r, v); } }
void pre(N *t, vector<int> &v) { if (t) { v.push_back(t->k); pre(t->l, v); pre(t->r, v); } }
void pr(const vector<int> &v) { for (size_t i = 0; i < v.size(); i++) cout << v[i] << (i + 1 < v.size() ? " " : ""); cout << "\n"; }
string T = " ABCDE.F";                          // array tree, 1-indexed, '.' = empty
void tr(int i, int m, string &o) {              // m: 0 pre, 1 in, 2 post
    if (i >= (int)T.size() || T[i] == '.') return;
    if (m == 0) o += T[i];
    tr(2 * i, m, o); if (m == 1) o += T[i]; tr(2 * i + 1, m, o); if (m == 2) o += T[i];
}
double avg(N *t, const vector<int> &ks) { long long s = 0; int c; for (int k : ks) { srch(t, k, c); s += c; } return (double)s / ks.size(); }

int main() {
    N *a = 0, *b = 0; vector<int> v;
    for (int k : {500, 300, 700, 200, 400, 600, 800, 350}) a = ins(a, k);
    for (int k = 10; k <= 80; k += 10) b = ins(b, k);
    in(a, v); pr(v);
    int c1, c2; srch(a, 350, c1); srch(b, 80, c2);
    cout << "height: random order " << ht(a) << ", sorted order " << ht(b) << "\ncomparisons (deepest key): " << c1 << " and " << c2 << "\n\n";

    cout << "=== Predict Output Programs ===\n";
    string p, i, q, lv;
    tr(1, 0, p); tr(1, 1, i); tr(1, 2, q);
    for (char ch : T) if (ch != '.' && ch != ' ') lv += ch;              // level order
    cout << "Program 1 (Array T=\"ABCDE.F\") : " << p << " " << i << " " << q << " " << lv << "\n";
    N *d = 0; vector<int> keys = {50, 30, 20, 40, 70, 60, 65, 80};
    for (int k : keys) d = ins(d, k);
    for (int k : {20, 30, 50}) d = del(d, k);
    v.clear(); pre(d, v); cout << "Program 2 (Deletions 20,30,50) : "; pr(v);

    cout << "\n=== 2,000 Keys Experimental Investigation ===\n";
    vector<int> ks(2000), sh; iota(ks.begin(), ks.end(), 1);
    N *s = 0, *r = 0; for (int k : ks) s = ins(s, k);
    srand(42); sh = ks; for (int k = 1999; k > 0; k--) swap(sh[k], sh[rand() % (k + 1)]);
    for (int k : sh) r = ins(r, k);
    printf("Sorted Order -> Height: %d | Avg Comparisons: %.2f\n", ht(s), avg(s, ks));
    printf("Random Order -> Height: %d   | Avg Comparisons: %.2f\n", ht(r), avg(r, ks));

    N *f = 0; for (int k : keys) f = ins(f, k);
    v.clear(); pre(f, v);
    ofstream("tree_preorder.txt") << [&] { string z; for (int k : v) z += to_string(k) + "\n"; return z; }();
    ifstream fi("tree_preorder.txt"); N *rb = 0; int k;
    while (fi >> k) rb = ins(rb, k);
    v.clear(); in(rb, v); cout << "Rebuilt Inorder (from Preorder file): "; pr(v);
    N *e = 0; int ce; cout << "Empty tree height: " << ht(e) << ", search(10): " << srch(e, 10, ce) << "\n";
    d = ins(d, 70); v.clear(); in(d, v); cout << "After inserting duplicate 70: "; pr(v);
}
