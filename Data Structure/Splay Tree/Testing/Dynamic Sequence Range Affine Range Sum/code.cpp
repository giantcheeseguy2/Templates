//https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum
#include <bits/stdc++.h>
using namespace std;

template<class mod>
struct Modular {

    using T = typename mod::type;
    using C = typename mod::cast;

    template<class U>
    static T fpow(T a, U b){
        T ret = 1;
        while(b){
            if(b%2 == 1) ret = (C)ret*a%mod::mod;
            a = (C)a*a%mod::mod;
            b /= 2;
        }
        return ret;
    }

    T val;

    Modular(){
        val = 0;
    }

    template<class U>
    Modular(const U &x){
        val = x%mod::mod;
        if(val < 0) val += mod::mod;
    }

    const T Mod() const { return mod::mod; }

    const T &operator()() const { return val; }

    template<class U>
    explicit operator U() const { return static_cast<U>(val); } 

    Modular operator-(){ return Modular<mod>(-val); }

    Modular &operator+=(const Modular &x){
        val = (val + x.val)%mod::mod;
        return *this;
    }

    Modular &operator-=(const Modular &x){
        val = (val - x.val + mod::mod)%mod::mod;
        return *this;
    }

    Modular &operator*=(const Modular &x){
        val = (C)val*x.val%mod::mod;
        return *this;
    }

    Modular &operator/=(const Modular &x){
        val = (C)val*fpow(x.val, mod::mod - 2)%mod::mod;
        return *this;
    }

    Modular &operator%=(const Modular &x){
        val = val%x.val;
        return *this;
    }

    template<class U>
    Modular &operator^=(const U &x){
        assert(("be careful when raising to a modded power", typeid(x) != typeid(*this)));
        val = fpow(val, x);
        return *this;
    }

    Modular &operator++(){ return *this += 1; }

    Modular &operator--(){ return *this -= 1; }

    Modular operator++(int){
        Modular ret = *this;
        ++*this;
        return ret;
    }

    Modular operator--(int){
        Modular ret = *this;
        --*this;
        return ret;
    }

    friend Modular operator+(const Modular &a, const Modular &b){ return Modular(a.val) += b; }

    friend Modular operator-(const Modular &a, const Modular &b){ return Modular(a.val) -= b; }

    friend Modular operator*(const Modular &a, const Modular &b){ return Modular(a.val) *= b; }

    friend Modular operator/(const Modular &a, const Modular &b){ return Modular(a.val) /= b; }

    friend Modular operator%(const Modular &a, const Modular &b){ return Modular(a.val) %= b; }

    template<class U>
    friend Modular operator^(const Modular &a, const U &b){ return Modular(a.val) ^= b; }

    bool hasSqrt(){
        return val == 0 || fpow(val, (mod::mod - 1)/2) == 1;
    }

    Modular sqrt(){
        if(val == 0) return 0;
        if(val == 1) return 1;
        assert(hasSqrt());
        Modular r = 1, c = 1, a = 1, b = 1, aa, bb;
        while(((r*r - val) ^ ((mod::mod - 1)/2)) == 1) r++;
        Modular mult = r*r - val;
        T x = (mod::mod + 1)/2;
        while(x){
            if(x%2 == 1){
                aa = c*b*mult + a*r;
                bb = c*a + r*b;
                a = aa, b = bb;
            }
            aa = c*c*mult + r*r;
            bb = 2*c*r;
            r = aa, c = bb;
            x /= 2;
        }
        if(a >= (mod::mod + 1)/2) a *= -1;
        return a;
    }

    friend bool operator<(const Modular &a, const Modular &b){ return a.val < b.val; }

    friend bool operator<=(const Modular &a, const Modular &b){ return a.val <= b.val; }

    friend bool operator>(const Modular &a, const Modular &b){ return a.val > b.val; }

    friend bool operator>=(const Modular &a, const Modular &b){ return a.val >= b.val; }

    friend bool operator==(const Modular &a, const Modular &b){ return a.val == b.val; }

    friend bool operator!=(const Modular &a, const Modular &b){ return a.val != b.val; }

    friend ostream &operator<<(ostream &out, Modular x){ return out << x.val; }

    friend istream &operator>>(istream &in, Modular &x){ 
        in >> x.val;
        x.val = x.val%mod::mod;
        if(x.val < 0) x.val += mod::mod;
        return in;
    }

    string to_string(const Modular&x) { return to_string(x.val); }
};

struct Mod { 
    using type = int;
    using cast = long long;
    const static type mod = 998244353;
};

using mint = Modular<Mod>;

struct Splay {

    using T = mint;
    using L = pair<mint, mint>;

    static L empty(){ return {1, 0}; }

    static T comb(T a, T b){
        return a + b;
    }

    static void apply_val(T &x, L v, int sz){
        x = v.first*x + v.second*sz;
    }

