#include <bits/stdc++.h>
using namespace std;

#ifdef LOCAL
#define DEBUG(...) debug(#__VA_ARGS__, __VA_ARGS__)
#else
#define DEBUG(...) 6
#endif

template<typename T, typename S> ostream& operator << (ostream &os, const pair<T, S> &p) {return os << "(" << p.first << ", " << p.second << ")";}
template<typename C, typename T = decay<decltype(*begin(declval<C>()))>, typename enable_if<!is_same<C, string>::value>::type* = nullptr>
ostream& operator << (ostream &os, const C &c) {bool f = true; os << "["; for (const auto &x : c) {if (!f) os << ", "; f = false; os << x;} return os << "]";}
template<typename T> void debug(string s, T x) {cerr << "\033[1;35m" << s << "\033[0;32m = \033[33m" << x << "\033[0m\n";}
template<typename T, typename... Args> void debug(string s, T x, Args... args) {for (int i=0, b=0; i<(int)s.size(); i++) if (s[i] == '(' || s[i] == '{') b++; else
if (s[i] == ')' || s[i] == '}') b--; else if (s[i] == ',' && b == 0) {cerr << "\033[1;35m" << s.substr(0, i) << "\033[0;32m = \033[33m" << x << "\033[31m | "; debug(s.substr(s.find_first_not_of(' ', i + 1)), args...); break;}}

const int MAX = 1e5 + 5;

struct Node {
    int sz, p, ch[2];
    int val, mn, mx, sum, lazySet, lazyAdd;
    bool flip, lazy;

    Node() : sz(0), p(0), ch(), val(0), mn(INT_MAX), mx(INT_MIN), sum(0), lazyAdd(0), flip(false), lazy(false) {}

    Node(int _val) : Node() {
        sz = 1;
        val = mn = mx = sum = _val;
    }
} tr[MAX];

void push(int u) {
    if (!u)
        return;
    if (tr[u].flip) {
        swap(tr[u].ch[0], tr[u].ch[1]);
        tr[tr[u].ch[0]].flip ^= true;
        tr[tr[u].ch[1]].flip ^= true;
        tr[u].flip = false;
    }
    if (tr[u].lazy) {
        tr[u].val = tr[u].mn = tr[u].mx = tr[u].lazySet;
        tr[u].sum = tr[u].lazySet * tr[u].sz;
        for (int i=0; i<2; i++) {
            int v = tr[u].ch[i];
            if (v) {
                tr[v].lazySet = tr[u].lazySet;
                tr[v].lazyAdd = 0;
                tr[v].lazy = true;
            }
        }
        tr[u].lazy = false;
    }
    if (tr[u].lazyAdd) {
        tr[u].val += tr[u].lazyAdd;
        tr[u].mn += tr[u].lazyAdd;
        tr[u].mx += tr[u].lazyAdd;
        tr[u].sum += tr[u].lazyAdd * tr[u].sz;
        tr[tr[u].ch[0]].lazyAdd += tr[u].lazyAdd;
        tr[tr[u].ch[1]].lazyAdd += tr[u].lazyAdd;
        tr[u].lazyAdd = 0;
    }
}

void pull(int u) {
    int l = tr[u].ch[0], r = tr[u].ch[1];
    push(l);
    push(r);
    tr[u].sz = tr[l].sz + tr[r].sz + 1;
    tr[u].mn = min({tr[l].mn, tr[r].mn, tr[u].val});
    tr[u].mx = max({tr[l].mx, tr[r].mx, tr[u].val});
    tr[u].sum = tr[l].sum + tr[r].sum + tr[u].val;
}

void attach(int u, int d, int v) {
    tr[u].ch[d] = v;
    tr[v].p = u;
    pull(u);
}

int dir(int u) {
    int v = tr[u].p;
    return tr[v].ch[0] == u ? 0 : tr[v].ch[1] == u ? 1 : -1;
}

void rotate(int u) {
    int v = tr[u].p, w = tr[v].p, du = dir(u), dv = dir(v);
    attach(v, du, tr[u].ch[!du]);
    attach(u, !du, v);
    if (~dv)
        attach(w, dv, u);
    else
        tr[u].p = w;
}

void splay(int u) {
    push(u);
    while (~dir(u)) {
        int v = tr[u].p, w = tr[v].p;
        push(w);
        push(v);
        push(u);
        int du = dir(u), dv = dir(v);
        if (~dv)
            rotate(du == dv ? v : u);
        rotate(u);
    }
}

int access(int u) {
    int v = u;
    splay(u);
    attach(u, 1, 0);
    while (tr[u].p) {
        v = tr[u].p;
        splay(v);
        attach(v, 1, u);
        splay(u);
    }
    return v;
}

void reroot(int u) {
    access(u);
    tr[u].flip = true;
    push(u);
}

void link(int u, int v) {
    reroot(u);
    access(v);
    tr[u].p = v;
}

void cut(int u, int v) {
    reroot(u);
    access(v);
    tr[v].ch[0] = tr[u].p = 0;
    pull(v);
}

int lca(int u, int v) {
    access(u);
    return access(v);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;
    for (int i=1; i<=n; i++) {
        int w;
        cin >> w;
        tr[i] = Node(w);
    }
    for (int i=0; i<n-1; i++) {
        int x, y;
        cin >> x >> y;
        link(x, y);
    }
    int rt;
    cin >> rt;
    for (int i=0; i<m; i++) {
        int k;
        cin >> k;
        if (k == 0) {
            cin >> rt;
        } else {
            int x, y;
            cin >> x >> y;
            if (k == 6) {
                reroot(rt);
                if (lca(y, x) != x) {
                    int p = tr[x].ch[0];
                    cut(x, p);
                    link(x, y);
                }
            } else if (k == 7) {
                reroot(rt);
                cout << lca(x, y) << "\n";
            } else {
                reroot(x);
                access(y);
                if (k == 1 || k == 2) {
                    int z;
                    cin >> z;
                    if (k == 1) {
                        tr[y].lazySet = z;
                        tr[y].lazy = true;
                    } else {
                        tr[y].lazyAdd = z;
                    }
                } else if (k == 3) {
                    cout << tr[y].mn << "\n";
                } else if (k == 4) {
                    cout << tr[y].mx << "\n";
                } else {
                    cout << tr[y].sum << "\n";
                }
            }
        }
    }

    return 0;
}