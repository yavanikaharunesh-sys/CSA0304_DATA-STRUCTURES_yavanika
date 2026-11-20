// Q7 - Ternary Search Tree dictionary: lookup, autocomplete, range, fuzzy spell-check
#include <bits/stdc++.h>
using namespace std;
struct T { char ch; bool end = 0; T *lo = 0, *eq = 0, *hi = 0; };
long long nodes = 0;
T *ins(T *n, const string &w, size_t i) {
    if (!n) { n = new T{w[i]}; nodes++; }
    if (w[i] < n->ch) n->lo = ins(n->lo, w, i);
    else if (w[i] > n->ch) n->hi = ins(n->hi, w, i);
    else if (i + 1 < w.size()) n->eq = ins(n->eq, w, i + 1);
    else n->end = 1;
    return n;
}
T *walk(T *n, const string &w, size_t i = 0) {              // node of last char of w (or null)
    while (n) {
        if (w[i] < n->ch) n = n->lo; else if (w[i] > n->ch) n = n->hi;
        else if (++i == w.size()) return n; else n = n->eq;
    }
    return 0;
}
void collect(T *n, string pre, vector<string> &o, const string &a = "", const string &b = "~") {   // in-order = alphabetical
    if (!n) return;
    collect(n->lo, pre, o, a, b);
    string s = pre + n->ch; if (n->end && s >= a && s <= b) o.push_back(s);
    collect(n->eq, s, o, a, b);
    collect(n->hi, pre, o, a, b);
}
void fuzzy(T *n, const string &w, const vector<int> &prev, const string &pre, int k, vector<string> &o) {
    if (!n) return;
    fuzzy(n->lo, w, prev, pre, k, o);
    vector<int> cur(w.size() + 1, 0); cur[0] = prev[0] + 1;   // Levenshtein row carried down the tree
    for (size_t j = 1; j <= w.size(); j++) cur[j] = min({prev[j] + 1, cur[j - 1] + 1, prev[j - 1] + (w[j - 1] != n->ch)});
    string s = pre + n->ch; if (n->end && cur.back() <= k) o.push_back(s);
    if (*min_element(cur.begin(), cur.end()) <= k) fuzzy(n->eq, w, cur, s, k, o);
    fuzzy(n->hi, w, prev, pre, k, o);
}
vector<string> spell(T *r, const string &w, int k) {
    vector<int> row(w.size() + 1); iota(row.begin(), row.end(), 0);
    vector<string> o; fuzzy(r, w, row, "", k, o); return o;
}
void show(const vector<string> &v, size_t lim = 6) { for (size_t i = 0; i < v.size() && i < lim; i++) cout << v[i] << " "; }
int main() {
    T *r = 0; vector<string> ws; string L = "abcdefghij";
    for (char a : L) for (char b : L) for (char c : L) ws.push_back(string("kural_") + a + b + c);   // 1,000 words
    for (int i = ws.size() - 1; i > 0; i--) swap(ws[i], ws[rand() % (i + 1)]);
    for (auto &w : ws) r = ins(r, w, 0);

    cout << "=== Regional Dictionary Verification (1,000 Sample Words) ===\n";
    auto t0 = chrono::steady_clock::now(); T *f = 0;
    for (int i = 0; i < 100000; i++) f = walk(r, "kural_ebc");
    double us = chrono::duration<double, micro>(chrono::steady_clock::now() - t0).count() / 100000;
    printf("Exact Lookup ('kural_ebc') : %s (Avg Time: %.3f us)\n", f && f->end ? "FOUND" : "NOT FOUND", us);
    T *z = walk(r, "kural_zzz"); printf("Exact Lookup ('kural_zzz') : %s\n", z && z->end ? "FOUND" : "NOT FOUND");
    vector<string> v; T *p = walk(r, "kural_ab"); if (p) collect(p->eq, "kural_ab", v);
    cout << "Autocomplete ('kural_ab')  : "; show(v); printf("(total %zu matches)\n", v.size());
    v.clear(); collect(r, "", v, "kural_ba", "kural_bf");
    cout << "Range ['kural_ba'..'kural_bf']: "; show(v); printf("(%zu words in range)\n", v.size());
    v = spell(r, "kural_ebx", 1);
    printf("Spell-check typo 'kural_ebx' (1 edit): %zu suggestions: ", v.size()); show(v, 4); cout << "...\n";
    printf("Spell-check 'kural_ab' (1 missing letter): %zu suggestions\n", spell(r, "kural_ab", 1).size());
    double kb = nodes * (double)sizeof(T) / 1024;
    printf("Memory (1,000 words): %.2f KB (%lld nodes) -> Projected 200,000 words: ~%.2f MB\n", kb, nodes, kb * 200 / 1024);
}