    static void apply_tag(L &x, L v, int sz){
        x = {v.first*x.first, v.first*x.second + v.second};
    }

    struct node {
        int par, sz, ind, rev;
        array<int, 2> child;
        T val, sum;
        L tag;

        node(int p, int i, T v){
            par = p;
            ind = i;
            val = sum = v;
            sz = 1;
            child = {-1, -1};
            tag = empty();
            rev = 0;
        }

        int l(){ return child[0]; }
        int r(){ return child[1]; }
    };
    
    int n;
    vector<node> splay;

    Splay(){
        n = 0;
    }

    void pull(int ind){
        if(ind == -1) return;
        splay[ind].sz = 1;
        splay[ind].sum = splay[ind].val;
        for(int i = 0; i <= 1; i++){
            int nxt = splay[ind].child[i];
            if(nxt != -1){
                splay[ind].sum = comb(splay[ind].sum, splay[nxt].sum);
                splay[ind].sz += splay[nxt].sz;
            }
        }
    }

    void push(int ind){
        if(ind == -1) return;
        if(splay[ind].rev == 0 && splay[ind].tag == empty()) return;
        if(splay[ind].rev) swap(splay[ind].child[0], splay[ind].child[1]);
        for(int i = 0; i <= 1; i++){
            int nxt = splay[ind].child[i];
            if(nxt != -1){
                int sz = splay[nxt].sz;
                apply_val(splay[nxt].val, splay[ind].tag, 1);
                apply_val(splay[nxt].sum, splay[ind].tag, sz);
                apply_tag(splay[nxt].tag, splay[ind].tag, sz);
                splay[nxt].rev ^= splay[ind].rev;
            }
        }
        splay[ind].rev = 0;
        splay[ind].tag = empty();
    }

    void reserve(int _n){
        splay.reserve(_n);
    }

    int newnode(T v){
        splay.push_back(node(-1, n, v));
        return n++;
    }

    //attaches a, to b's side
    void attach(int a, int b, int side){
        splay[b].child[side] = a;
        if(a != -1) splay[a].par = b;
    }

    int side(int ind){
        assert(splay[ind].par != -1);
        return splay[splay[ind].par].child[1] == ind;
    }

    //rotates ind with its parent
    void balance(int ind){
        if(splay[ind].par == -1) return;
        int s = side(ind), p = splay[ind].par;
        push(p);
        push(ind);
        if(splay[p].par != -1) attach(ind, splay[p].par, side(p));
        else splay[ind].par = -1;
        attach(splay[ind].child[!s], p, s);
        splay[ind].child[!s] = -1;
        attach(p, ind, !s);
        pull(p);
        pull(ind);
    }

    //finds the node at index x in rt's tree
    int getnode(int rt, int x){
        assert(0 <= x && x < splay[rt].sz);
        int tmp = rt, prv = x, last = -1;
        while(true){
            push(rt);
            int nxt = (splay[rt].l() == -1 ? 0 : splay[splay[rt].l()].sz);
            last = rt;
            if(x == nxt) return rt;
            if(x <= nxt) rt = splay[rt].l();
            else x -= nxt + 1, rt = splay[rt].r();
        }
    }

    //moves ind to root
    int reroot(int ind, int tar = -1){
        while(splay[ind].par != tar){
            if(splay[splay[ind].par].par != tar){
                balance((side(ind) == side(splay[ind].par) ? splay[ind].par : ind));
            }
            balance(ind);
        }
        return ind;
    }

    //merges two trees a and b
    int merge(int a, int b){
        if(a == -1) return b;
        if(b == -1) return a;
        int last = reroot(getnode(a, splay[a].sz - 1));
        push(last);
        splay[b].par = last;
        splay[last].child[1] = b;
        pull(last);
        return last;
    }

    //splits rt into [0, k], (k, sz)
    pair<int, int> split(int rt, int k){
        if(k == splay[rt].sz - 1) return pair<int, int>{rt, -1};
        if(k == -1) return pair<int, int>{-1, rt};   
        int a = reroot(getnode(rt, k)), b = splay[a].child[1]; 
        push(a);
        splay[a].child[1] = -1;
        splay[b].par = -1;
        pull(a);
        return pair<int, int>{a, b};
    }

    //inserts v at index x in rt's tree
    void insert(int &rt, int x, T v){
        if(rt == -1){
            rt = newnode(v);
            return;
        }
        assert(0 <= x && x <= splay[rt].sz);
        pair<int, int> nw = split(rt, x - 1);
        int cur = newnode(v);
        rt = merge(nw.first, merge(cur, nw.second));
        rt = reroot(cur);
    }

