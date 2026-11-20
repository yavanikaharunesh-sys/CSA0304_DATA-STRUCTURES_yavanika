// Q6 - Trie for phone contacts: prefix count, autocomplete, delete, benchmark
#include <bits/stdc++.h>
using namespace std;
struct Trie { Trie *ch[26] = {}; int words = 0, end = 0; };
long long cnt = 0;
void insert(Trie *t, const char *w) {                       // TODO 1
    for (int i = 0; w[i]; i++) {
        int x = w[i] - 'a';
        if (!t->ch[x]) { t->ch[x] = new Trie; cnt++; }
        t = t->ch[x]; t->words++;
    }
    t->end = 1;
}
int count_prefix(const Trie *t, const char *p) {            // TODO 2
    for (int i = 0; p[i]; i++) { t = t->ch[p[i] - 'a']; if (!t) return 0; }
    return t->words;
}
const Trie *walk(const Trie *t, const string &p) { for (char c : p) { t = t->ch[c - 'a']; if (!t) return 0; } return t; }
void collect(const Trie *t, string s, vector<string> &o) {
    if (t->end) o.push_back(s);
    for (int i = 0; i < 26; i++) if (t->ch[i]) collect(t->ch[i], s + char('a' + i), o);
}
vector<string> complete(const Trie *t, const string &p) { vector<string> o; if ((t = walk(t, p))) collect(t, p, o); return o; }
void remove(Trie *t, const string &w) {
    const Trie *n = walk(t, w); if (!n || !n->end) return;
    for (char c : w) {
        Trie *nx = t->ch[c - 'a'];
        if (--nx->words == 0) { t->ch[c - 'a'] = 0; cnt--; return; }
        t = nx;
    }
    t->end = 0;
}
void show(const char *lab, const vector<string> &v) {
    cout << lab; if (v.empty()) cout << "(no matches)";
    for (size_t i = 0; i < v.size(); i++) cout << v[i] << (i + 1 < v.size() ? " " : ""); cout << "\n";
}
int main() {
    Trie *r = new Trie;
    for (const char *w : {"anand", "anitha", "anu", "arun", "bala", "anusuya"}) insert(r, w);
    printf("a:%d an:%d anu:%d b:%d z:%d\n", count_prefix(r, "a"), count_prefix(r, "an"), count_prefix(r, "anu"), count_prefix(r, "b"), count_prefix(r, "z"));
    printf("Nodes created for 6 contacts: %lld (%lld bytes)\n", cnt, cnt * (long long)sizeof(Trie));
    cout << "\n=== Autocomplete & Deletion Verification ===\n";
    show("Autocomplete 'an' : ", complete(r, "an"));
    show("Autocomplete 'anu': ", complete(r, "anu"));
    show("Empty prefix ''   : ", complete(r, ""));
    show("No match 'xyz'    : ", complete(r, "xyz"));
    remove(r, "anu"); show("After deleting 'anu', autocomplete 'an': ", complete(r, "an"));

    cout << "\n=== Scalability & Memory Benchmark ===\n"; srand(9);
    for (int n : {500, 5000, 50000}) {
        cnt = 0; Trie *t = new Trie; vector<string> names;
        for (int i = 0; i < n; i++) {
            string s; int len = 6 + rand() % 5; for (int j = 0; j < len; j++) s += char('a' + rand() % 26);
            names.push_back(s); insert(t, s.c_str());
        }
        auto t0 = chrono::steady_clock::now(); long long sink = 0;
        for (int rep = 0; rep < 20; rep++) for (auto &s : names) sink += count_prefix(t, s.c_str());
        double us = chrono::duration<double, micro>(chrono::steady_clock::now() - t0).count() / (20.0 * n);
        printf("N = %-5d | Avg Lookup: %.3f us | Trie Nodes: %-6lld (%.2f MB) | Sorted Array: %.1f KB%s\n", n, us, cnt, cnt * (double)sizeof(Trie) / 1048576, n * 16.0 / 1024, sink < 0 ? "!" : "");
    }
}