    //removes the value at index in rt's tree
    void erase(int &rt, int x){
        assert(0 <= x && x < splay[rt].sz);
        rt = reroot(getnode(rt, x));
        if(splay[rt].child[0] != -1) splay[splay[rt].child[0]].par = -1;
        if(splay[rt].child[1] != -1) splay[splay[rt].child[1]].par = -1;
        rt = merge(splay[rt].child[0], splay[rt].child[1]);
    }

    //rotates l...r k, l_i = l_(i - k)
    void rotate(int &rt, int l, int r, int k){
        if(l == r) return;
        k %= r - l + 1;
        if(k == 0) return;
        pair<int, int> a = split(rt, l - 1);
        pair<int, int> b = split(a.second, r - l);
        pair<int, int> cur = split(b.first, k - 1);
        rt = merge(a.first, merge(cur.second, merge(cur.first, b.second)));
    }

    void update(int &rt, int l, int r, L v){
        assert(0 <= l && r < splay[rt].sz);
        if(0 == l && r == splay[rt].sz - 1){
            apply_val(splay[rt].val, v, 1);
            apply_val(splay[rt].sum, v, splay[rt].sz);
            apply_tag(splay[rt].tag, v, splay[rt].sz);
        } else if(l == 0){
            rt = reroot(getnode(rt, r + 1));
            int lc = splay[rt].l();
            apply_val(splay[lc].val, v, 1);
            apply_val(splay[lc].sum, v, splay[lc].sz);
            apply_tag(splay[lc].tag, v, splay[lc].sz);
            rt = reroot(lc);
        } else if(r == splay[rt].sz - 1){
            rt = reroot(getnode(rt, l - 1));
            int rc = splay[rt].r();
            apply_val(splay[rc].val, v, 1);
            apply_val(splay[rc].sum, v, splay[rc].sz);
            apply_tag(splay[rc].tag, v, splay[rc].sz);
            rt = reroot(rc);
        } else {
            rt = reroot(getnode(rt, l - 1));
            reroot(getnode(rt, r + 1), rt);
            int c = splay[splay[rt].r()].l();
            apply_val(splay[c].val, v, 1);
            apply_val(splay[c].sum, v, splay[c].sz);
            apply_tag(splay[c].tag, v, splay[c].sz);
            rt = reroot(c);
        }
    }

    void reverse(int &rt, int l, int r){
        assert(0 <= l && r < splay[rt].sz);
        if(0 == l && r == splay[rt].sz - 1){
            splay[rt].rev ^= 1;
        } else if(l == 0){
            rt = reroot(getnode(rt, r + 1));
            int lc = splay[rt].l();
            splay[lc].rev ^= 1;
            rt = reroot(lc);
        } else if(r == splay[rt].sz - 1){
            rt = reroot(getnode(rt, l - 1));
            int rc = splay[rt].r();
            splay[rc].rev ^= 1;
            rt = reroot(rc);
        } else {
            rt = reroot(getnode(rt, l - 1));
            reroot(getnode(rt, r + 1), rt);
            int c = splay[splay[rt].r()].l();
            splay[c].rev ^= 1;
            rt = reroot(c);
        }
    }

    T query(int &rt, int l, int r){
        assert(0 <= l && r < splay[rt].sz);
        if(l == r){
            rt = reroot(getnode(rt, l));
            return splay[rt].val;
        }
        if(0 == l && r == splay[rt].sz - 1){
            return splay[rt].sum;
        } else if(l == 0){
            rt = reroot(getnode(rt, r + 1));
            return splay[splay[rt].l()].sum;
        } else if(r == splay[rt].sz - 1){
            rt = reroot(getnode(rt, l - 1));
            return splay[splay[rt].r()].sum;
        } else {
            rt = reroot(getnode(rt, l - 1));
            reroot(getnode(rt, r + 1), rt);
            return splay[splay[splay[rt].r()].l()].sum;
        }
    }
};

int main(){
    int n, q;
    cin >> n >> q;
    Splay splay;
    splay.reserve(n + q);
    int rt = -1;
    for(int i = 0; i < n; i++){
        int x;
        cin >> x;
        splay.insert(rt, i, x);
    }
    while(q--){
        int t;
        cin >> t;
        if(t == 0){
            int a, b;
            cin >> a >> b;
            splay.insert(rt, a, b);
        } else if(t == 1){
            int x;
            cin >> x;
            splay.erase(rt, x);
        } else if(t == 2){
            int l, r;
            cin >> l >> r;
            splay.reverse(rt, l, r - 1);
        } else if(t == 3){
            int l, r, b, c;
            cin >> l >> r >> b >> c;
            splay.update(rt, l, r - 1, {b, c});
        } else if(t == 4){
            int l, r;
            cin >> l >> r;
            cout << splay.query(rt, l, r - 1) << "\n";
        } else if(t == 5){
            int l, r, k;
            cin >> l >> r >> k;
            splay.rotate(rt, l, r - 1, k);
        }
    }
}